project('libimgui', 'cpp')

inc = include_directories('.')

src = [ 'imgui.cpp',
        'imgui_demo.cpp',
        'imgui_draw.cpp',
        'imgui_tables.cpp',
        'imgui_widgets.cpp',
        'imgui_impl_glfw.cpp',
        'imgui_impl_opengl3.cpp' ]

libimgui = static_library('imgui', src,
         include_directories : inc)

libimgui_dep = declare_dependency(include_directories : inc,
             link_with: libimgui)