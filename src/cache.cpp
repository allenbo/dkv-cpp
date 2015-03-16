#include "dkv/cache.hpp"
#include "dkv/stdmap.hpp"

namespace dkv {

Cache::Cache() {
  table_ = new Stdmap();
  lock_  = new common::Mutex();
  lru_   = new ListLRU();
}

Cache::~Cache() {
  delete table_;
  delete lru_;
  delete lock_;
}

Status Cache::process(Request& req, networking::Buffer& res) {
  common::ScopeLock _(lock_);
  Item* r = nullptr;
  Status st;

  switch(req.type_) {
    case ReqType::REQ_GET:
      r = table_->get(req.get.key, req.get.key_len);
      if (r == nullptr) {
        return Status(Status::Code::MAP_KEY_MISSING);
      }
      
      lru_->update(r);

      res.writeString(r->value(), r->value_size());
      return Status();
      break;

    case ReqType::REQ_SET:
      while (!lru_->check(req.set.key_len, req.set.val_len) ) {
        r = lru_->discard();
        table_->del(r->key(), r->key_size());
        delete r;
      }

      r = new Item(req.set.key, req.set.key_len, req.set.value, req.set.val_len);
      st = table_->set(req.set.key, req.set.key_len, r);
      if (!st.ok()) {
        delete r;
        return st;
      }

      st = lru_->place(r); 
      return st;
      break;

    case ReqType::REQ_DEL:
      r = table_->del(req.del.key, req.del.key_len);
      if (r == nullptr) {
        return Status(Status::Code::MAP_KEY_MISSING);
      }

      st = lru_->unlink(r);
      delete r;
      return Status();
      break;
  }
  return Status(Status::Code::PROTO_ERROR);
}

}
