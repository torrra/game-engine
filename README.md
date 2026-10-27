# Game engine project

This was a school project spanning over 4 months. The goal was to create a game engine from the ground up, in C++ and to develop a game using that engine!

Note: This project was developed for Windows and MSVC.

<br>

## Table of contents

* [Contributors](https://github.com/torrra/game-engine/main/README.md#contributors)
* [Features](https://github.com/torrra/game-engine/main/README.md#features)
* [Third-party dependencies](https://github.com/torrra/game-engine/main/README.md#dependencies)
* [The game](https://github.com/torrra/game-engine/edit/main/README.md#the-game)
* [Build](https://github.com/torrra/game-engine/main/README.md#build)

<br>

## Contributors

* [MetaXGhost](https://github.com/MetaXGhost) : Project build system, external library integration and main developer on [the game](https://github.com/torrra/game-engine/main/README.md#the-game)  
* [MLev29](https://github.com/MLev29) : UI, Resource management, Input API  
* [Torra](https://github.com/torrra) : Math, scripting, entity-component-systems implementation

<br>

## Features

The editor application has a dedicated window allowing us to edit the scene in real time.  

https://github.com/user-attachments/assets/4de71b9d-2bcb-43c7-9e8c-0308b97414f7

<br>
 
We can also visualize animations  

https://github.com/user-attachments/assets/e9695f8f-6e1d-42aa-87c1-66dfb1bbf075

<br>

and simulate the game from the editor!  

https://github.com/user-attachments/assets/49b052a6-e925-4a12-9a1f-f5a6c19c7167

<br>

* It is also possible to build an executable from the game, by simply clicking the "Build" button.
* Lua scripts can be attached to Entities (objects) as components, 

<br>

## Dependencies

Math library: [mathlib](https://github.com/torrra/mathlib) *by me :)*


Physics simulation: Nvidia PhysX  

Asset importing: assimp  
Sound engine: fmod  

Graphics API: OpenGL 4.5  
Windowing: glfw  

In-engine scripting: Lua  
  
No worries, CMake takes care of them all (see [Build](https://github.com/torrra/game-engine/main/README.md#build) for more)

<br>

## The game

We developed a simple first-person shooter, entirely with our engine! 

https://github.com/user-attachments/assets/70b44b29-ba24-44b4-844c-4c99f1608a48

<br>

## Build

This project uses CMake as its build system. To build it you will need:  
* Windows
* Git
* CMake 3.25 or higher
* Visual studio 2022 version 17
* An internet connection (to fetch mathlib from github)


### Step 1 : clone the repo

Open CMD and move open the directory in which you wish to install the repo. You can either press your Win key and type out 'cmd', then use 'cd <target_folder>", or open windows file explorer and right click inside to open in terminal. You can now enter the following command:

```bash
git clone https://github.com/torrra/game-engine.git
```

### Step 2 : generate visual studio solution

Once you have downloaded the repo, you need to open the folder with `cd game-engine`, and run CMake with either:

```bash
cd build
cmake ..
```

or

```bash
cmake -B build -S .
```

### Step 3 : open Visual Studio

After CMake has successfully configured the VS solution, you can open it with your file explorer, or by using 
```
cmake --open .
``` 
or ```cmake --open build``` if you ran the second CMake command shown in the previous step.  
Once Visual Studio has finished starting up, you can select Debug or Release and click on 'Local Windows Debugger' to compile everything. The application will start automatically.
