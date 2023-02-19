#include "client.h"

using namespace Client;
using namespace Net;

/**
 * @brief Construct a new Client Model:: Client Model object
 *
 * @param __port Port number to set connection
 */
ClientModel::ClientModel() {
  debug_i_console("Created uninitialized client");
  con_handler = std::make_shared<Net::BasicConnectionHandler>();
  comm_unit = std::make_shared<Net::BasicCommunicationModel>();
  debug_w_console(comm_unit);
}



void ClientModel::init_new_client(unsigned int __port, const std::string& host) {
  port = __port;
  bool sock_res = con_handler->setup_socket(AF_INET, SOCK_STREAM, 0);
  bool serv_res = con_handler->setup_server_addr(AF_INET, host, port);
  main_socket = con_handler->get_sockfd();

  if (!(sock_res & serv_res)) {
    debug_f_console("Failed to create socket. terminating...");
    connection_state = 1;
  }
  int connect_result = connect_to_host(*con_handler.get());
  if (!connect_result) {
    debug_f_console("Failed to connect to host, ... terminating...");
    connection_state = 1;
  }

  connection_state = 0;
  auto connect_th = std::async(std::launch::async, 
                                  &Client::ClientModel::connect_to_default_host, this);

  auto read_th = std::async(std::launch::async,
                            &Net::BasicCommunicationModel::start_read_async,
                            this->comm_unit, main_socket);
  // auto write_th = std::async(std::launch::async,
  //                           &Net::BasicCommunicationModel::start_write_async,
  //                           this->comm_unit, main_socket, std::ref(std::cin));
  return;

}

/**
 * @brief Connect to default setted up host
 *
 * @return int result of connection trial
 */
int ClientModel::connect_to_default_host(void) {
  return comm_unit->get_io_model()->get_handler()->send_connect_request(con_handler->get_sockfd(), DataTransfer::MessageModel{"{}"});
}

int ClientModel::get_connection_state() const noexcept {
  return this->connection_state;
}

BasicConnectionHandlerSharedPtr ClientModel::get_con_handler() const noexcept {
  return this->con_handler;
}

BasicCommunicationModelSharedPtr ClientModel::get_comm_unit() const noexcept {
  return this->comm_unit;
}

int ClientModel::send_to_host(const DataTransfer::MessageModel& message) {
  debug_i_console("SOCKET : ", main_socket);
  comm_unit->get_io_model()->send_message(message, con_handler->get_sockfd());
  return 0;
}

int ClientModel::close_connection() {
  // auto utoken =
  // get_comm_unit()->get_io_model()->get_handler()->get_client_prototype_ptr_c()->unique_token;
  // auto termCommand = DataTransfer::TerminateConnectionCommand(utoken);
  // get_comm_unit()->get_io_model()->send_message(termCommand, main_socket);
  // return 0;
}

int ClientModel::read_commands (const std::vector<std::string>& commands){
  //check if command is to connect to host 
  if(commands.front() == CONFIG_CONNECTION){
      std::thread th = std::thread([this,&commands]
      {
        int port = stoi(commands[2]);
        this->init_new_client(port, commands[1]);
        return 0;
    });
    std::cout << "Joining  establishing ....\n";
    th.detach();
    std::cout << "Done  establishing ....\n";
    return 0;
  }

  std::string command_str = comm_unit->get_io_model()->get_handler()->get_input_command(commands);
  comm_unit->start_write_async(con_handler->get_sockfd(), command_str);
  return 0;
}

/**
 * @brief Destroy the Client Model:: Client Model object
 *
 */
ClientModel::~ClientModel() { debug_i_console("Desctructed Client Model"); }
