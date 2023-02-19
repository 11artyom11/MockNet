#include "communication_unit.h"

using namespace Net;

/**
 * @brief Construct a new Basic Communication Model:: Basic Communication Model
 * object
 *
 */
BasicCommunicationModel::BasicCommunicationModel()
    : BasicCommunicationModel(max_read_th_count, max_write_th_count) {}

/**
 * @brief Construct a new Basic Communication Model:: Basic Communication Model
 * object
 *
 * @param max_read_thread_count Maximum read thread count that can be forked at
 * once
 * @param max_write_thread_count  Maximum write thread count that can be forked
 * at once
 */
BasicCommunicationModel::BasicCommunicationModel(
    uint32_t _max_read_thread_count, uint32_t _max_write_thread_count) {
  this->m_io_model = std::make_shared<iounit::IOModel>();
  this->max_read_thread_count = _max_read_thread_count;
  this->max_write_thread_count = _max_write_thread_count;
  sem_init(&read_lock, 0, _max_read_thread_count);
  sem_init(&write_lock, 0, _max_write_thread_count);
}

/**
 * @brief Destroy the Basic Communication Model:: Basic Communication Model
 * object
 *
 */
BasicCommunicationModel::~BasicCommunicationModel() {}

/**
 * @brief Starting N count of forked threads which are async-ly
 * reading from socket and sent to function of output
 * [*need to think over*] maybe add std::ostream to pass to io->read function
 * @param sockfd Socket file descriptor
 * @return void*
 */

void BasicCommunicationModel::start_read_async(int sockfd) {
  /*Here must be set message resolver to resolve inputted data*/

  debug_i_console("Called BasicCommunicationModel::start_read_async(..)");
  /*function which is called to forkk another thread in order to
      support async read model and not to mess with write model*/

  for (;;) {
    /*While conenction has not been
    terminated keep reading from sockfd*/

    /*This infinite loop can hold maximum $max_read_thread_count
    connections*/

    //        sem_wait(&read_lock);

    char buff[1024];
    bzero(buff, 1024);

    int read_result = read(sockfd, buff, sizeof(buff));
    if (read_result == 0) {
      debug_f_console("Host is no longer available. Terminating...");
      return;
    }
    /*Start new thread to print retrieved buffer*/
    m_io_model->read_q(sockfd, buff);

    //        sem_post(&read_lock);
  }

  /*Async-ly start read lambda (see :43) */
  debug_i_console("Ended BasicCommunicationModel::start_read_async(..)");
}

IOModel_shrd_ptr BasicCommunicationModel::get_io_model(void) const {
  return this->m_io_model;
}

/**
 * @brief Function to support async writing function
 * don't need to support multi stream work yet. Maybe later feature
 * will be added.
 *
 * P.S. Yes it will be added because input eays will be extending
 * exponentinlly :))
 * @param sockfd
 * @return void*
 */
void BasicCommunicationModel::start_write_async(int sockfd, std::istream& is) {
  std::string buf_s;
  do {
    is >> buf_s;
    DataTransfer::MessageModel model(buf_s);
    if (!DataTransfer::is_message_valid(model)) {
      debug_f_console("Bad Message");
      continue;
    }
    m_io_model->send_message(model, sockfd);
    // write(sockfd, buf_s.c_str(), strlen(buf_s.c_str()));
    // buf_s.clear();
  } while (buf_s.length() && !is.eof());
}


void BasicCommunicationModel::start_write_async(int sockfd, const std::string& command) 
{
    std::cout << "To send : " << command << "\n";
    DataTransfer::MessageModel model(command);
    m_io_model->send_message(model, sockfd);
}
