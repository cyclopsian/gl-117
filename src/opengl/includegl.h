// APPLE path is different (thanks Minami)
#ifndef __APPLE__
#  include <GL/glu.h>
#endif
#include <SDL.h>
#ifdef HAVE_SDL_MIXER
#  include <SDL_mixer.h>
#endif
#ifdef HAVE_SDL_NET
#  include <SDL_net.h>
#endif

#ifdef _MSC_VER
  #pragma warning(disable:4786) // workaround for MSVC6 bug, needs service pack
#endif
