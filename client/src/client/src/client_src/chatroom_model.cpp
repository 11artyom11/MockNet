#include "chatroom_model.h"

std::string ClientPrototype::get_unique_token(void) const
{
  return this->unique_token;
}
      
void ClientPrototype::set_unique_token(const std::string& unique_token)
{
  this->unique_token = unique_token;
}


ChatRoom* ClientPrototype::get_master_room(const std::string& unique_token) const
{
  return as_master_chatrooms.at(unique_token);
}

ChatRoom* ClientPrototype::get_customer_room(const std::string& unique_token) const
{
  return as_customer_chatrooms.at(unique_token);
}

void ClientPrototype::register_customer_chatroom(const std::string& unique_token, const ChatRoom& new_room)
{
  chatrooms.push_back(new_room);
  as_customer_chatrooms[unique_token] = const_cast<ChatRoom*>(&new_room);
  return;
}

void ClientPrototype::register_master_chatroom(const std::string& unique_token, const ChatRoom& new_room)
{
  chatrooms.push_back(new_room);
  as_master_chatrooms[unique_token] = const_cast<ChatRoom*>(&new_room);
  return;
}

//*******//

/**
 * @brief Construct a new Chat Room:: Chat Room object
 *
 * @param master The master client who is the owner of chatroom
 */
ChatRoom::ChatRoom(const ClientPrototype& master): master_client{std::make_shared<ClientPrototype>(master)}{
  
}


/**
 * @brief Return pointer on constant instance of master of ChatRoom
 *
 * @return ClientPrototype* const
 */
ClientPrototype* const ChatRoom::get_master(void) const noexcept {
  return master_client.get();
}

/**
 * @brief Return room_id of *this
 *
 * @return std::string
 */
std::string ChatRoom::get_room_id(void) const noexcept { return this->room_id; }

/**
 * @brief Send recieved from participant of chatroom message to other clients
 * joined this chatroom
 *
 * @param utoken trigger client utoken
 * @param message json recieved from trigger client
 * @return count of clients to whom this message has been sent
 */
int ChatRoom::broadcast_to_all_users(const std::string& utoken,
                                      const DataTransfer::MessageModel& message) const {
  
}

/**
 * @brief Print current state of chatroom e.g RoomID MasterUtoken etc
 *
 */
void ChatRoom::dump_state(void) const noexcept {
  debug_r_console("+=========+===================+");
  debug_r_console("|   Room ID    | ", room_id, " |");
  debug_r_console("+==============+==============+");
  debug_r_console("|   Master     | ", master_client->get_unique_token(), " |");
  debug_r_console("+==============+==============+");
  int idx = 0;
  for (const auto& sec_client : secondary_clients) {
    debug_r_console("| Client ", idx++, " | ", sec_client->get_unique_token(),
                " |");
    debug_r_console("+--------------+--------------+");
  }
  debug_r_console("| Whole Client Count : ", idx + 1, " |");
  debug_r_console("+--------------+--------------+");
}

/**
 *
 * @brief Destroy the Chat Room:: Chat Room object
 *
 */
ChatRoom::~ChatRoom() {}