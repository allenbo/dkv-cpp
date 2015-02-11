#include "dkv/request.hpp"

using namespace networking;

namespace dkv {

Request::Request()
    : type(ReqType::REQ_UNDEFINED),
      key(nullptr), key_len(0),
      value(nullptr), val_len(0) {
}

Status from_buffer(const Buffer& buffer) {
  Buffer::Byte b;
  int st = buffer.readByte(&b);
  if (s != 0) {
    return Status(Status::Code::SERIALIZATION_FAILURE);
  }

  if (b == ReqType::REQ_GET ||
      b == ReqType::REQ_SET ||
      b == ReqType::REQ_DEL) {
    type = b;
    s = buffer.readInt(&key_len);
    if (s != 0) {
      return Status(Status::Code::SERIALIZATION_FAILURE);
    }

    key = new char[key_len];
    s = buffer.read(key, key_len);
    if (s != 0) {
      return Status(Status::Code::SERIALIZATION_FAILURE);
    }

    if (b == ReqType::REQ_SET) {
      s = buffer.readInt(&val_len);
      if (s != 0) {
        return Status(Status::Code::SERIALIZATION_FAILURE);
      }

      value = new char[val_len];
      s = buffer.read(value, val_len);
      if (s != 0) {
        return Status(Status::Code::SERIALIZATION_FAILURE);
      }
    }
  } else {
    return Status(Status::Code::SERIALIZATION_FAILURE);
  }
  return Status();
}

Buffer Request::to_buffer() {
  Buffer buffer;
  buffer.writeByte(type);
  buffer.writeString(key, key_len);

  if (type == ReqType::REQ_SET) {
    buffer.writeString(value, val_ken);
  }
  return buffer;
}

Request::~Request() {
  if (key) {
    delete []key;
  }
  if (value ) {
    delete []value;
  }
}

};
