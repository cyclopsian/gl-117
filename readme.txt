 GGGG L              1    1 77777
G     L             11   11    7
G GGG L     -----  1 1  1 1   7
G   G L              1    1  7
 GGGG LLLLL          1    1 7     (C) Thomas A. Drexl aka heptargon 2002


GL-117 is an action flight simulator written in C++ and OpenGL.
It is distributed unter the terms of the GNU Public License (GPL).


MINIMUM REQUIREMENTS:

- Linux or MSWindows
- GL, GLU (or compatible), GLUT
- SDL, SDL_mixer (both optional but strongly recommended)


MSWINDOWS INSTALLATION:

You might have to install GL, GLU, GLUT, and SDL, SDL_mixer
Look into your system directory, that is
 \WINDOWS\SYSTEM for Win9x/ME
 \WINDOWS\SYSTEM32 for WinNT/2000/XP
You need the files opengl32.dll, glu32.dll, glut32.dll, and sdl.dll,
sdl_mixer.dll there!
If one is missing, search the internet.
That's it. Execute the binary gl-117.exe in the windows directory!


LINUX INSTALLATION:

If you got a binary gl-117 in the linux directory, you will only need
	GL, GLU, GLUT, and SDL, SDL_mixer
installed on your system.
If you have to compile the sources, you will need
	GL-devel, GLU-devel, GLUT-devel, SDL-devel, SDL_mixer
OR:	mesa-devel, mesaglu-devel, mesaglut-devel, SDL-devel, SDL_mixer
as well.

To compile gl-117, just execute "./configure" and "make". The new binary
is in the src directory and may be executed.
As root enter "make install" to properly install all game files.
Then you may start the game just using "gl-117".
Never execute the game as root!


STARTUP:

When starting the game, it will test for a valid screen resolution
and save the startup settings in a file called "conf".
MSWindows users will locate this file in the "saves"-directory,
please execute "wordpad" to view the file.
On Linux/Unix the game will expect this file in the directory
"$HOME/.gl-117".
The configuration options are quite self-explanatory, adjust them
to your system.
If you lack a hardware accelerated video card, please set the
quality option to 0 or 1. Further acceleration can be achieved
negligating fullscreen mode and making the window smaller.


FAQ:

Libraries are installed, but gcc (linker) does not find them?
Try the command
 g++ -g -O2 -lm -o gl-117 main.o gl.o model.o 3ds.o audio.o land.o \
  glland.o aiobject.o effects.o roam.o net.o mathtab.o dirs.o \
  objload.o pilots.o \
  -lGL -lGLU -lglut -lSDL -lSDLmain -lSDL_mixer \
  -lXmu -lXext -lXi -lXxf86vm

Graphics do not initialize?
This game needs properly installed versions of OpenGL, GLU, and GLUT!

Cannot open GLX?
Try another bpp setting. (32, 16, 8)

Graphics too lame?
Sorry, OpenGL is not supported by every graphics card.
You will need at least an Nvidia TNT2 chipset to seriously play the game,
as well as the appropriate drivers.
The game has been tested in 800x600 mode on
 Athlon 900 TNT2, quality=2, view=50: FPS=Perfect
 Athlon 1400 GeForce2, quality=3, view=50: FPS=Perfect
