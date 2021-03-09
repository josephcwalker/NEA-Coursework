# NEA Coursework

AQA A-Level Computer Science Coursework

## Description

Each user is the owner of their own AI which they train to play connect 4. The AI will learn to play like the user it has been assigned. Not learn to win the game.

## Build Instructions

The repository already contains everything required to build the project.

Make sure to download all submodules as well as the code so that SFML and Eigen exist to be used within the code.

Simply, either using the premake5.exe within the *vendor* folder or a previously installed version, run premake5.lua using gmake2 as the argument.

This has not been tested for any other compilers other than with makefiles so the compatibility with Visual Studio has not been tested.

After generating the makefiles, the project can be compiled using *make* which should compile both the SFML submodule and the code itself producing a *bin* folder containing all the binaries.

The *res* folder will have to be copied to the directory containing the executable allowing it to use the images and fonts within that folder. Without this, no text or images will be displayed.