# Totoro Native UI Architecture

## Goal

Replace heavy Android UI dependency with a minimal native rendering layer.

## Current Hardware Interface

Display:

- framebuffer device
- LCDfb kernel driver

Input:

- touchscreen
- keypad
- sensors


## Planned Stack

Application
    |
UI widgets
    |
Renderer
    |
Framebuffer backend
    |
Linux framebuffer


## Current State

Phase:
Hardware interface discovery

Status:

- framebuffer identified
- input devices identified
- native UI prototype exists
- no framebuffer ownership changes performed

