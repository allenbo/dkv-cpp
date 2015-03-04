#ifndef __DKV_ITEM_HPP__
#define __DKV_ITEM_HPP__

#include "dkv/util.hpp"

namespace dkv {

class Item;

class Item {
  public:
    /* create item with lengthed key and lengthed value */
    Item(const char* key, int klen, const char* value, int vlen);
    ~Item();

    inline uint16_t size() { return item_size_; }
    inline uint16_t key_size() { return key_size_; }
    inline uint16_t value_size() { return value_size_; }

    bool compare_key(const char* key, uint16_t key_len);
    bool compare_key(const std::string& key);

    const char* key() { return key_; }
    const char* value() { return value_; }

    void update_key(const char* key, int klen);
    void update_value(const char* value, int vlen);

    void update(const char* key, int klen, const char* value, int vlen);

  private:
    char     *key_;
    uint16_t key_size_;
    char     *value_;
    uint16_t value_size;

    uint16_t item_size_; // item size = key size + value size + sizof(Item)
    char     *data_;
};

}

#endif
