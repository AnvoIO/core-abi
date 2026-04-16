# copyright defined in LICENSE

cmake_minimum_required (VERSION 3.16...4.0)
project(core-abi VERSION 1.0 LANGUAGES CXX C)

set(default_build_type "Release")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
      STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(CORE_ABI_NO_INT128 "disable use of __int128" OFF)
option(CORE_ABI_ONLY_LIBRARY "define and build the core-abi library" OFF)

if(NOT DEFINED SKIP_SUBMODULE_CHECK)
  execute_process(COMMAND git submodule status --recursive
                  COMMAND grep -c "^[+\-]"
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  OUTPUT_VARIABLE submodule_status
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(submodule_status GREATER 0)
     message(FATAL_ERROR "git submodules are not up to date.
Please run the command 'git submodule update --init --recursive'.")
  endif()
endif()

find_package(Threads)
include(GNUInstallDirs)

add_library(core-abi STATIC src/abi.cpp src/crypto.cpp)
target_include_directories(core-abi PUBLIC
                          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/rapidjson/include>"
                          "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

if(CORE_ABI_NO_INT128)
target_compile_definitions(core-abi PUBLIC CORE_ABI_NO_INT128)
endif()

add_library(core-abi_module MODULE src/core_abi.cpp src/abi.cpp src/crypto.cpp)
target_include_directories(core-abi_module PUBLIC
                          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                          "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/external/rapidjson/include>"
                          "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(core-abi_module ${CMAKE_THREAD_LIBS_INIT})
set_target_properties(core-abi_module PROPERTIES OUTPUT_NAME "core-abi")

enable_testing()

add_executable(test_core_abi src/test.cpp src/core_abi.cpp src/ship.abi.cpp)
target_link_libraries(test_core_abi core-abi ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_core_abi COMMAND test_core_abi)

if(NOT CORE_ABI_NO_INT128)
    add_executable(test_core_abi_template src/template_test.cpp src/core_abi.cpp)
    target_link_libraries(test_core_abi_template core-abi ${CMAKE_THREAD_LIBS_INIT})
    add_test(NAME test_core_abi_template COMMAND test_core_abi_template)
endif()

add_executable(test_core_abi_key src/key_test.cpp src/core_abi.cpp)
target_link_libraries(test_core_abi_key core-abi ${CMAKE_THREAD_LIBS_INIT})
add_test(NAME test_core_abi_key COMMAND test_core_abi_key)

add_executable(test_core_abi_reflect src/reflect_test.cpp)
target_include_directories(test_core_abi_reflect PRIVATE include)
add_test(NAME test_core_abi_reflect COMMAND test_core_abi_reflect)

# Causes build issues on some platforms
# add_executable(test_core_abi_sanitize src/test.cpp src/core_abi.cpp src/abi.cpp src/crypto.cpp)
# target_include_directories(test_core_abi_sanitize PRIVATE include external/outcome/single-header external/rapidjson/include external/date/include)
# target_link_libraries(test_core_abi_sanitize core-abi -fno-omit-frame-pointer -fsanitize=address,undefined ${CMAKE_THREAD_LIBS_INIT})
# target_compile_options(test_core_abi_sanitize PUBLIC -fno-omit-frame-pointer -fsanitize=address,undefined)
# add_test(NAME test_core_abi_sanitize COMMAND test_core_abi_sanitize)

# add_executable(fuzzer src/fuzzer.cpp src/core_abi.cpp)
# target_include_directories(fuzzer PRIVATE external/outcome/single-header external/rapidjson/include external/date/include)
# target_link_libraries(fuzzer -fsanitize=fuzzer,address,undefined,signed-integer-overflow -fstandalone-debug ${CMAKE_THREAD_LIBS_INIT})
# target_compile_options(fuzzer PRIVATE -fsanitize=fuzzer,address,undefined,signed-integer-overflow -fstandalone-debug)

if (CMAKE_CXX_COMPILER_ID MATCHES Clang|AppleClang)
    target_compile_options(core-abi PRIVATE -Wall -Wextra -Wno-unused-parameter -fcolor-diagnostics)
    target_compile_options(test_core_abi PRIVATE -Wall -Wextra -Wno-unused-parameter -fcolor-diagnostics)
endif()

if (NOT CORE_ABI_ONLY_LIBRARY)
add_subdirectory(tools)
endif()

if(CORE_ABI_INSTALL_COMPONENT)
   set(INSTALL_COMPONENT_ARGS COMPONENT ${CORE_ABI_INSTALL_COMPONENT} EXCLUDE_FROM_ALL)
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/core_net DESTINATION include ${INSTALL_COMPONENT_ARGS})
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/eosio DESTINATION include ${INSTALL_COMPONENT_ARGS})

install(TARGETS core-abi
  EXPORT core-abi
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} ${INSTALL_COMPONENT_ARGS}
	ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR} ${INSTALL_COMPONENT_ARGS}
)

install(EXPORT core-abi
  DESTINATION "share/core-abi"
  FILE core-abi-targets.cmake
  ${INSTALL_COMPONENT_ARGS}
)

export(TARGETS core-abi
       FILE ${CMAKE_CURRENT_BINARY_DIR}/share/core-abi/core-abi-targets.cmake)
configure_file(core-abi-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/share/core-abi/core-abi-config.cmake COPYONLY)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/share/core-abi/core-abi-config.cmake
        DESTINATION "share/core-abi"
        ${INSTALL_COMPONENT_ARGS})
