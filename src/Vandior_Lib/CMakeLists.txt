include(GenerateExportHeader)


add_library(vandior_lib sample_library.cpp)



add_library(Vandior::vandior_lib ALIAS vandior_lib)

target_link_libraries(vandior_lib PRIVATE Vandior_options Vandior_warnings)

target_include_directories(vandior_lib ${WARNING_GUARD} PUBLIC $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                                                  $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_compile_features(vandior_lib PUBLIC cxx_std_20)

set_target_properties(
  vandior_lib
  PROPERTIES VERSION ${PROJECT_VERSION}
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN YES)

generate_export_header(vandior_lib EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/Vandior/sample_library_export.hpp)

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(vandior_lib PUBLIC VANDIOR_LIB_STATIC_DEFINE)
endif()
