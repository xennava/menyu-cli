cmake_minimum_required(VERSION 3.30)

project(menu LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_UNITY_BUILD ON)
set(CMAKE_UNITY_BUILD_BATCH_SIZE 0)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(menu ${SRCS})

target_include_directories(menu
    PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

  target_compile_options(menu PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
    )

  # Windows / MinGW compatibility
  if(WIN32)
    target_compile_options(menu PRIVATE
            -Wno-error=sign-compare
        )
    target_link_options(menu PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
    )

  endif()

  if(CMAKE_BUILD_TYPE STREQUAL "Debug")

    # Sanitizers: Linux native only
    if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
      target_compile_options(menu PRIVATE
                -fsanitize=address,undefined
                -fno-omit-frame-pointer
            )

      target_link_options(menu PRIVATE
                -fsanitize=address,undefined
            )
    endif()

  elseif(CMAKE_BUILD_TYPE STREQUAL "Release")

    target_link_options(menu PRIVATE
            -s
        )

  endif()

endif()

# Use LLD when available on native Linux
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  find_program(LLD_LINKER ld.lld)

  if(LLD_LINKER AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_options(menu PRIVATE
            -fuse-ld=lld
        )
  endif()
endif()

# ------------------------------------------------------------
# utf8proc
# ------------------------------------------------------------

set(UTF8PROC_INSTALL OFF CACHE BOOL "" FORCE)
set(UTF8PROC_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

add_subdirectory(
    "${PROJECT_SOURCE_DIR}/third_party/utf8proc-master"
    "${CMAKE_BINARY_DIR}/utf8proc"
)

target_link_libraries(menu
    PRIVATE
        utf8proc::utf8proc
)

# ------------------------------------------------------------
# Platform Dependencies
# ------------------------------------------------------------

if(WIN32)

  # Windows - PDCurses

  set(PDCURSES_DIR
        "${PROJECT_SOURCE_DIR}/third_party/pdcurses"
    )

  target_include_directories(menu BEFORE
        PRIVATE
            "${PDCURSES_DIR}/include"
    )

  target_link_libraries(menu
        PRIVATE
            "${PDCURSES_DIR}/lib/pdcurses.a"
    )

elseif(UNIX)

  # Linux / macOS - ncurses

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
