cmake_minimum_required (VERSION 3.9)

project ("glslCompiler")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_FLAGS "-g")
set(CMAKE_CXX_FLAGS "-g")
set(VCPKG_ROOT $ENV{VCPKG_ROOT})

set(cpps srcs/main.cpp
  srcs/compiler.cpp
)

set(hpps srcs/compiler.hpp
)

add_executable(${PROJECT_NAME} ${cpps} ${hpps})

include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(glslang CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glslang::OSDependent
  glslang::glslang
  glslang::MachineIndependent
  glslang::GenericCodeGen
  glslang::OGLCompiler
  glslang::glslang-default-resource-limits
  glslang::SPVRemapper
  glslang::SPIRV
  glslang::HLSL
)
