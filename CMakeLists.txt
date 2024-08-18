cmake_minimum_required(VERSION 3.28)
project(TicTacToe C)

set(CMAKE_C_STANDARD 11)

include_directories(
        /usr/include/gtk-4.0
        /usr/include/pango-1.0
        /usr/include/harfbuzz
        /usr/include/gdk-pixbuf-2.0
        /usr/include/cairo
        /usr/include/glib-2.0
        /usr/lib/glib-2.0/include
        /usr/include/freetype2
        /usr/include/graphene-1.0
        /usr/lib/graphene-1.0/include
        /usr/include/libmount
        /usr/include/blkid
        /usr/include/fribidi
        /usr/include/sysprof-6
        /usr/include/libpng16
        /usr/include/pixman-1
)

link_libraries(
        gtk-4
        harfbuzz
        pangocairo-1.0
        pango-1.0
        gdk_pixbuf-2.0
        cairo-gobject
        cairo
        vulkan
        graphene-1.0
        gio-2.0
        glib-2.0
        gobject-2.0
)

add_executable(TicTacToe main.c
#        game.c
#        game.h)
)