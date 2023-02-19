cmake_minimum_required(VERSION 2.8.9)
project (server_test)


#if these moduls are above current level link_directories helps us to locate shared libs for linkning
link_directories(../lib/src/util/data_transfer/message_model/)
link_directories(../lib/src/util/data_transfer/message_resolver/)
link_directories(../lib/src/helpers/)
# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS},
                    "../src/server/include/",
                    "../src/server/include/customer_model/",
                    "../lib/include/util/cyptolib/",
                    "../lib/include/helpers/debug_helper/",
                    "../lib/include/util/data_transfer/message_model/",
                    "../lib/include/util/data_transfer/message_resolver/",
                    "../src/server/include/chat_model/"
                    )

set (CMAKE_BINARY_DIR "./bin/")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_CXX_FLAGS "-g -w -O2 -std=c++17")

add_executable(server_test serv_model_test.cpp    
                    ../src/server/src/server.cpp 
                    ../src/server/src/handler.cpp
                    ../src/server/src/thman.cpp
                    ../src/server/src/customer_model/customer.cpp
                    ../src/server/src/customer_model/customer_crypto_unit.cpp
                    ../src/server/src/chat_model/chatroom_model.cpp
                    ../src/server/src/chat_model/chatroom_manager.cpp
)                        

target_link_libraries(server_test
                        ${GTEST_LIBRARIES} 
                            pthread
                                message_model
                                    message_resolver
                                        debug_helper)