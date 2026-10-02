#include <errno.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <poll.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/utsname.h>
#include <unistd.h>

#define FBDEV "/dev/graphics/fb0"
#define INPUTDEV "/dev/input/event4"
#define LCDFB_IOCTL_UPDATE_LCD 0x46ffu

struct lcd_dirty_rows { unsigned int top, bottom; };

struct fbmap {
    int fd;
    uint8_t *mem;
    size_t len;
    unsigned int width, height, stride, yoffset;
};

static void px(struct fbmap *f, int x, int y, uint32_t c) {
    if (x < 0 || y < 0 || (unsigned)x >= f->width || (unsigned)y >= f->height) return;
    uint8_t *p = f->mem + (size_t)f->yoffset * f->stride + (size_t)y * f->stride + (size_t)x * 4;
    p[0]=(uint8_t)c; p[1]=(uint8_t)(c>>8); p[2]=(uint8_t)(c>>16); p[3]=(uint8_t)(c>>24);
}
static void rect(struct fbmap *f,int x,int y,int w,int h,uint32_t c){
    int yy,xx; for(yy=y;yy<y+h;yy++) for(xx=x;xx<x+w;xx++) px(f,xx,yy,c);
}
static void border(struct fbmap *f,int x,int y,int w,int h,int t,uint32_t c){
    rect(f,x,y,w,t,c); rect(f,x,y+h-t,w,t,c); rect(f,x,y,t,h,c); rect(f,x+w-t,y,t,h,c);
}
static const uint8_t D[10][7]={{14,17,19,21,25,17,14},{4,12,4,4,4,4,14},{14,17,1,2,4,8,31},{30,1,1,14,1,1,30},{2,6,10,18,31,2,2},{31,16,16,30,1,1,30},{6,8,16,30,17,17,14},{31,1,2,4,8,8,8},{14,17,17,14,17,17,14},{14,17,17,15,1,2,12}};
static const uint8_t U[26][7]={{14,17,17,31,17,17,17},{30,17,17,30,17,17,30},{14,17,16,16,16,17,14},{30,17,17,17,17,17,30},{31,16,16,30,16,16,31},{31,16,16,30,16,16,16},{14,17,16,23,17,17,15},{17,17,17,31,17,17,17},{14,4,4,4,4,4,14},{7,2,2,2,18,18,12},{17,18,20,24,20,18,17},{16,16,16,16,16,16,31},{17,27,21,17,17,17,17},{17,25,21,19,17,17,17},{14,17,17,17,17,17,14},{30,17,17,30,16,16,16},{14,17,17,17,21,18,13},{30,17,17,30,20,18,17},{15,16,16,14,1,1,30},{31,4,4,4,4,4,4},{17,17,17,17,17,17,14},{17,17,17,17,17,10,4},{17,17,17,21,21,21,10},{17,17,10,4,10,17,17},{17,17,10,4,4,4,4},{31,1,2,4,8,16,31}};
static void ch(struct fbmap*f,int x,int y,char c,int s,uint32_t col){
    static const uint8_t dash[7]={0,0,0,31,0,0,0}, dot[7]={0,0,0,0,0,12,12};
    const uint8_t*g=0; if(c>='A'&&c<='Z')g=U[c-'A']; else if(c>='0'&&c<='9')g=D[c-'0']; else if(c=='-')g=dash; else if(c=='.')g=dot;
    if(!g)return; int r,k,a,b; for(r=0;r<7;r++)for(k=0;k<5;k++)if(g[r]&(1u<<(4-k)))for(a=0;a<s;a++)for(b=0;b<s;b++)px(f,x+k*s+b,y+r*s+a,col);
}
static void text(struct fbmap*f,int x,int y,const char*s,int z,uint32_t c){while(*s){if(*s==' ')x+=6*z;else{ch(f,x,y,*s,z,c);x+=6*z;}s++;}}
static int refresh(struct fbmap*f){struct lcd_dirty_rows r={0,f->height-1};return ioctl(f->fd,LCDFB_IOCTL_UPDATE_LCD,&r);}
static void screen(struct fbmap*f,int on){
    const uint32_t BG=0xff101820u,P=0xff1d2a35u,W=0xffffffffu,A=0xff38d9a9u,O=0xffd96b38u;
    rect(f,0,0,f->width,f->height,BG); border(f,3,3,f->width-6,f->height-6,3,A);
    text(f,50,24,"TOTORO LINUX",3,W); text(f,42,58,"NATIVE UI",2,A);
    rect(f,28,92,184,72,P); border(f,28,92,184,72,2,A); text(f,42,108,"KERNEL",2,W); text(f,42,132,"LINUX",2,A);
    rect(f,28,184,184,70,on?O:P); border(f,28,184,184,70,3,W); text(f,on?56:72,207,on?"PRESSED":"TOUCH",2,W);
    text(f,28,276,"240-320",2,W);
}
static int button(int x,int y){return x>=28&&x<212&&y>=184&&y<254;}

int main(void){
    struct fb_fix_screeninfo fix; struct fb_var_screeninfo var; struct fbmap f={-1,0,0,0,0,0,0};
    struct utsname u; int in=-1,on=0,x=-1,y=-1;
    f.fd=open(FBDEV,O_RDWR); if(f.fd<0){perror(FBDEV);return 2;}
    if(ioctl(f.fd,FBIOGET_FSCREENINFO,&fix)<0||ioctl(f.fd,FBIOGET_VSCREENINFO,&var)<0){perror("FBIOGET");return 3;}
    if(var.xres!=240||var.yres!=320||var.bits_per_pixel!=32||fix.line_length!=960){fprintf(stderr,"unexpected framebuffer\n");return 4;}
    f.width=var.xres; f.height=var.yres; f.stride=fix.line_length; f.yoffset=var.yoffset; f.len=(size_t)f.stride*var.yres_virtual;
    f.mem=mmap(0,f.len,PROT_READ|PROT_WRITE,MAP_SHARED,f.fd,0); if(f.mem==MAP_FAILED){perror("mmap");return 5;}
    in=open(INPUTDEV,O_RDONLY); if(in<0){perror(INPUTDEV);return 6;}
    uname(&u);
    printf("TOTORO_NATIVE_UI\nFB %ux%u virtual_y=%u bpp=%u stride=%u yoffset=%u\nTOUCH %s\n",var.xres,var.yres,var.yres_virtual,var.bits_per_pixel,fix.line_length,var.yoffset,INPUTDEV);
    screen(&f,0); if(refresh(&f)<0){perror("LCDFB_IOCTL_UPDATE_LCD");return 7;}
    for(;;){
        struct pollfd p={in,POLLIN,0}; struct input_event e;
        int pr=poll(&p,1,1000); if(pr<0){if(errno==EINTR)continue;perror("poll");break;} if(!pr)continue;
        if(read(in,&e,sizeof(e))!=(ssize_t)sizeof(e))continue;
        if(e.type==EV_ABS&&e.code==ABS_MT_POSITION_X){x=e.value;if(x<0)x=0;if(x>239)x=239;}
        else if(e.type==EV_ABS&&e.code==ABS_MT_POSITION_Y){y=e.value;if(y<0)y=0;if(y>319)y=319;}
        else if(e.type==EV_SYN&&e.code==SYN_REPORT){if(x>=0&&y>=0&&button(x,y)){on=!on;printf("TOUCH x=%d y=%d state=%s\n",x,y,on?"ON":"OFF");screen(&f,on);if(refresh(&f)<0)break;}x=y=-1;}
    }
    munmap(f.mem,f.len);close(in);close(f.fd);return 0;
}
