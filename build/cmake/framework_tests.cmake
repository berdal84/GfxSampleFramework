function(project_framework_tests_Debug_Win64)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_BUILD_TYPE RelWithDebInfo)

  add_definitions(
    -DAPT_DEBUG
    -DFRM_DEBUG
  )

  set(INCLUD_DIRS 
    ../../src/win
    ../../extern/ApplicationTools/src/win
    ../../extern/ApplicationTools/src/win/extern
    ../../tests
  )
  include_directories(${INCLUD_DIRS})

  set(SRC 
    ../../tests/all/framework_tests.cpp
  )
  add_executable( framework_tests_Debug_Win64 ${SRC})
  set_target_properties( framework_tests_Debug_Win64 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    LIBRARY_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    RUNTIME_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    OUTPUT_NAME  "framework_tests"
  )

  set(LIBS 
    shlwapi
    hid
    opengl32
  )
  target_link_libraries(framework_tests_Debug_Win64 ${LIBS})
endfunction(project_framework_tests_Debug_Win64)
project_framework_tests_Debug_Win64()

function(project_framework_tests_Debug_Linux64)
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
    ../../tests
  )
  include_directories(${INCLUD_DIRS})

  set(SRC 
    ../../tests/all/framework_tests.cpp
  )
  add_executable( framework_tests_Debug_Linux64 ${SRC})
  set_target_properties( framework_tests_Debug_Linux64 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    LIBRARY_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    RUNTIME_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    OUTPUT_NAME  "framework_tests"
  )
endfunction(project_framework_tests_Debug_Linux64)
project_framework_tests_Debug_Linux64()

function(project_framework_tests_Release_Win64)
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_BUILD_TYPE RelWithDebInfo)

  add_definitions(
  )

  set(INCLUD_DIRS 
    ../../src/win
    ../../extern/ApplicationTools/src/win
    ../../extern/ApplicationTools/src/win/extern
    ../../tests
  )
  include_directories(${INCLUD_DIRS})

  set(SRC 
    ../../tests/all/framework_tests.cpp
  )
  add_executable( framework_tests_Release_Win64 ${SRC})
  set_target_properties( framework_tests_Release_Win64 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    LIBRARY_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    RUNTIME_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    OUTPUT_NAME  "framework_tests"
  )

  set(LIBS 
    shlwapi
    hid
    opengl32
  )
  target_link_libraries(framework_tests_Release_Win64 ${LIBS})
endfunction(project_framework_tests_Release_Win64)
project_framework_tests_Release_Win64()

function(project_framework_tests_Release_Linux64)
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
    ../../tests
  )
  include_directories(${INCLUD_DIRS})

  set(SRC 
    ../../tests/all/framework_tests.cpp
  )
  add_executable( framework_tests_Release_Linux64 ${SRC})
  set_target_properties( framework_tests_Release_Linux64 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    LIBRARY_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    RUNTIME_OUTPUT_DIRECTORY "/home/dallecortb/GfxSampleFramework/bin"
    OUTPUT_NAME  "framework_tests"
  )
endfunction(project_framework_tests_Release_Linux64)
project_framework_tests_Release_Linux64()
