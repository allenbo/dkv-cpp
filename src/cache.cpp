#include "dkv/cache.hpp"
#include "dkv/stdmap.hpp"

namespace dkv {

Cache::Cache() {
  table_ = new Stdmap();
  lock_  = new common::Mutex();
}

Cache::~Cache() {
  delete table_;
  delete lock_;
}

Status Cache::process(Request& req, networking::Buffer& res) {
  common::ScopeLock _(lock_);
  switch(req.type) {
    case ReqType::REQ_GET:
      Item* r = table_.get(req.key, req.key_len);
      if (r == nullptr) {
        return Status(Status::Code::MAP_KEY_MISSING);
      }
      res.writeString(r->value(), r->value_size());
      return Status();
      break;
    case ReqType::REQ_SET:
      return table_.set(req.key, req.key_len, req.value, req.val_len);
      break;
    case ReqType::REQ_DEL:
      Item* r = table_.del(req.key, req.key_len);
      if (r == nullptr) {
        return Status(Status::Code::MAP_KEY_MISSING);
      }
      return Status();
      break;
  }
  return Status(Status::Code::PROTO_ERROR);
}

}
