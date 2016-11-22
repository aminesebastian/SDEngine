

set(command "C:/Program Files/CMake/bin/cmake.exe;--build;.;--config;Debug")
execute_process(
  COMMAND ${command}
  RESULT_VARIABLE result
  OUTPUT_FILE "G:/SD_Engine/SDEngine/Assimp/binaries/test/gtest/src/gtest-stamp/gtest-build-out.log"
  ERROR_FILE "G:/SD_Engine/SDEngine/Assimp/binaries/test/gtest/src/gtest-stamp/gtest-build-err.log"
  )
if(result)
  set(msg "Command failed: ${result}\n")
  foreach(arg IN LISTS command)
    set(msg "${msg} '${arg}'")
  endforeach()
  set(msg "${msg}\nSee also\n  G:/SD_Engine/SDEngine/Assimp/binaries/test/gtest/src/gtest-stamp/gtest-build-*.log")
  message(FATAL_ERROR "${msg}")
else()
  set(msg "gtest build command succeeded.  See also G:/SD_Engine/SDEngine/Assimp/binaries/test/gtest/src/gtest-stamp/gtest-build-*.log")
  message(STATUS "${msg}")
endif()
