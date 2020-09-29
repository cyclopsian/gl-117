project(
  'gl-117',
  'cpp',
  version: '1.3.2',
  license: 'GPL-2.0-or-later',
  default_options: [
    'cpp_std=c++03'
  ]
)

sdl = dependency('sdl')
sdlmixer = dependency('SDL_mixer', required: false)
sdlnet = dependency('SDL_net', required: false)
gl = dependency('gl')
glu = dependency('glu')

subdir('src')
subdir('doc')

gl_117_datadir = get_option('datadir') / meson.project_name()

subdir('maps')
subdir('models')
subdir('music')
subdir('sounds')
subdir('shaders')
subdir('textures')
subdir('units')
