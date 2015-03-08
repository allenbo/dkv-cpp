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

    Item* get(char* key, uint16_t key_len);
    Item* del(char* key, uint16_t key_len);
    Status set(char* key, uint16_t key_len, char* value, uint16_t val_len);

  private:
    std::unordered_map<std::string, Item*> table_;
};

}

#endif
