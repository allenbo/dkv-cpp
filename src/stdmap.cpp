#include "dkv/stdmap.hpp"
#include <iostream>

namespace dkv {


Stdmap::Stdmap() {
}

Stdmap::~Stdmap() {
}

Item* Stdmap::get(const char* key, uint16_t key_len) {
  std::string str_key(key, key_len);
  if (table_.count(str_key) == 0) {
    return nullptr;
  }

  return table_[str_key];
}

Item* Stdmap::del(const char* key, uint16_t key_len) {
  std::string str_key(key, key_len);
  Item* r = table_[str_key];
  if (r == nullptr) {
    return r;
  }
  table_.erase(str_key);
  return r;
}

Status Stdmap::set(const char* key, uint16_t key_len, Item* v) {
  std::string str_key(key, key_len);
  if (table_.count(str_key) != 0) {
    return Status(Status::Code::MAP_KEY_DUPLICATE);
  }

  table_[str_key] = v;
  return Status();
}

}
