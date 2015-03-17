#include "dkv/cache.hpp"
#include "dkv/stdmap.hpp"

namespace dkv {

Cache::Cache()
    : curr_mem_size_(0) {
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

  int suc = 0;

  switch(req.type_) {
    case ReqType::REQ_GET:
      r = table_->get(req.get.key, req.get.key_len);
      if (r == nullptr) {
        return Status(Status::Code::MAP_KEY_MISSING);
      }
      
      lru_->update(r);

      res.writeString(r->value(), r->value_size());
      return Status();

    case ReqType::REQ_SET:
      r = table_->get(req.set.key, req.set.key_len);
      if (r != nullptr) {
        return Status(Status::Code::MAP_KEY_DUPLICATE);
      }

      while (req.set.key_len + req.set.val_len + sizeof(Item) + curr_mem_size_ > MAX_MEMORY) {
        r = lru_->discard();
        table_->del(r->key(), r->key_size());
        curr_mem_size_ -= r->size();
        delete r;
      }

      r = new Item(req.set.key, req.set.key_len,
                   req.set.value, req.set.val_len);
      table_->set(req.set.key, req.set.key_len, r);
      lru_->place(r); 
      curr_mem_size_ += r->size();
      return Status();

    case ReqType::REQ_DEL:
      r = table_->del(req.del.key, req.del.key_len);
      if (r == nullptr) {
        return Status(Status::Code::MAP_KEY_MISSING);
      }

      st = lru_->unlink(r);
      curr_mem_size_ -= r->size();
      delete r;
      return Status();

    case ReqType::REQ_UPDATE:
      r = table_->get(req.update.key, req.update.key_len);
      if (r == nullptr) {
        return Status(Status::Code::MAP_KEY_MISSING);
      }

      while (req.update.val_len -  r->value_size() + curr_mem_size_ > MAX_MEMORY) {
        Item* p = lru_->discard();
        table_->del(p->key(), r->key_size());
        curr_mem_size_ -= p->size();
        delete p;
      }

      curr_mem_size_ += req.update.val_len - r->value_size();
      r->update_value(req.update.value, req.update.val_len);
      lru_->update(r);
      return Status();

    case ReqType::REQ_GETS:
      // return how many operations are successfully performed
      for (int i = 0; i < (int)req.gets.n; i ++) {
        char* key = req.gets.keys[i];
        uint16_t key_len = req.gets.key_lens[i];
        r = table_->get(key,key_len);
          if (r != nullptr) {
            lru_->update(r);
            res.writeString(key, key_len);
            res.writeString(r->value(), r->value_size());
            suc ++;
          }
      }
      return Status();

    case ReqType::REQ_SETS:
      for(int i = 0; i < (int)req.sets.n; i ++) {
        char* key = req.sets.keys[i];
        uint16_t key_len = req.sets.key_lens[i];

        r = table_->get(key, key_len);
        if (r != nullptr) {
          continue;
        }

        char* value = req.sets.values[i];
        uint16_t val_len = req.sets.val_lens[i];

        while (key_len + val_len + sizeof(Item) + curr_mem_size_ > MAX_MEMORY) {
          r = lru_->discard();
          table_->del(r->key(), r->key_size());
          curr_mem_size_ -= r->size();
          delete r;
        }

        r = new Item(key, key_len, value, val_len);
        table_->set(key, key_len, r);
        lru_->place(r);
        curr_mem_size_ += r->size();
        // if succeed, return key
        res.writeString(key, key_len);
      }

    case ReqType::REQ_DELS:
      for(int i = 0; i < (int)req.dels.n; i ++ ) {
        char* key = req.dels.keys[i];
        uint16_t key_len = req.dels.key_lens[i];

        r = table_->del(key, key_len);
        if (r) {
          st = lru_->unlink(r);
          curr_mem_size_ -= r->size();
          delete r;
          res.writeString(key, key_len);
        }
      }
      return Status(); 

    case ReqType::REQ_UPDATES:
      for(int i = 0; i < (int)req.updates.n; i ++ ) {
        char* key = req.updates.keys[i];
        uint16_t key_len = req.updates.key_lens[i];

        r = table_->get(key, key_len);
        
        if(r) {
          char* value = req.updates.values[i];
          uint16_t val_len = req.updates.val_lens[i];
          while (val_len - r->value_size() + curr_mem_size_ > MAX_MEMORY) {
            Item* p = lru_->discard();
            table_->del(key, key_len);
            curr_mem_size_ -= p->size();
            delete p;
          }

          curr_mem_size_ += val_len - r->value_size();
          r->update_value(value, val_len);
          lru_->update(r);
          res.writeString(key, key_len);
        }
      }
      return Status();
  }
  return Status(Status::Code::PROTO_ERROR);
}

}
