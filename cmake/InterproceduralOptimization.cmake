macro(Vandior_enable_ipo)
  include(CheckIPOSupported)
  check_ipo_supported(RESULT result OUTPUT output)
  if(result)
    message(STATUS "IPO is supported. Enabling Inter procedural Optimization.")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
  else()
    message(SEND_ERROR "IPO is not supported: ${output}")
  endif()
endmacro()
