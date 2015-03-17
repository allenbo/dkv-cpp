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

Status ListLRU::place(Item* item) {
  link(item);
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
    CHECK_NE(tail_, head_);
    tail_ = tail_->prev;
    tail_->next = nullptr;
  } else {
    CHECK_EQ(tail_, head_);
    tail_ = head_ = nullptr;
  }
  return r;
}

Status ListLRU::link(Item* item) {
  if (head_ == nullptr) {
    CHECK_NULL(tail_);
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
    CHECK_NE(item, tail_);
    item->next->prev = item->prev;
  } else {
    // tail
    CHECK_EQ(item, tail_);
    tail_ = item->prev;
  }

  if (item->prev) {
    // not head
    CHECK_NE(item, head_);
    item->prev->next = item->next;
  } else {
    // head
    CHECK_EQ(item, head_);
    head_ = item->next;
  }
  item->prev = item->next = nullptr;
  return Status();
}

}
