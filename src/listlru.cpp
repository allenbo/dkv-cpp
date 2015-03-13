#include "dkv/listlru.hpp"
#include "common/all.hpp"
namespace dkv {


ListLRU::ListLRU()
    :head_(nullptr),
     tail_(nullptr),
     curr_mem_size_(0) {
}

ListLRU::~ListLRU() {
}

bool ListLRU::check(int klen, int vlen) {
  return klen + vlen + sizeof(Item) + curr_mem_size_ <= ListLRU::MEMORY_SIZE;
}

Status ListLRU::place(Item* item) {
  link(item);
  curr_mem_size_ += item->size(); 
  return Status();
}

Status ListLRU::update(Item* item) {
  if (head_ == item) {
    // doesn't need to be updated
    return Status();
  }

  unlink(item);
  link(item);
  return Status();
}

Item* ListLRU::discard() {
  CHECK_NOTNULL(tail_) << "Tail has to be not null" << std::endl;

  Item* r = tail_;
  // tail is not the only item
  if (tail_->prev) {
    tail_ = tail_->prev;
    tail_->next = nullptr;
  } else {
    tail_ = head_ = nullptr;
  }
  curr_mem_size_ -= r->size();
  return r;
}

Status ListLRU::link(Item* item) {
  if (head_ == nullptr) {
    head_ = item;
    tail_ = item;
    return Status();
  }
  item->next = head_;
  head_->prev = item;
  head_ = item;
  return Status();
}

Status ListLRU::unlink(Item* item) {
  if (item->next) {
    // not tail
    item->next->prev = item->prev;
  } else {
    // tail
    tail_ = item->prev;
  }

  if (item->prev) {
    // not head
    item->prev->next = item->next;
  } else {
    // head
    head_ = item->next;
  }

  item->prev->next = item->next;
  return Status();
}

}
