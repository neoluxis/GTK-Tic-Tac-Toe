cmake_minimum_required(VERSION 3.28)
project(TicTacToe C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK REQUIRED gtk+-3.0)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})
add_compile_options(${GTK_CFLAGS_OTHER})

add_executable(Arrow_Shoukyo main.c)
target_link_libraries(Arrow_Shoukyo ${GTK_LIBRARIES})
