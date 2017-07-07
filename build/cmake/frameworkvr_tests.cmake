function(project_frameworkvr_tests_Debug_Win64)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_BUILD_TYPE RelWithDebInfo)

  add_definitions(
    -DAPT_DEBUG
    -DFRM_DEBUG
    -DGLEW_STATIC
  )

  set(INCLUD_DIRS 
    ../../src/all
    ../../src/all/extern
    ../../extern/ApplicationTools/src/all
    ../../extern/ApplicationTools/src/all/extern
    ../../src/win
    ../../extern/ApplicationTools/src/win
    ../../extern/ApplicationTools/src/win/extern
    ../../tests
    ../../src/vr/all
  )
  include_directories(${INCLUD_DIRS})

  set(LIB_DIRS
    $(OVRSDKROOT)/LibOVR/Lib/Windows/x64/Debug/cmake
  )
  link_directories(${LIB_DIRS})

  set(SRC 
    ../../tests/vr/frameworkvr_tests.cpp
  )
  add_executable( frameworkvr_tests_Debug_Win64 ${SRC})
  set_target_properties( frameworkvr_tests_Debug_Win64 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    LIBRARY_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    RUNTIME_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    OUTPUT_NAME  "frameworkvr_tests"
  )

  set(LIBS 
    LibOVR
    shlwapi
    hid
    opengl32
  )
  target_link_libraries(frameworkvr_tests_Debug_Win64 ${LIBS})
endfunction(project_frameworkvr_tests_Debug_Win64)
project_frameworkvr_tests_Debug_Win64()

function(project_frameworkvr_tests_Release_Win64)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_BUILD_TYPE RelWithDebInfo)

  add_definitions(
    -DGLEW_STATIC
  )

  set(INCLUD_DIRS 
    ../../src/all
    ../../src/all/extern
    ../../extern/ApplicationTools/src/all
    ../../extern/ApplicationTools/src/all/extern
    ../../src/win
    ../../extern/ApplicationTools/src/win
    ../../extern/ApplicationTools/src/win/extern
    ../../tests
    ../../src/vr/all
  )
  include_directories(${INCLUD_DIRS})

  set(LIB_DIRS
    $(OVRSDKROOT)/LibOVR/Lib/Windows/x64/Release/cmake
  )
  link_directories(${LIB_DIRS})

  set(SRC 
    ../../tests/vr/frameworkvr_tests.cpp
  )
  add_executable( frameworkvr_tests_Release_Win64 ${SRC})
  set_target_properties( frameworkvr_tests_Release_Win64 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    LIBRARY_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    RUNTIME_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    OUTPUT_NAME  "frameworkvr_tests"
  )

  set(LIBS 
    LibOVR
    shlwapi
    hid
    opengl32
  )
  target_link_libraries(frameworkvr_tests_Release_Win64 ${LIBS})
endfunction(project_frameworkvr_tests_Release_Win64)
project_frameworkvr_tests_Release_Win64()
