set(LIBNAME ThreadIf)

add_library(${LIBNAME} INTERFACE)
target_include_directories(${LIBNAME} INTERFACE ./Include)

if(NOT CROSS_COMPILING)
  # add_subdirectory(Test)
  add_subdirectory(Mocks)
else()
  add_subdirectory(Source)
endif()