cmake_minimum_required(VERSION 3.16)

macro(set_and_check _var _file)
    set(${_var} "${_file}")
    if(NOT EXISTS "${_file}")
        message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
    endif()
endmacro()

set(HIDAPI_FOUND TRUE)

if(CMAKE_SIZEOF_VOID_P STREQUAL "4")
    set(_arch_subdir "x86")
elseif(CMAKE_SIZEOF_VOID_P STREQUAL "8")
    set(_arch_subdir "x64")
else()
    set(HIDAPI_FOUND FALSE)
    return()
endif()

set_and_check(HIDAPI_PREFIX       "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(HIDAPI_EXEC_PREFIX  "${CMAKE_CURRENT_LIST_DIR}/..")
set_and_check(HIDAPI_INCLUDE_DIR  "${HIDAPI_PREFIX}/include")
set(HIDAPI_INCLUDE_DIRS           "${HIDAPI_INCLUDE_DIR}")
set_and_check(HIDAPI_BINDIR       "${HIDAPI_PREFIX}/lib/${_arch_subdir}")
set_and_check(HIDAPI_LIBDIR       "${HIDAPI_PREFIX}/lib/${_arch_subdir}")

set(HIDAPI_LIBRARY      HIDAPI::hidapi)

set(_hidapi_library     "${HIDAPI_LIBDIR}/hidapi.lib")
set(_hidapi_dll_library "${HIDAPI_BINDIR}/hidapi.dll")
if(EXISTS "${_hidapi_library}" AND EXISTS "${_hidapi_dll_library}")
    if(NOT TARGET HIDAPI::hidapi)
        add_library(HIDAPI::hidapi SHARED IMPORTED)
        set_target_properties(HIDAPI::hidapi
            PROPERTIES
                INTERFACE_INCLUDE_DIRECTORIES "${HIDAPI_INCLUDE_DIRS}"
                IMPORTED_IMPLIB "${_hidapi_library}"
                IMPORTED_LOCATION "${_hidapi_dll_library}"
        )
    endif()
    set(HIDAPI_hidapi_FOUND TRUE)
else()
    set(HIDAPI_hidapi_FOUND FALSE)
endif()
unset(_hidapi_library)
unset(_hidapi_dll_library)
