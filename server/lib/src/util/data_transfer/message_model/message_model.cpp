#include "../../../../include/util/data_transfer/message_model/message_model.h"

using json = nlohmann::json;
using string = std::string;
using namespace DataTransfer;

MessageModel::MessageModel() : MessageModel("{}") {
  debug_i_console("Called default c-tor of MessageModel\n Empty object created");
}

/**
 * @brief Construct a new Message Model:: Message Model object
 *
 * @param json_s
 */
MessageModel::MessageModel(const string& json_s) __THROW

{
  try {
    m_message = nlohmann::json(json::parse(json_s));
  } catch (const std::exception& e) {
    //debug_f_console(e.what());
    //debug_f_console("Parse Error !!!");
  }

  debug_i_console("Constructed new MessageModel object");
}

/**
 * @brief return pointer const to json object
 * means that pointed object cannot be edited via returned ptr
 *
 * @return json* const pointer to json object
 */
json* const MessageModel::get_json_instance(void) noexcept {
  json* const js_obj_ptr = (json* const)(&this->m_message);
  return js_obj_ptr;
}

/**
 * @brief Format message model to string and return
 *
 * @return string indroduction of $(*this) message model
 */
string MessageModel::to_str(void) const noexcept {
  return this->m_message.dump();
}
