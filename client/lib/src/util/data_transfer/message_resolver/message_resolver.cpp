#include "message_resolver.h"

using MessageResolver = DataTransfer::MessageResolver;
using MessageModel = DataTransfer::MessageModel;

/**
 * @brief Construct a new Message Resolver:: Message Resolver object
 *
 */
MessageResolver::MessageResolver() {}

/**
 * @brief recieve a command which will tell what kind of message template
 * creator will be created
 *
 * @param init_data init data std::map which is filled by key-value
 * needed for message constructing
 * @param command enum type
 * @return MessageModel instance which is filled by passed data
 */
MessageModel MessageResolver::distribute_command(
    const std::map<string, JsonValType>& init_data,
    DataTransfer::COMMAND_NAME command) const {
  /*If passed initial data is ill formed throw corresponding exception*/
  if (!is_message_initial_data_valid(command, init_data)) {
    throw INVALID_INITIAL_MESSAGE_DATA;
  }

  switch (command) {
    case DataTransfer::COMMAND_NAME::SIGN_UP_COMMAND:
      return init_sign_up_message(init_data);

    case DataTransfer::COMMAND_NAME::LOG_IN_COMMAND:
      return init_log_in_message(init_data);

    default:
      throw UNKNOWN_TEMPLATE_MESSAGE_COMMAND;
  }
}

/**
 * @brief create message model by passed map data
 *
 *
 * @param init_data multi-typed map containing all needed data
 * for **LOGIN** message constructing
 * @return MessageModel
 */
MessageModel MessageResolver::init_log_in_message(
    const std::map<string, JsonValType>& init_data) const __THROW {
  MessageModel mes;
}

/**
 * @brief create message model by passed map data
 *
 *
 * @param init_data multi-typed map containing all needed data
 * for **SIGNUP** message constructing
 * @return MessageModel
 */
MessageModel MessageResolver::init_sign_up_message(
    const std::map<string, JsonValType>& init_data) const __THROW {}

/**
 * @brief check if all needed for certain message template data
 * exists in associative container
 *
 * Data is passing in this form
 * +-----------------------+
 * | "command" | "sign_up" |
 * | "username"| "username"|
 * | "passwd"  | "********"|
 * +-----------------------+
 *
 * @return true if all OK
 * @return false if at least one of ESSENTIAL data doesnt exist
 */
bool DataTransfer::is_message_initial_data_valid(
    DataTransfer::COMMAND_NAME cN, const std::map<string, JsonValType>& data) {
  switch (cN) {
    case DataTransfer::COMMAND_NAME::SIGN_UP_COMMAND: {
      for (auto c_key : COMMAND_DATA_KEY_BINDINGS.at(SIGN_UP_COMMAND)) {
        try {
          (void)data.at(c_key);

        } catch (const std::exception& e) {
          debug_f_console("CAUGHT ERROR : ", INVALID_INITIAL_MESSAGE_DATA);
          return false;
        }
      }
      return true;
    }
    case DataTransfer::COMMAND_NAME::LOG_IN_COMMAND: {
      for (auto c_key : COMMAND_DATA_KEY_BINDINGS.at(SIGN_UP_COMMAND)) {
        try {
          (void)data.at(c_key);
        } catch (const std::exception& e) {
          debug_f_console("CAUGHT ERROR : ", INVALID_INITIAL_MESSAGE_DATA);
          return false;
        }
      }
      return true;
    }

    default:
      return false;
  }
}

/**
 * @brief check if all needed for certain message template data
 * exists in MessageModel instance
 *
 * @return true if all OK
 * @return false if at least one of ESSENTIAL data doesnt exist
 */
bool DataTransfer::is_message_valid(MessageModel& mModel) {
  try {
    std::string command = mModel.get<std::string>("command");
    bool is_command_valid = false;
    debug_i_console(std::find(COMMAND_LIST.begin(), COMMAND_LIST.end(), command) !=
                 COMMAND_LIST.end());
    return std::find(COMMAND_LIST.begin(), COMMAND_LIST.end(), command) !=
           COMMAND_LIST.end();

  } catch (const std::exception& e) {
    debug_f_console(e.what());
    debug_f_console("Thrown INVALID_INITIAL_MESSAGE_DATA exception");
    debug_f_console("message_resolver.cpp :151");
    return false;
  }
}

/**
 * @brief Return according command number from enum
 * based on passed string (also command , but as variable)
 *
 * @param command
 * @return DataTransfer::COMMAND_NAME
 */
DataTransfer::COMMAND_NAME DataTransfer::get_command_from_s(
    std::string command) {
  if (command == SIGN_UP_COMMAND) {
    return DataTransfer::COMMAND_NAME::SIGN_UP_COMMAND;
  }
  if (command == LOG_IN_COMMAND) {
    return DataTransfer::COMMAND_NAME::LOG_IN_COMMAND;
  }
  if (command == WRITE_COMMAND) {
    return DataTransfer::COMMAND_NAME::WRITE_COMMAND;
  }
  if (command == EXIT_COMMAND) {
    return DataTransfer::COMMAND_NAME::EXIT_COMMAND;
  }

  if (command == LOG_IN_REQUEST) {
    return DataTransfer::COMMAND_NAME::LOG_IN_REQUEST;
  }

  if (command == SIGN_UP_VERIFY) {
    return DataTransfer::COMMAND_NAME::SIGN_UP_VERIFY;
  }
  if (command == LOG_IN_ACCEPT) {
    return DataTransfer::COMMAND_NAME::LOG_IN_ACCEPT;
  }
  if (command == LOG_IN_VERIFY) {
    return DataTransfer::COMMAND_NAME::LOG_IN_VERIFY;
  }

  if (command == LOG_IN_REQUEST) {
    return DataTransfer::COMMAND_NAME::LOG_IN_REQUEST;
  }

  if (command == SIGN_UP_VERIFY) {
    return DataTransfer::COMMAND_NAME::SIGN_UP_VERIFY;
  }
  if (command == LOG_IN_ACCEPT) {
    return DataTransfer::COMMAND_NAME::LOG_IN_ACCEPT;
  }
  if (command == LOG_IN_VERIFY) {
    return DataTransfer::COMMAND_NAME::LOG_IN_VERIFY;
  }
}

/**
 * @brief Destroy the Data Transfer:: Message Resolver:: Message Resolver object
 *
 */
DataTransfer::MessageResolver::~MessageResolver() {}
