#include "dkv/request.hpp"

using namespace networking;

namespace dkv {

Request::Request()
    : type(ReqType::REQ_UNDEFINED),
      key(nullptr), key_len(0),
      value(nullptr), val_len(0) {
}

Status Request::from_buffer(Buffer& buffer) {
  Buffer::Byte b;
  int s = buffer.readByte(&b);
  if (s != 0) {
    return Status(Status::Code::SERIALIZATION_FAILURE);
  }

  ReqType rt = (ReqType)b;
  if (rt == ReqType::REQ_GET ||
      rt == ReqType::REQ_SET ||
      rt == ReqType::REQ_DEL) {
    int ts = 0;
    type = rt;
    s = buffer.readInt(&ts);
    if (s != 0) {
      return Status(Status::Code::SERIALIZATION_FAILURE);
    }
    key_len = ts;

    key = new char[key_len];
    s = buffer.read(key, key_len);
    if (s != 0) {
      return Status(Status::Code::SERIALIZATION_FAILURE);
    }

    if (rt == ReqType::REQ_SET) {
      s = buffer.readInt(&ts);
      if (s != 0) {
        return Status(Status::Code::SERIALIZATION_FAILURE);
      }
      val_len = ts;

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
  buffer.writeByte((Buffer::Byte)type);
  buffer.writeString(key, key_len);

  if (type == ReqType::REQ_SET) {
    buffer.writeString(value, val_len);
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
