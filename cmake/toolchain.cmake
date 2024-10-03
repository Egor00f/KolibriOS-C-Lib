
SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR x86)
SET(CMAKE_CROSSCOMPILING 1)

# Compiler
SET(CMAKE_C_COMPILER	kos32-gcc)
SET(CMAKE_CXX_COMPILER	kos32-g++)
SET(CMAKE_LINKER	kos32-ld)
SET(CMAKE_AR	kos32-ar)
set(CMAKE_STRIP	kos32-strip)
set(CMAKE_OBJCOPY	kos32-objcopy)

SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_AR> -s -c <TARGET>")
SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_AR> -s -c <TARGET>")

set(CMAKE_C_STANDARD_COMPUTED_DEFAULT "11")
set(CMAKE_C_COMPILER_VERSION "5.4.0")
set(CMAKE_CXX_STANDARD_COMPUTED_DEFAULT "11")
set(CMAKE_CXX_COMPILER_VERSION "5.4.0")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(CMAKE_C_COMPILER_FORCED TRUE)
set(CMAKE_CXX_COMPILER_FORCED TRUE)