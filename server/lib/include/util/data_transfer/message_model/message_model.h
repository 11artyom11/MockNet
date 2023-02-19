/**
 * @file message_model.h
 * @author Artyom Grigorian (grigorianartyom1@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-03-20
 * @section DESCRIPTION
 * This class is made to provide high level visualization of
 * Data Transfer.
 * Mainly defines API's essential to format message into different
 * data standards such as JSON.
 * Is defined in DataTransfer namespace
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __MESSAGE_MODEL_H__
#define __MESSAGE_MODEL_H__

#include <string>

#include "../../../../../3rdparty/json/single_include/nlohmann/json.hpp"
#include "../../../helpers/debug_helper/debug_helper.h"

using json = nlohmann::json;
using string = std::string;

namespace DataTransfer {
class MessageModel {
 public:
  MessageModel();
  explicit MessageModel(const string& json_s) __THROW;

  MessageModel(MessageModel&& other_) = delete;

  ~MessageModel() = default;

  template <typename ValType>
  void get_into(const string& key, ValType& into) const;

  template <typename ValType>
  ValType get(const string& key) const;

  template <typename ValType>
  void set(const string& key, const ValType& value);

  json* const get_json_instance(void) noexcept;
  string to_str(void) const noexcept;

 protected:
  json m_message;
};

template <typename ValType>
ValType MessageModel::get(const string& key) const {
  return this->m_message.at(key).get<ValType>();
}

/**
 * @brief get value from json object of message
 *
 * @tparam ValType
 * @param key
 * @return ValType
 */
template <typename ValType>
void MessageModel::get_into(const string& key, ValType& into) const {
  into = m_message[key].get<ValType>();
}

/**
 * @brief set value for the certain key for json object of message
 *
 * @tparam ValType
 * @param key
 * @param value
 */
template <typename ValType>
void MessageModel::set(const string& key, const ValType& value) {
  // debug_i_console("In void MessageModel::set ");
  this->m_message[key] = value;
  // debug_i_console("Out void MessageModel::set ");
}

};      // namespace DataTransfer
#endif  //__MESSAGE_MODEL_H__
