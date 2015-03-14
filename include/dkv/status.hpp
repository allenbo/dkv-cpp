#ifndef __DKV_STATUS_HPP__
#define __DKV_STATUS_HPP__

#include "networking/buffer.hpp"

namespace dkv {

class Status {
  public:

    enum class Code : char {
      OK,

      HANDLER_EXIST,

      MAP_KEY_DUPLICATE,
      MAP_KEY_MISSING,

      IO_EOF,
      PROTO_ERROR,

      SERIALIZATION_FAILURE,

      NETWORKING_ERROR,

      LRU_DEFICIT_MEMORY,
    };

    Status() : code_(Code::OK) {}
    Status(Code code) : code_(code) {}
    Status(const Status& o) : code_(o.code_) {}

    inline bool ok() { return code_ == Code::OK; }

    Status from_buffer(networking::Buffer& buf);    
    networking::Buffer to_buffer();

  private:

    Code code_;
};

}

#endif
