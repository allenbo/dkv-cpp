#ifndef __DKV_CACHE_HPP__
#define __DKV_CACHE_HPP__

#include "dkv/status.hpp"
#include "dkv/hashtable.hpp"
#include "dkv/listlru.hpp"
#include "dkv/item.hpp"
#include "dkv/request.hpp"
#include "networking/buffer.hpp"
#include "common/all.hpp"

namespace dkv {

class Cache {
  public:
    static const int64_t MAX_MEMORY = 2 * 1024 * 1024 * 1024; // 2 GB
    Cache();
    ~Cache();

    Status process(Request& req, networking::Buffer& buf);

  private:

    Hashtable     *table_;
    ListLRU       *lru_;
    common::Mutex *lock_;

    int64_t       curr_mem_size_;
    int64_t       nops_;

    // no copy
    Cache(const Cache&);
    Cache& operator=(const Cache&);
};

}

#endif
