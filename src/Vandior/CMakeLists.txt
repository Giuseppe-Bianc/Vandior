add_executable(vandior main.cpp)

target_link_libraries(
  vandior
  PRIVATE Vandior::Vandior_options
          Vandior::Vandior_warnings)

target_link_system_libraries(
  vandior
  PRIVATE
          CLI11::CLI11
          Vandior::vandior_lib
)

target_include_directories(vandior PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
