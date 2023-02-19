#include "handler.h"

using Handler = Client::Handler;

Handler::Handler()
{

}

void Handler::commap_init(void) {
  commap[CONNECT_REQUEST] = &Handler::send_connect_request;
  commap[LOG_IN_REQUEST] = &Handler::send_login_request;
  commap[CONNECT_ACCEPT] = &Handler::on_connect_accept_recieved;
  commap[LOG_IN_ACCEPT] = &Handler::on_login_accept_recieved;
  commap[CONNECT_COMMAND] = &Handler::send_connect_command;
  commap[LOG_IN_COMMAND] = &Handler::send_login_command;
  commap[SIGN_UP_COMMAND] = &Handler::send_sign_up_command;
  commap[EXIT_COMMAND] = &Handler::send_terminate_connection;
  commap[CONNECT_VERIFY] = &Handler::on_connect_verify_recieved;
  commap[LOG_IN_VERIFY] = &Handler::on_login_verify_recieved;
  commap[SIGN_UP_VERIFY] = &Handler::on_sign_up_verify_recieved;
  commap[BRDCST_MESSAGE_COMMAND] = &Handler::on_broadcast_message_recieved;
  commap[CHATROOM_CREATE_VERIFY] = &Handler::on_chatroom_create_verified;
  commap[CHATROOM_JOINED_CUSTOMER] = &Handler::on_new_customer_joined_room;
}

void Handler::input_commap_init(void){
  input_commap[CREATE_CHATROOM] = "{\"command\":\"com_crt_chatroom\"}";
  /* .... */
}

int Handler::send_login_request(int sfd, const DataTransfer::MessageModel&) {}

int Handler::send_connect_request(int sfd, const DataTransfer::MessageModel&) {
  this->current_state = CONNECT_STATE::conn_request;
  DataTransfer::ConnectRequest cR{"127.0.0.1"};
  string to_send = cR.to_str();
  debug_i_console("Here is it");
  try {
    int error_code;
    socklen_t error_code_size = sizeof(error_code);
    int retval = getsockopt(sfd, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
    if (error_code != 0) {
      /* socket has a non zero error status */
      fprintf(stderr, "socket error: %s\n", strerror(error_code));
    }
    int res = send(sfd, (char*)to_send.c_str(), to_send.length(), NULL);
    debug_i_console("result :", res);
    return res;

  } catch (...) {

  }
  return -1;
}

int Handler::on_connect_accept_recieved(int sfd, const DataTransfer::MessageModel& message) {
  debug_i_console("in on_connect_accept_recieved");
  /*
  Recieve RSA public key from server, cipher by it aes token and send
  ecncrypted token to server
  */
  this->current_state = CONNECT_STATE::conn_accept;

  send_connect_command(sfd, message);

  return 0;
}

int Handler::on_login_accept_recieved(int sfd,
                                      const DataTransfer::MessageModel&) {}

int Handler::send_connect_command(int sfd,
                                  const DataTransfer::MessageModel& message) {
  debug_i_console("In send_connect_command");
  debug_i_console("Message recieved  : ", message.to_str());

  auto utoken = message.get<string>("unique_token");
  cP.set_unique_token(utoken);

  DataTransfer::ConnectCommand cC{"localhost", utoken};

  string raw_str = cC.to_str();  
                    //"{\"command\":\"com_connect\", \"ip\":\"127.0.0.1\",
                    //\"aes_token\":\"1234556789\",
                    //\"unique_token\":\""+cP.unique_token+"\"}";
  send(sfd, (char*)raw_str.c_str(), strlen((char*)raw_str.c_str()), NULL);
  return 0;
}

int Handler::send_login_command(int sfd, const DataTransfer::MessageModel&) {}

int Handler::send_sign_up_command(int sfd, const DataTransfer::MessageModel&) {}

int Handler::on_connect_verify_recieved(
    int sfd, const DataTransfer::MessageModel& message) {
  std::string token_fetched = message.get<std::string>("unique_token");
  if (!cP.get_unique_token().compare(token_fetched)) {
    debug_i_console("CONNECTION VERIFIED");
    this->current_state = CONNECT_STATE::conn_verify;
  } else {
    debug_f_console("Tokens do not match");
  }

  return 0;
}

int Handler::on_sign_up_verify_recieved(int sfd,
                                        const DataTransfer::MessageModel&) {}

int Handler::on_login_verify_recieved(int sfd,
                                      const DataTransfer::MessageModel&) {}

int Handler::send_terminate_connection(int sfd,
                                       const DataTransfer::MessageModel& msg) {}

int Handler::on_broadcast_message_recieved(
    int sfd, const DataTransfer::MessageModel& message) {
  auto only_message = message.get<std::string>("message");
  auto trig_token = message.get<std::string>("trg_token");
  auto room_id = message.get<std::string>("room_id");
  auto trig_nickname = message.get<std::string>("name");
  debug_r_console(trig_nickname, " : ", only_message);
  return 0;
}

int Handler::on_chatroom_create_verified(int sfd, const DataTransfer::MessageModel& message)
{
  debug_i_console("Chatroom created successfully");
  ChatRoom new_room(cP);
  if (!message.get<std::string>("owner_id").compare(cP.get_unique_token()))
  {
    std::cout << "Dear Master Chatroom Created\n";
    cP.register_master_chatroom(new_room.get_room_id(), new_room);
  } else {
    //this case normally wont be executed
    debug_i_console("Master id does not match ");
    return 0;
  }

  return 0;
}

int Handler::on_new_customer_joined_room (int sfd, const DataTransfer::MessageModel& message)
{
  // std::cout << message.to_str() << "\n";
  std::string new_customer_id = message.get<std::string>("cust_id");
  std::string owner_id = message.get<std::string>("owner_id");
  std::string room_id =  message.get<std::string>("room_id");

  // std::cout << cP.get_unique_token() << " " << owner_id << !cP.get_unique_token().compare(owner_id) << "\n";
  // std::cout << cP.get_unique_token() << " " << new_customer_id << !cP.get_unique_token().compare(new_customer_id) << "\n";

  //If this client is master of that room
  if (!cP.get_unique_token().compare(owner_id))
  {
    std::cout << "New customer " << new_customer_id << " joined "<< room_id << " room (you are master in this room)\n";
  } 
  //if new customer is this client
  else if (!cP.get_unique_token().compare(new_customer_id))
  {
    std::cout << "You have just joined "<< room_id << " room (you are customer in this room)\n";
  }
  //if this client is old customer of that room 
  else {
    std::cout << "New customer " << new_customer_id << " joined "<< room_id << " room (you are customer in this room)\n";
  }
	return 0;
}

CONNECT_STATE Handler::get_net_state(void) const { return this->current_state; }

ClientPrototype const* Handler::get_client_prototype_ptr_c(void) const {
  return &(this->cP);
}

decltype(&Client::Handler::send_connect_request) Handler::get_command(
    std::string command) {
  try {
    debug_i_console("SIZE : ", commap.size());
    return commap.at(command);
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return nullptr;
  }
}

/**
 * This function processes the messages that need interception in core, such as unique token and etc
 * and returns final message which is to be sent.
*/
std::string Handler::get_input_command(std::vector<std::string> com_buffer){
  std::string key_code = com_buffer[0];
  if (key_code == JOIN_CHATROOM_COMMAND)
  {
	  // TODO replace with function, b/c further messages may be very complex'
	  return "{\"command\":\""+ std::string(JOIN_CHATROOM_COMMAND)+"\",\"master_token\":\""+com_buffer[2]+"\",\"room_id\":\""+com_buffer[1]+"\",\"utoken\":\""+cP.get_unique_token()+"\"}";
  }
  try 
  {
    return input_commap[key_code];
  } 
  catch (const std::exception& ex)
  {
    debug_f_console("No Key code found", ex.what());
    return "NULL";
  }
  
}