// APPLE path is different (thanks Minami)
#ifdef __APPLE__
#  include <GLUT/glut.h>
#  include "SDL/SDL.h"
#  include "SDL/SDL_thread.h"
#  ifdef HAVE_SDL_MIXER
#    include "SDL/SDL_mixer.h"
#  endif
#  ifdef HAVE_SDL_NET
#    include "SDL/SDL_net.h"
#  endif
#else
#  ifdef USE_GLUT
#    include <GL/glut.h>
#  else
#    include <GL/glu.h>
#    include "SDL/SDL.h"
#    include "SDL/SDL_thread.h"
#    ifdef HAVE_SDL_MIXER
#      include "SDL/SDL_mixer.h"
#    endif
#    ifdef HAVE_SDL_NET
#      include "SDL/SDL_net.h"
#    endif
#  endif
#endif // __APPLE__

#ifdef _MSC_VER
  #pragma warning(disable:4786) // workaround for MSVC6 bug, needs service pack
#endif
