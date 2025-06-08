set (BAB_SOURCES
  ${BAB_SOURCES_DIR}/bab/bab.c
  ${BAB_SOURCES_DIR}/bab/sdl_impl/sdl_impl.c
  ${BAB_SOURCES_DIR}/utils/endian.c
)

set (BAB_HEADERS
  ${BAB_SOURCES_DIR}/bab/sdl_impl/sdl_impl.h
  ${BAB_SOURCES_DIR}/utils/endian.h
)

set(BAB_INCLUDE_DIRS "${CMAKE_SOURCE_DIR}/include")
# foreach(_headerFile ${BAB_HEADERS})
#   get_filename_component(_dir ${_headerFile} PATH)
#   list(APPEND BAB_INCLUDE_DIRS ${_dir})
# endforeach()
list(REMOVE_DUPLICATES BAB_INCLUDE_DIRS)

include_directories(${CMAKE_SOURCE_DIR}/include)


# NOTE: Save
add_library(BABlib ${BAB_HEADERS} ${BAB_SOURCES})

target_include_directories(BABlib
  PUBLIC
  $<BUILD_INTERFACE:${BAB_SOURCES_DIR}>
  $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_link_libraries(BABlib PUBLIC SDL3::SDL3)

add_executable(BAB main.c)

target_link_libraries(BAB PUBLIC BABlib)
target_include_directories(BAB PUBLIC ${BAB_INCLUDE_DIRS})
set_target_properties(BAB PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BUILD_OUTPUT}/bin)

