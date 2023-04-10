# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\qt_mvc_test_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\qt_mvc_test_autogen.dir\\ParseCache.txt"
  "qt_mvc_test_autogen"
  "src\\module\\event_bus\\CMakeFiles\\event_bus_autogen.dir\\AutogenUsed.txt"
  "src\\module\\event_bus\\CMakeFiles\\event_bus_autogen.dir\\ParseCache.txt"
  "src\\module\\event_bus\\event_bus_autogen"
  )
endif()
