#ifndef __DKV_HASHTABLE_HPP__
#define __DKV_HASHTABLE_HPP__

#include "dkv/item.hpp"
#include "dkv/status.hpp"

namespace dkv {

class Hashtable {
  public:
    virtual ~Hashtable() {};

    /* @return pointer to item if the key exists */
    virtual Item* get(const char* key, uint16_t key_len) = 0;
    /* @return pointer to item if the key exists */
    virtual Item* del(const char* key, uint16_t key_len) = 0;
    /* @return ok if set operation completes */
    virtual Status set(const char* key, uint16_t key_len, Item* item) = 0;
};

}

#endif
