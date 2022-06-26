project('wro-editor', 'cpp',
  default_options : ['warning_level=3', 'cpp_std=c++17'])

compiler = meson.get_compiler('cpp')

src = [ 'src/main.cpp',
        'src/shader.cpp',
        'src/game.cpp',
        'src/util.cpp' ]

inc = include_directories('include')

glad_proj = subproject('glad')
glad_dep = glad_proj.get_variable('libglad_dep')

dep = [ compiler.find_library('m'),
        compiler.find_library('dl'),
        compiler.find_library('GL'),
        compiler.find_library('glfw3'),
        compiler.find_library('pthread'),
        glad_dep ]

executable('wro_editor', src,
  dependencies: dep,
  include_directories: inc)
