```
  ________    __                        __        __    __________
 /   ___  \  /\ \                      /  \      /  \  /\________ \
/\  /___/\_\ \ \ \                    /    \    /    \ \/_______/\ \
\ \ \   \/_/_ \ \ \         _______  /\__/\ \  /\__/\ \         \/ /
 \ \ \  /\__ \ \ \ \       /\______\ \/_/\ \ \ \/_/\ \ \        / /
  \ \ \ \/_/\ \ \ \ \      \/______/      \ \ \     \ \ \      / /
   \ \ \___\/  \ \ \ \_______              \ \ \     \ \ \    /\ \
    \ \________/  \ \________\              \ \_\     \ \ \   \ \_\
     \/_______/    \/________/               \/_/      \/_/    \/_/
```

GL-117 is an arcade action flight simulator written in C++ and OpenGL. It was
originally developed from 2002-2004. It is distributed under the terms of the
GNU General Public License v2. This is an import of the original sourceforge
CVS repository with minor fixes and an update to a modern build system (meson).
The original project website is gone, but an archive can be seen
[here][gl-117-old].

## Building

Requirements are:

- SDL 1.2
- SDL_Mixer 1.2
- SDL_Net 1.2
- OpenGL
- GLU

Steps to build:

```sh
meson build
ninja -C build
sudo ninja -C build install
```

## Credits

The original credits are as follows:

- Thomas Drexl (developer)
- Jean-Marc Le Peuvedic (developer)
- Eugene Andreeschev (developer and designer)
- Eric Cheung (texture designer)
- Piotr Pawlow (lens flares, bugfixing)
- Louens Veen (mouse interface, landscape improvements)
- Arne Reiners (action physics model, cockpit improvements)
- Norbert Drexl (moon terrain)
- James Stone (cool soundtrack => version 2.0)
- Bernhard Kaindl (SuSE)
- Cedric Delfosse (Debian)

Additional credits:

- Jason Francis (meson support, bugfixing)

## See Also

- [Linux Air Combat][lac] - A rewrite of GL-117 but with a realistic WWII flight sim theme

[gl-117-old]: https://web.archive.org/web/20080904212658/http://www.heptargon.de/gl-117/gl-117.html
[lac]: https://askmisterwizard.com/2019/LinuxAirCombat/LinuxAirCombat.htm
