#include "connection_handler.h"

#include "constants.h"
#include "debug_helper.h"

using namespace Net;

/**
 * @brief Construct a new Basic Connection Handler:: Basic Connection Handler
 * object
 *
 */
BasicConnectionHandler::BasicConnectionHandler() {
  debug_i_console("Created BasicConnectionHandler object at ", this);
  this->server_addr = std::make_unique<struct sockaddr_in>();
}

/**
 * @brief Destroy the Basic Connection Handler:: Basic Connect ion Handler
 * object
 *
 */
BasicConnectionHandler::~BasicConnectionHandler() {
  debug_i_console("Destructed BasicConnectionHandler object at  ", this);
}

/**
 * @brief returns main socket file descriptor
 *
 * @return int which represents sockfd in fd table
 */
int BasicConnectionHandler::get_sockfd(void) const noexcept {
  return (*this).m_connection;
}

/**
 * @brief returns basic representation of network unit
 * for more info see
 * https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
 *
 * @return struct sockaddr_in*
 */
struct sockaddr_in* BasicConnectionHandler::get_sockaddr(void) const noexcept {
  return (*this).server_addr.get();
}

/**
 * @brief Setup socket for communicating
 *
 * @param __domain  : The domain to which future connection will be requested
 * @param __type    : Connection type
 * @param __protocol : Protocol using for communication
 * @return True  on socket creating success.
            False on socket creating failure
 */
bool BasicConnectionHandler::setup_socket(int __domain, int __type,
                                          int __protocol) {
  debug_i_console("Called BasicConnectionHandler::setup_socket(...) function");
  m_connection = socket(__domain, __type, __protocol);
  return (m_connection == -1 ? false : true);
}

/**
 * @brief Setup server addr object
 *
 * @param __family : Protocol family for instance
 * @param __host   : Recv and Send Host address
 * @param __port   : Port on which connection will be released
 * @return true on  setup success false on failure
 */
bool BasicConnectionHandler::setup_server_addr(int __family, std::string __host,
                                               uint32_t __port) {
  debug_i_console(
      "Called BasicConnectionHandler::setup_server_addr(...) function");

  this->server_addr->sin_family = __family;
  debug_i_console("Ended BasicConnectionHandler::setup_server_addr(...) function");
  this->server_addr->sin_addr.s_addr = inet_addr(__host.c_str());
  this->server_addr->sin_port = htons(__port);

  if (this->server_addr) {
    return true;
  }
  return false;
}

/**
 * @brief Sets basic server model to new instance
 *
 * @param new_serv_addr
 * @return true
 * @return false
 */
bool BasicConnectionHandler::set_server_addr(
    struct sockaddr_in* new_serv_addr) {
  debug_i_console("Called BasicConnectionHandler::set_server_addr(...) function");

  this->server_addr = std::make_unique<struct sockaddr_in>(*new_serv_addr);
  return (bool)this->server_addr;
}

/**
 * @brief Connects client model to certain host
 *
 * @param basic_CH : Basic implementation of client model
 * @return true on connect success false on failure
 */
bool Net::connect_to_host(const BasicConnectionHandler& basic_CH) {
  debug_i_console("Called BasicConnectionHandler::connect_to_host(...) function");

  int sockfd = basic_CH.get_sockfd();

  sockaddr_in_unq_ptr sockaddr =
      std::make_unique<struct sockaddr_in>(*basic_CH.get_sockaddr());
  debug_i_console(sockaddr->sin_addr.s_addr, sockaddr->sin_family,
               sockaddr->sin_port);

  int result = connect(sockfd, (SA*)(sockaddr.get()), sizeof(*sockaddr));
  return (result == -1 ? false : true);
}