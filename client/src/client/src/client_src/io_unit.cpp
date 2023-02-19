#include "io_unit.h"

using namespace iounit;

/**
 * @brief Construct a new IOModel::IOModel object
 *
 */
IOModel::IOModel() {
  this->m_handler = std::make_unique<Client::Handler>();
  m_handler.get()->commap_init();
  m_handler.get()->input_commap_init();
}

Handler_shrd_ptr IOModel::get_handler(void) const { return m_handler; }

void IOModel::send_message(const DataTransfer::MessageModel& message, int sfd) {
  size_t len = message.to_str().length();
  char* message_c = (char*)malloc(len);
  memcpy(message_c, message.to_str().c_str(), len);
  message_c[len] = '\0';
  debug_i_console("To send: ", message_c);
  int res = send(sfd, message_c, strlen(message_c), NULL);
  debug_i_console("RESULT OF SEND : ", res);
  return;
}

/**
 * @brief Destroy the IOModel::IOModel object
 *
 */
IOModel::~IOModel() {}
