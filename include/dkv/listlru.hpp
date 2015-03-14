#ifndef __DKV_LISTLRU_HPP__
#define __DKV_LISTLRU_HPP__

#include "dkv/item.hpp"
#include "dkv/status.hpp"

namespace dkv {

class ListLRU {
  public:
    static const int64_t MEMORY_SIZE = 1 * 1024 * 1024 * 1024; // 2 GB

    ListLRU();

    ~ListLRU();

    bool check(int klen, int vlen);
    Status update(Item* item);
    Item* discard();
    Status place(Item* item);

    Status link(Item* item);
    Status unlink(Item* item);

  private:
    Item    *head_;
    Item    *tail_;
    int64_t curr_mem_size_;

};

}

#endif
