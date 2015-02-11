#ifndef __DKV_REQUEST_HPP__
#define __DKV_REQUEST_HPP__

#include "dkv/status.hpp"
#include "networking/buffer.hpp"

namespace dkv {

enum class ReqType : char {
  REQ_UNDEFINED = 0,
  REQ_GET,
  REQ_SET,
  REQ_DEL,
};

struct Request {
  ReqType  type;
  char*    key;
  uint16_t key_len;
  char*    value;
  uint16_t val_len;

  Request();
  ~Request();

  networking::Buffer to_buffer();

  Status from_buffer(const networking::Buffer& buffer);
};

}

#endif
