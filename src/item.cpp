#include "dkv/item.hpp"
#include <string.h>

namespace dkv {

Item::Item(const char* key, int klen, const char* value, int vlen)
    : prev(nullptr), next(nullptr) {
  update(key, klen, value, vlen);
}
Item::~Item() {
  if (data_) {
    delete []data_;
    data_ = key_ = value_ = nullptr;
    key_size_ = value_size_ = item_size_ = 0;
  }
}

bool Item::compare_key(const char* key, uint16_t key_len) {
  return key_len == key_size_ && strncmp(key, key_, key_size_) == 0;
}

bool Item::compare_key(const std::string& key) {
  return compare_key(key.c_str(), (uint16_t)key.size());
}

void Item::update_key(const char* key, int klen) {
  if (key_size_ == klen) {
    memcpy(key_, key, klen);
    return;
  }

  key_size_ = klen;
  item_size_ += key_size_ + value_size_ + sizeof(Item);

  char* tmp = new char[key_size_ + value_size_];

  key_ = tmp;

  memcpy(key_, key, key_size_);
  memcpy(tmp + key_size_, value_, value_size_);
  value_ = tmp + key_size_;

  delete data_;
  data_ = tmp;
}

void Item::update_value(const char* value, int vlen) {
  if (value_size_ == vlen) {
    memcpy(value_, value, vlen);
    return;
  }

  value_size_ = vlen;
  item_size_ = key_size_ + value_size_ + sizeof(Item);

  char* tmp = new char[key_size_ + value_size_];

  key_ = tmp;
  value_ = tmp + key_size_;

  memcpy(key_, data_, key_size_);
  memcpy(value_, value, value_size_);

  delete data_;
  data_ = tmp;
}

void Item::update(const char* key, int klen, const char* value, int vlen) {
  key_size_ = klen;
  value_size_ = vlen;
  item_size_ = key_size_ + value_size_ + sizeof(Item);

  data_ = new char[key_size_ + value_size_];

  key_ = data_;
  value_ = data_ + key_size_;
  
  memcpy(key_, key, key_size_);
  memcpy(value_, value, value_size_);
}

}
