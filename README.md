# Games Engine Construction (VIS2068-N)

You are required to design and implement a basic 2D game engine using C++ and the HAPI API. To demonstrate the capabilities of this engine you should create a small game demo.

* There should be a clear separation between the systems of your game (black box systems).
* You should aim to implement a visulation system and a world model.
* Code should be bug free, well commented and maintainable.
* The demo must run initially in a window.

## Glass Engine
Glass Engine is a 2D component based game engine written on top of HAPI. There engine is split into blackboxed subsystems that can run independantly of each other. Engine features include:

### Graphics
* Sprites
* Spritesheets
* Sprites to background (optional optimisation)
* Dirty rectangles
* Module based particle system
### UI
* Button
* Image
* Text
* Font
### Audio
* Audio Source
### Input
* Controller Input
* Haptic Feedback
* Mouse & Keyboard Input
### Physics
* Gravity
* Point Gravity
* Simple Rigidbody 2D
### Misc.
* Hot level reloading
* Scene model through xml
* Gameobject heirachy
* Local multiplayer support
