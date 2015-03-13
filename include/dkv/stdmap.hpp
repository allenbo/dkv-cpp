#ifndef __DKV_STDMAP_HPP__
#define __DKV_STDMAP_HPP__

#include "dkv/hashtable.hpp"
#include <unordered_map>

#include <map>

namespace dkv {

class Stdmap : public  Hashtable {
  public:
    Stdmap();
    ~Stdmap();

    Item* get(const char* key, uint16_t key_len);
    Item* del(const char* key, uint16_t key_len);
    Status set(const char* key, uint16_t key_len, Item* item);

  private:
    std::unordered_map<std::string, Item*> table_;
};

}

#endif
