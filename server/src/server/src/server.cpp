#include <server.h>

using namespace DataTransfer;

/**
 * @brief this function is deprecated since 20 Mar 2022
 * use generic connnection handlers instead
 *
 * @param msg
 * @return true if close command in $msg found
 * @return false otherwise
 *
 */
bool Server::is_client_connection_close(const char* msg) {
  for (int i = 0; i < strlen(msg); ++i) {
    if (msg[i] == EXIT_SYM) {
      return true;
    }
  }
  return false;
}

/**
 * @brief Construct a new Server:: Server Model object =>
 *  default constructor
 *
 */
Server::ServerModel::ServerModel(uint32_t port)
    : listen_port{port},
      protocol_family{AF_INET},
      listen_ip{INADDR_ANY}

{
  debug_i_console("Constructed ServerModel instance");
  server_addr = std::make_unique<struct sockaddr_in>();
  server_addr->sin_port = htons(listen_port);
  server_addr->sin_family = protocol_family;
  server_addr->sin_addr.s_addr = listen_ip;
  m_handler = std::make_unique<Handler>(3);
}

/**
 * @brief sets working port on which server will be listening to
 *
 * @param port
 * @return true if succeed to set listen port else false
 */
bool Server::ServerModel::set_server_port(uint16_t port) {
  debug_i_console("Server port set to ", port);
  std::string debugMes = "Set server port to " + (port + '0');
  server_addr->sin_port = htons(port);
  // Debug() << std::string{debugMes};
  this->listen_port = port;
  return true;
}

/**
 * @brief sets working protocol family (ipV4 as default)
 *
 * @param family
 * @return true if succeed to set protocol family
 * @return false if failed to set protocol family
 */
bool Server::ServerModel::set_protocol_family(uint16_t family) {
  debug_i_console("Server protocol family set to ", family);
  std::string debugMes = "Set protocol family to " + (family + '0');
  // Debug() << std::string{debugMes};
  server_addr->sin_family = family;
  this->protocol_family = family;
  return true;
}

/**
 * @brief Sets ip on which server will be listening to
 *  (INADDR_ANY (default parameter) means server will listen to all pending
 * addresses)
 *
 * @param ip
 * @return true if succeed to set listen ip
 * @return false if failed to set listen ip
 */
bool Server::ServerModel::set_listen_ip(in_addr_t ip) {
  debug_i_console("Server listen ip set to ", ip);
  std::string debugMes = "Set listen ip to " + (ip + '0');
  // Debug() << std::string{debugMes};
  server_addr->sin_addr.s_addr = htons(ip);
  this->listen_ip = ip;
  return true;
}

/**
 * @brief bind socket file descriptor to server address
 *
 * @param sockfd
 * @return On success, zero is returned.  On error, -1 is returned, and
       errno is set to indicate the error.
 */
int Server::ServerModel::bind_client_socket(int sockfd) {
  /*for more info see https://man7.org/linux/man-pages/man2/bind.2.html*/
  return bind(sockfd, reinterpret_cast<sockaddr*>(this->server_addr.get()),
              sizeof(*server_addr));
}

/**
 * @brief Prepare to accept connections on socket FD.
N connection requests will be queued before further requests are refused.
Returns 0 on success, -1 for errors.
 *
 * @param sockfd
 * @param pending_connection_quantity
 * @return Returns 0 on success, -1 for errors.
 */
int Server::ServerModel::listen_to_socket(
    int sockfd, uint32_t pending_connection_quantity) {
  return listen(sockfd, pending_connection_quantity);
}

/**
 * @brief Await a connection on socket FD.
When a connection arrives, open a new socket to communicate with it,
set *ADDR (which is *ADDR_LEN bytes long) to the address of the connecting
peer and *ADDR_LEN to the address's actual length. r
 *
 * @param sockfd
* @return   Returns  the
            new socket's descriptor, or -1 for errors
 */
int Server::ServerModel::accept_connection_from_socket(int sockfd) {
  socklen_t size = sizeof(*this->server_addr);
  return accept(sockfd, reinterpret_cast<sockaddr*>(this->server_addr.get()),
                &size);
}

/**
 * @brief Output common server configs to log
 * using Debug module
 *
 */
void Server::ServerModel::dump_server_state(void) const noexcept {
  debug_i_console("Server listen address : ", server_addr->sin_addr.s_addr);
  debug_i_console("Server listen port : ", server_addr->sin_port, "(", listen_port,")");
  debug_i_console("Server protocol family : ", server_addr->sin_family);
  return;
}

/**
 * @brief Returns set upped server address struct
 *
 * @return struct sockaddr_in*
 */
struct sockaddr_in* Server::ServerModel::get_server_addr() const {
  return this->server_addr.get();
}

/**
 * @brief Call corresponding function based on retrieved message
 * construction (Json template model)
 *
 * @param socket : connection descriptor
 * @param response : returved message in char array
 * @return 0 retrieved from handler function in case of success
 * otherwise return 1
 */
int Server::ServerModel::distribute_incoming_connections(int socket,
                                                         char* response) {
  MessageModel message(std::string{response});
  std::string response_s = message.get<std::string>("command");
  auto mem_function = (*m_handler.get()).get_command(response_s);
  //debug_w_console((mem_function ? "IS VALID FUNCTION" : "FUNCTION IS INVALID"));
  if (!mem_function) {
    return UNKNOWN_COMMAND_ERROR;
  }
  int res = ((*m_handler.get()).*mem_function)(socket, message);
  return res;
}

/**
 * @brief Main function which listens to open port
 * and distributes connections between according handler functions
 *
 * @note This function uses multithread mechanism and forks
 * another thread for each incoming connections
 *
 * @param connection
 */
void Server::ServerModel::handle_connection(int connection) {
  if (connection < 0) {
    //debug_f_console("Failed to grab connection. errno: ", errno, ", terminating...");
    exit(EXIT_FAILURE);
  }

  // Read from the connection
  char buffer[MAX_JSON_MESSAGE_SIZE];
  int bytes;  // for bytes written and size of buffer

  do {
    bytes = read(connection, buffer, sizeof(buffer) - 1);
    buffer[bytes] = 0x00;
    debug_i_console("Recieved message : ", buffer);
    //debug_i_console("Received message len : ", bytes);
    if (bytes < 0)
    {
      continue;
    }
    int distribute_result = distribute_incoming_connections(connection, buffer);
    if (distribute_result == (TERMINATE_CODE_SUCCESS | TERMINATE_CODE_FAILURE)) {
      return;
    }    // Send a message to the connection
    memset(buffer,0,bytes-1);
  } while (bytes>0);

  close(connection);
  // Close the connections
}

/**
 * @brief Destroy the Server:: Server Model:: Server Model object
 *
 */
Server::ServerModel::~ServerModel() {
  debug_i_console("Destructed Server Model object");
}

/**
 * create socket
 * bind with ip and port
 * listen socket to port
 * accept messages
 *
 */