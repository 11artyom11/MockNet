/**
 * @file io_unit.h
 * @author Artyom Grigorian (grigorianartyom1@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-03-20
 * @section DESCRIPTION
 * This IOModel class is made to handle client input and output
 * and provide multiprocess mechanism for this process
 * Is defined in iounit namespace
 *
 * @note In client side handler is on one level deeper
 * while on server side it is directly binded to server instance
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef __IO_UNIT_H__
#define __IO_UNIT_H__

#include <algorithm>
#include <istream>
#include <memory>
#include <mutex>
#include <ostream>
#include <queue>
#include <thread>
#include <utility>

#include "debug_helper.h"
#include "constants.h"
#include "handler.h"

namespace iounit {
template <typename T>
using shared_ptr = std::shared_ptr<T>;

typedef shared_ptr<Client::Handler> Handler_shrd_ptr;

class IOModel {
 public:
  IOModel();
  ~IOModel();
  template <typename... mesType>
  void write_q(mesType... istreams);

  template <typename mesType>
  void read_q(int sfd, mesType messages);

  Handler_shrd_ptr get_handler(void) const;
  void send_message(const DataTransfer::MessageModel&, int sfd);

 private:
  /*Queues in which we store output and input streams which
  will be released in near future*/
  std::queue<std::thread> wrtie_thread_q;
  std::queue<std::thread> read_thread_q;
  std::mutex write_mutex;
  std::mutex read_mutex;
  /*
  Handler to handle incoming and outgoing
  messages
  */
  Handler_shrd_ptr m_handler;
};
}  // namespace iounit

/*This function works in atomic way*/
template <typename... mesType>
void iounit::IOModel::write_q(mesType... istreams) {
  /*Lock mutex to do atomic write to queue and check in
      empty subject write queue*/
  write_mutex.lock();

  /*From here must be DB manager write functions released*/

  /********************************************/
  /*Think over turning mutexes into semaphores*/
  /********************************************/

  write_mutex.unlock();
}

/*This function workss atomic */
template <typename mesType>
void iounit::IOModel::read_q(int sfd, mesType message) {
  debug_w_console("In read_q function");
  /*Lock mutex to do atomic read to queue and check in
      empty subject read queue*/
  read_mutex.lock();
  debug_i_console("RAW STR : ", message);
  std::string message_str{message};

  /* AES CASE */
  if (m_handler->get_net_state() != CONNECT_STATE::conn_request) {
    std::string message_str = message;

  } else {
    debug_w_console("GOT UNSAFE CASE");
    /* ... */
  }
  DataTransfer::MessageModel mesModel(message_str);
  /*Must be check of retrieved message (..later) FIXXX*/
  /*is_message_valid (message) knd of this*/
  std::string response_s = mesModel.get<decltype(response_s)>("command");

  debug_i_console(response_s, " Retrieved response");
  auto mem_function = (*m_handler.get()).get_command(response_s);
  if (mem_function) {
    int res = ((*m_handler.get()).*mem_function)(sfd, mesModel);
  } else {
    debug_f_console("Handler function does not exist!!!");
  }

  read_mutex.unlock();
}

#endif
