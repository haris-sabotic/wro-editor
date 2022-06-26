project('libglad', 'c')


compiler = meson.get_compiler('c')

inc = include_directories('include')
libglad = static_library('glad',
  'src/glad.c',
  include_directories : inc,
  dependencies: compiler.find_library('dl'))

libglad_dep = declare_dependency(include_directories : inc,
  link_with : libglad)
