cmake_minimum_required(VERSION 3.30)
project(menu CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 0)

file(GLOB SRCS CONFIGURE_DEPENDS src/*.cpp)

add_link_options(-fuse-ld=lld)
if (CMAKE_BUILD_TYPE STREQUAL Debug)
  add_compile_options(-fsanitize=address,leak -Wall -Werror)
  add_link_options(-fsanitize=address,leak)
elseif(CMAKE_BUILD_TYPE STREQUAL Release)
  add_compile_options(-O2 -s)
  add_link_options(-s)
endif()

add_executable(menu ${SRCS})

target_include_directories(menu
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

set(UTF8PROC_INSTALL OFF CACHE BOOL "" FORCE)
set(UTF8PROC_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

add_subdirectory(
    ${PROJECT_SOURCE_DIR}/third_party/utf8proc-master
    ${CMAKE_BINARY_DIR}/utf8proc
)

target_link_libraries(menu
    PRIVATE
        utf8proc::utf8proc
)

if(WIN32)

  # --------------------------------------------------------
  # Windows - PDCurses
  # --------------------------------------------------------

  set(PDCURSES_DIR
        ${PROJECT_SOURCE_DIR}/third_party/pdcurses
    )

  target_include_directories(menu
        PRIVATE
            ${PDCURSES_DIR}/include
    )

  target_link_libraries(menu
        PRIVATE
            ${PDCURSES_DIR}/lib/pdcurses.a
    )


elseif(UNIX)

  # --------------------------------------------------------
  # Linux / MacOS - ncurses
  # --------------------------------------------------------

  find_package(Curses REQUIRED)

  target_include_directories(menu
        PRIVATE
            ${CURSES_INCLUDE_DIRS}
    )

  target_link_libraries(menu
        PRIVATE
            ${CURSES_LIBRARIES}
    )

endif()
