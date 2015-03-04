#ifndef __DKV_CACHE_HPP__
#define __DKV_CACHE_HPP__

#include "dkv/status.hpp"
#include "dkv/hashtable.hpp"
#include "dkv/item.hpp"
#include "dkv/request.hpp"
#include "networking/buffer.hpp"
#include "common/all.hpp"

namespace dkv {

class Cache {
  public:
    Cache();
    ~Cache();

    Status process(Request& req, networking::Buffer& buf);

  private:

    HashTable     *table_;
    common::Mutex *lock_;

    // no copy
    Cache(const Cache&);
    Cache& operator=(const Cache&);
};

}

#endif
