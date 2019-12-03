# GLEX
## A simple C++11 and OpenGL based 3d engine for homebrew app development

### The Goal

The goal of the GLEX library is to allow developers that do not have a deep understanding of OpenGL or console-specific low-level 3D graphics APIs to easily create non-trivial, useful, cross-platform 3D applications that can easily be tested on a PC and then run as-is on various retro consoles.

To support this goal, a selection of simple C++11 classes are provided that abstract away as many of the low level details as possible.

For example, GLEX includes simple classes to show basic 3D models, textured 3D models exported in obj format from applications like Blender, basic bitmap font rendering, with 2D graphics and input handling coming soon.

### The What

GLEX is a small (mostly) self-contained library for creating OpenGL based homebrew applications for retro consoles. 

Currently only the Dreamcast is supported using the great [GLdc](https://gitlab.com/simulant/GLdc) library written by the [Simulant](https://gitlab.com/simulant/simulant) team, though Original Xbox support is planned next, and PC support is already partially implemented (currently Mac only, though that will expand to support Windows and Linux soon).

In contrast to most (all?) other 3D libraries available for retro consoles that are generally either very low level (like GLdc) or full game engines (like Simulant), GLEX is focused on providing simple high-level C++11 primitives needed to create applications and nothing more.

### The Why

It occurred to me that almost all console homebrew written for powerful 3D consoles only uses 2D. I think this is due to the high learning curve of getting into 3D graphics programming coupled with the low barrier to entry and high availability of ports of 2D frameworks like SDL. These frameworks make it simple to create and test a UI on a PC and then run it on a console. GLEX hopes to make it similarly easy to create and test 3D UIs.

I first started writing this library as a way to learn OpenGL's fixed function and array based (but not VBO) draw modes, which tend to map well to retro consoles. For example most OpenGL console ports more or less support the OpenGL 1.2 spec. This means no shaders, no VBOs, etc. However, once I had made some progress, I realized the potential a library like this could have and decided to turn it from a personal learning project into a proper library.

Due to the fact that OpenGL has changed much since the fixed-function days, it can be hard to find information and tutorials on those legacy modes, making it even harder for a developer that is not already experienced in OpenGL or 3D graphics programming in general to start building something useful.

That's where GLEX attempts to fit in. By providing a set of (relatively) modern C++ classes to handle the basic needs of an application, and abstracting away the low-level details, ideally it should be as simple to create and test a 3D homebrew application as it is to create a 2D application using something like SDL.

### Getting started

Detailed compilation instructions coming soon...

### API Documentation

Coming soon...