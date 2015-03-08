#include "dkv/status.hpp"

namespace dkv {

using namespace networking;

Status  Status::from_buffer(Buffer& buf) {
  Buffer::Byte b;
  int st = buf.readByte(&b);
  if (st != 0) {
    return Status(Code::SERIALIZATION_FAILURE);
  }

  code_ = (Code)b;
  return Status();
}

Buffer Status::to_buffer() {
  Buffer buf;
  buf.writeByte((Buffer::Byte)code_);
  return buf;
}

}
