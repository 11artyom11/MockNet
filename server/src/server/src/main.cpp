#include <server.h>

using namespace Server;

int main(int argc, char *argv[]) {
  // Create a socket (IPv4, TCP)

  if (argc != 2) {
    //debug_f_console("1 parameter expected (port) 2 were given: terminating....");
    return 0;
  }

  uint32_t port = atoi(argv[1]);

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    //debug_f_console("Failed to create socket, terminating....");
    exit(EXIT_FAILURE);
  }

  ServerModel servModel(port);
  ThreadPool th_pool;
  sockaddr_in *sockaddr = servModel.get_server_addr();

  servModel.dump_server_state();

  /*bind a socket to sockaddr*/
  if (servModel.bind_client_socket(sockfd) != 0) {
    //debug_f_console("Failed to bind socket to sockaddr_t instance, terminating...");
    exit(EXIT_FAILURE);
  }

  // Start listening. Hold at most 10 connections in the queue
  if (servModel.listen_to_socket(sockfd, 1)){
    //debug_f_console("Failed to listen on socket. errno: ", errno, ", terminating...");
    exit(EXIT_FAILURE);
  }
  int connection;
  while (1) {
    // Grab a connection from the queue
    connection = servModel.accept_connection_from_socket(sockfd);
    th_pool.submit([&servModel, &connection] { servModel.handle_connection(connection); });
  }
  close(sockfd);
}