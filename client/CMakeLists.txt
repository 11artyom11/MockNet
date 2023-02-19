cmake_minimum_required(VERSION 2.8.9)


#project name : client_server
project (client_server)

include(FetchContent)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set (THIRD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)

function(CloneRepository repositoryURL branchName projectName sourceDir)
  #Commands are left empty so that we only checkout the source and no not perform any kind of build
  message("Starting to clone ${projectName} into ${sourceDir}")
  FetchContent_Declare(
    "${projectName}"
    GIT_REPOSITORY "${repositoryURL}"
    SOURCE_DIR "${sourceDir}"
    GIT_TAG "origin/${branchName}"
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
  )
  FetchContent_MakeAvailable(${projectName})
endfunction(CloneRepository)

CloneRepository("https://github.com/nlohmann/json.git" "master" "json" "${THIRD_PARTY_DIR}/json")

#adding subproject client
add_subdirectory( src/client)

#adding shared lib helpers
add_subdirectory(lib/src/helpers)

#adding shared lib utilities
add_subdirectory(lib/src/util)
