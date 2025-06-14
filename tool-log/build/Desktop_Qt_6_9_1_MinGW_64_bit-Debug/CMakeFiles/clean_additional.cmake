# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\tool-log_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\tool-log_autogen.dir\\ParseCache.txt"
  "tool-log_autogen"
  )
endif()
