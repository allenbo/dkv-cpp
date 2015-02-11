#ifndef __DKV_STATUS_HPP__
#define __DKV_STATUS_HPP__

namespace dkv {

class Status {
  public:

    enum class Code {
      OK,

      HANDLER_EXIST,

      MAP_KEY_DUPLICATE,
      MAP_KEY_MISSING,

      IO_EOF,
      PROTO_ERROR,

      SERIALIZATION_FAILURE,
    };

    Status() : code_(Code::OK) {}
    Status(Code code) : code_(code) {}
    Status(const Status& o) : code_(o.code_) {}

    inline bool ok() { return code_ == Code::OK; }

  private:

    Code code_;
};

}

#endif
