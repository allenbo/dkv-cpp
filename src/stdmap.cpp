#include "dkv/stdmap.hpp"

namespace dkv {


Stdmap::Stdmap() {
}

Stdmap::~Stdmap() {
}

Item* Stdmap::get(char* key, uint16_t key_len) {
  std::string str_key(key, key_len);
  return table_[str_key];
}

Item* Stdmap::del(char* key, uint16_t key_len) {
  std::string str_key(key, key_len);
  Item* r = table_[str_key];
  if (r == nullptr) {
    return r;
  }
  table_.erase(str_key);
  return r;
}

Status Stdmap::set(char* key, uint16_t key_len, char* value, uint16_t val_len) {
  std::string str_key(key, key_len);
  if (table_.count(str_key) != 0) {
    return Status(Status::Code::MAP_KEY_DUPLICATE);
  }

  Item* v = new Item(key, key_len, value, val_len);
  table_[str_key] = v;
  return Status();
}

}
