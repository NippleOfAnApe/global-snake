Globa RPG Snake
=========
***
![GIF](resources/tailCut.gif)

### Building a snake with RPG elements and experimenting with what feels right

This is a port of my original [Snake game](https://github.com/NippleOfAnApe/rpg-rts-snake) but rewritten in C++ with all the OOP paradigms. This game is easily exetnsible. Key structures are:
 - GamaManager: a Mediator object that connects and tells how other objects are connected;
 - Player, FoodManager: Proxies that connect to concrete objects and manage their states.
 - Map: A bridge that mostly manages textures, but because there's not much going on there, it's also responsible for Camera logic.

This project uses CMake to generate a Makefile. It downloads Raylib automatically and builds the project. Simply navigate to this directory and run

```bash
    mkdir build && cd build
    cmake -S ../ -B .
    make
```
That should create an executable. That's it.

========

*Work in progress*

Development process is being documented in [Blog](BLOG.md)
  
### Icons from [Freepik](https://www.freepik.com/)

https://twitter.com/Cethyel
