# Loop-Backed ext2 Capability Evidence — 2026-09-29

## Scope

This checkpoint records the reversible loop-filesystem experiment performed on the physical Samsung Galaxy Y GT-S5360 JPLC1 handset after root and chroot capability were already confirmed.

The experiment used only a 4 MiB regular file under `/data/local/tmp`. It did not write any BML/STL partition, EFS, PIT, boot/recovery image, modem, or partition table.

## Preconditions observed

- Root access: `uid=0(root) gid=0(root)`.
- `/data`: Samsung RFS, read-write.
- Latest observed free space before the loop test: approximately 162.1 MiB.
- Loop device nodes: `/dev/block/loop0` through `/dev/block/loop7`, root:root, mode 0600.
- No `/dev/loop*` nodes exist on this Android environment.
- BusyBox: v1.17.2.
- BusyBox provides `losetup`, `mount`, `umount`, and `mke2fs`.

Applet presence alone was not treated as capability proof; each relevant operation was exercised on the live handset.

## Experiment sequence

### 1. Create a 4 MiB regular file

Command:

    /system/xbin/busybox dd if=/dev/zero of=/data/local/tmp/totoro-loop-test.img bs=1024 count=4096

Result:

    4096+0 records in
    4096+0 records out
    4194304 bytes
    dd_status=0

### 2. Create an ext2 filesystem

Command:

    /system/xbin/busybox mke2fs -F /data/local/tmp/totoro-loop-test.img

Observed:

    Block size=1024
    1024 inodes
    4096 blocks
    mke2fs_status=0

This proves the userspace tool can create a valid ext2 filesystem image without touching a physical partition.

### 3. Discover a free loop device

Command:

    /system/xbin/busybox losetup -f

Result:

    /dev/loop0
    losetup_find_status=0

The returned path did not correspond to an existing Android device node. The actual node was:

    /dev/block/loop0

This path distinction is specific to the observed Android device layout.

### 4. Attach the image to the real loop block device

Command:

    /system/xbin/busybox losetup /dev/block/loop0 /data/local/tmp/totoro-loop-test.img

Result:

    losetup_attach_status=0

The first attempt using `/dev/loop0` failed with `No such file or directory`; no device node was created. Retrying with the existing `/dev/block/loop0` succeeded.

### 5. Mount ext2 read-write

Mount point:

    /data/local/tmp/totoro-loop-mnt

Command:

    /system/xbin/busybox mount -t ext2 /dev/block/loop0 /data/local/tmp/totoro-loop-mnt

Result:

    mount_status=0

Live mount output:

    /dev/block/loop0 on /data/local/tmp/totoro-loop-mnt type ext2 (rw,relatime,errors=continue)

This is direct proof that the JPLC1 kernel can mount an ext2 filesystem backed by a regular file through a loop block device.

### 6. Verify read/write access

Write:

    echo 'LOOP_FS_WRITE_OK' > /data/local/tmp/totoro-loop-mnt/test.txt

Result:

    write_status=0

Read-back:

    /system/xbin/busybox cat /data/local/tmp/totoro-loop-mnt/test.txt

Result:

    LOOP_FS_WRITE_OK
    read_status=0

This proves read/write access to the mounted ext2 filesystem.

### 7. Execution interpretation

A shell-script execution attempt returned:

    test.sh: applet not found
    exec_status=1

The test script used:

    #!/system/xbin/busybox

This was not a valid proof of ext2 executable-file support because the installed BusyBox invocation treated the script path as an applet argument. The mount flags did not contain `noexec`, but no ELF binary was executed from the loop filesystem during this experiment.

Therefore this checkpoint deliberately records **loop-mounted ext2 read/write capability as proven**, while **execution of an ELF binary from the loop filesystem remains unproven**.

### 8. Clean teardown

Commands:

    /system/xbin/busybox umount /data/local/tmp/totoro-loop-mnt
    /system/xbin/busybox losetup -d /dev/block/loop0
    /system/xbin/busybox rm -f /data/local/tmp/totoro-loop-test.img
    /system/xbin/busybox rmdir /data/local/tmp/totoro-loop-mnt

Results:

    umount_status=0
    losetup_detach_status=0
    cleanup_status=0

Final verification reported both temporary paths as absent.

## Capability conclusion

The physical JPLC1 handset now has direct runtime evidence for:

- regular-file creation under `/data/local/tmp`;
- ext2 filesystem creation;
- loop-device discovery;
- loop-file attachment using `/dev/block/loop0`;
- read-write ext2 mounting through the old 2.6.35.7 kernel;
- read/write access inside the loop-mounted filesystem;
- clean unmount and loop detach.

The validated storage mechanism is therefore:

    /data
      |
      +-- regular ext2 image
              |
              v
        /dev/block/loop0
              |
              v
        ext2 mountpoint

This makes a filesystem-image rootfs technically viable for the Android-native userspace track.

## Storage implication

The latest measured `/data` free space was approximately 162.1 MiB. The loop experiment used only 4 MiB and is therefore not a capacity test for a full Linux rootfs.

The removable SD is VFAT and mounted `noexec`. It remains useful for transport/storage, but it should not be assumed to be a suitable direct execution root for Linux binaries. A loop-backed Linux filesystem on `/data` is currently the experimentally validated Linux-filesystem mechanism.

No NAND partition, BML/STL device, EFS, PIT, boot/recovery image, modem, or partition layout was modified.

## Next gate

Before deploying a real rootfs:

1. Select an ARMv6-compatible userspace candidate.
2. Validate its executable architecture/ABI and kernel compatibility offline.
3. Test a minimal candidate binary on the handset before unpacking a full rootfs.
4. Keep the first real rootfs deliberately small.
5. Prefer a loop-backed ext2 image if the final image size fits comfortably within the measured `/data` capacity.
6. Validate networking and the required `/proc`, `/sys`, and `/dev` interfaces separately before SSH integration.

