function(project_frameworkvr_Debug_Win64)
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
    ../../src/vr/all
    $(OVRSDKROOT)/LibOVR/Include
  )
  include_directories(${INCLUD_DIRS})

  set(LIB_DIRS
    $(OVRSDKROOT)/LibOVR/Lib/Windows/x64/Debug/cmake
  )
  link_directories(${LIB_DIRS})

  set(SRC 
    ../../src/vr/all/frm/AppSampleVr.h
    ../../src/vr/all/frm/AppSampleVr.cpp
  )
  add_library( frameworkvr_Debug_Win64 STATIC ${SRC})
  set_target_properties( frameworkvr_Debug_Win64 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/lib"
    LIBRARY_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/lib"
    RUNTIME_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/lib"
    OUTPUT_NAME  "frameworkvr"
  )
endfunction(project_frameworkvr_Debug_Win64)
project_frameworkvr_Debug_Win64()

function(project_frameworkvr_Release_Win64)
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
    ../../src/vr/all
    $(OVRSDKROOT)/LibOVR/Include
  )
  include_directories(${INCLUD_DIRS})

  set(LIB_DIRS
    $(OVRSDKROOT)/LibOVR/Lib/Windows/x64/Release/cmake
  )
  link_directories(${LIB_DIRS})

  set(SRC 
    ../../src/vr/all/frm/AppSampleVr.h
    ../../src/vr/all/frm/AppSampleVr.cpp
  )
  add_library( frameworkvr_Release_Win64 STATIC ${SRC})
  set_target_properties( frameworkvr_Release_Win64 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/lib"
    LIBRARY_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/lib"
    RUNTIME_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/lib"
    OUTPUT_NAME  "frameworkvr"
  )
endfunction(project_frameworkvr_Release_Win64)
project_frameworkvr_Release_Win64()
