#include "server.h"
#include <gtest/gtest.h>


#define PORT 8888

using namespace Server;

TEST(SERVER_CONNECTIVITY_TEST, set_socket_listen){
    ServerModel servModel(PORT);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in* sockaddr = servModel.get_server_addr();
    int res = servModel.listen_to_socket(sockfd, 1);
    EXPECT_EQ(res,0);
}

int main(int argc, char* argv[])
{
    debug_i_console("Hello?");
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}