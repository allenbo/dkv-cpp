#include "dkv/request.hpp"
#include "common/all.hpp"

using namespace networking;

namespace dkv {

Status read_char_with_len(Buffer& buffer, char** str, uint16_t* len) {
  int ts = 0, s = 0;
  s = buffer.readInt(&ts);
  if (s != 0) {
    return Status(Status::Code::SERIALIZATION_FAILURE);
  }
  *len = ts;

  *str = new char[*len + 1];
  s = buffer.read(*str, *len);
  (*str)[*len] = 0;
  if (s != 0) {
    return Status(Status::Code::SERIALIZATION_FAILURE);
  }
  return Status();
}

Status read_uint16(Buffer& buffer, uint16_t* len) {
  int s = 0;
  s = buffer.readUShort(len);
  if (s != 0) {
    return Status(Status::Code::SERIALIZATION_FAILURE);
  }
  return Status();
}

Request::Request()
    :type_(ReqType::REQ_UNDEFINED),
     isowner_(true) {
}

Request Request::Get(std::string& key) {
  Request r;
  r.type_    = ReqType::REQ_GET;
  r.isowner_ = false;

  r.get.key     = (char*)key.c_str();
  r.get.key_len = key.size();
  return r;
}

Request Request::Del(std::string& key) {
  Request r;
  r.type_    = ReqType::REQ_DEL;
  r.isowner_ = false;

  r.del.key     = (char*)key.c_str();
  r.del.key_len = key.size();
  return r;
}

Request Request::Set(std::string& key, std::string& value) {
  Request r;
  r.type_    = ReqType::REQ_SET;
  r.isowner_ = false;

  r.set.key     = (char*)key.c_str();
  r.set.key_len = key.size();

  r.set.value   = (char*)value.c_str();
  r.set.val_len = value.size();
  return r;
}

Request Request::Update(std::string& key, std::string& value) {
  Request r;
  r.type_    = ReqType::REQ_UPDATE;
  r.isowner_ = false;

  r.update.key     = (char*)key.c_str();
  r.update.key_len = key.size();

  r.update.value   = (char*)value.c_str();
  r.update.val_len = value.size();
  return r;
}

Request Request::Gets(std::vector<std::string>& keys) {
  CHECK_LE(keys.size(), 256); 
  Request r;

  r.type_    = ReqType::REQ_GETS;
  r.isowner_ = false;
  r.gets.n   = keys.size();
  for (size_t i = 0 ; i < keys.size(); i ++ ) {
    r.gets.keys[i] = (char*)keys[i].c_str();
    r.gets.key_lens[i] = keys[i].size();
  }
  return r;
}

Request Request::Dels(std::vector<std::string>& keys) {
  CHECK_LE(keys.size(), 256); 

  Request r;
  r.type_    = ReqType::REQ_DELS;
  r.isowner_ = false;
  r.dels.n   = keys.size();
  for (size_t i = 0 ; i < keys.size(); i ++ ) {
    r.dels.keys[i] = (char*)keys[i].c_str();
    r.dels.key_lens[i] = keys[i].size();
  }
  return r;
}

Request Request::Sets(std::vector<std::string>& keys, std::vector<std::string>& values) {
  CHECK_LE(keys.size(), 256); 
  CHECK_EQ(keys.size(), values.size());
  Request r;

  r.type_    = ReqType::REQ_SETS;
  r.isowner_ = false;
  r.sets.n   = keys.size();
  for (size_t i = 0 ; i < keys.size(); i ++ ) {
    r.sets.keys[i] = (char*)keys[i].c_str();
    r.sets.key_lens[i] = keys[i].size();
    r.sets.values[i] = (char*)values[i].c_str();
    r.sets.val_lens[i] = values[i].size();
  }
  return r;
}

Request Request::Updates(std::vector<std::string>& keys, std::vector<std::string>& values) {
  CHECK_LE(keys.size(), 256); 
  CHECK_EQ(keys.size(), values.size());
  Request r;

  r.type_    = ReqType::REQ_UPDATES;
  r.isowner_ = false;
  r.updates.n   = keys.size();
  for (size_t i = 0 ; i < keys.size(); i ++ ) {
    r.updates.keys[i] = (char*)keys[i].c_str();
    r.updates.key_lens[i] = keys[i].size();
    r.updates.values[i] = (char*)values[i].c_str();
    r.updates.val_lens[i] = values[i].size();
  }
  return r;
}

Request::~Request() {
  if (isowner_) {
    switch(type_) {
      case ReqType::REQ_GET: delete [] get.key; break;
      case ReqType::REQ_DEL: delete [] del.key; break;
      case ReqType::REQ_SET: delete [] set.key; delete [] set.value; break;
      case ReqType::REQ_UPDATE: delete [] update.key; delete [] update.value; break;

      case ReqType::REQ_GETS:
        for (int i = 0; i < (int)gets.n; i ++ )
          delete [] gets.keys[i];
        break;
      case ReqType::REQ_DELS:
        for (int i = 0; i < (int)dels.n; i ++ )
          delete [] dels.keys[i];
        break;
      case ReqType::REQ_SETS:
        for (int i = 0; i < (int)sets.n; i ++ ) {
          delete [] sets.keys[i];
          delete [] sets.values[i];
        }
        break;
      case ReqType::REQ_UPDATES:
        for (int i = 0; i < updates.n; i ++ ) {
          delete [] updates.keys[i];
          delete [] updates.values[i];
        }
        break;
      case ReqType::REQ_UNDEFINED:
        break;
    }
  }
}

Status Request::from_buffer(Buffer& buffer) {
  Buffer::Byte b;
  int s = buffer.readByte(&b);
  if (s != 0) {
    return Status(Status::Code::SERIALIZATION_FAILURE);
  }

  ReqType rt = (ReqType)b;
  type_ = rt;

  Status rst;

  switch(type_) {
    case ReqType::REQ_GET:
      rst = read_char_with_len(buffer, &get.key, &get.key_len);
      if (!rst.ok()) {
        return rst;
      }
      break;
    case ReqType::REQ_DEL:
      rst = read_char_with_len(buffer, &del.key, &del.key_len);
      if (!rst.ok()) {
        return rst;
      }
      break;
    case ReqType::REQ_SET:
      rst = read_char_with_len(buffer, &set.key, &set.key_len);
      if (!rst.ok()) {
        return rst;
      }

      rst = read_char_with_len(buffer, &set.value, &set.val_len);
      if (!rst.ok()) {
        return rst;
      }
      break;
    case ReqType::REQ_UPDATE:
      rst = read_char_with_len(buffer, &update.key, &update.key_len);
      if (!rst.ok()) {
        return rst;
      }

      rst = read_char_with_len(buffer, &update.value, &update.val_len);
      if (!rst.ok()) {
        return rst;
      }
      break;
    case ReqType::REQ_GETS:
      rst = read_uint16(buffer, &gets.n);
      if (!rst.ok()) {
        return rst;
      }
      
      for(int i = 0; i < (int)gets.n; i ++) {
        rst = read_char_with_len(buffer, &gets.keys[i], &gets.key_lens[i]);
        if (!rst.ok()) {
          return rst;
        }
      }
      break;
    case ReqType::REQ_DELS:
      rst = read_uint16(buffer, &dels.n);
      if (!rst.ok()) {
        return rst;
      }
      
      for(int i = 0; i < (int)dels.n; i ++) {
        rst = read_char_with_len(buffer, &dels.keys[i], &dels.key_lens[i]);
        if (!rst.ok()) {
          return rst;
        }
      }
      break;
    case ReqType::REQ_SETS:
      rst = read_uint16(buffer, &sets.n);
      if (!rst.ok()) {
        return rst;
      }
      
      for(int i = 0; i < (int)sets.n; i ++) {
        rst = read_char_with_len(buffer, &sets.keys[i], &sets.key_lens[i]);
        if (!rst.ok()) {
          return rst;
        }
        rst = read_char_with_len(buffer, &sets.values[i], &sets.val_lens[i]);
        if (!rst.ok()) {
          return rst;
        }
      }
      break;
    case ReqType::REQ_UPDATES:
      rst = read_uint16(buffer, &updates.n);
      if (!rst.ok()) {
        return rst;
      }
      
      for(int i = 0; i < (int)updates.n; i ++) {
        rst = read_char_with_len(buffer, &updates.keys[i], &updates.key_lens[i]);
        if (!rst.ok()) {
          return rst;
        }
        rst = read_char_with_len(buffer, &updates.values[i], &updates.val_lens[i]);
        if (!rst.ok()) {
          return rst;
        }
      }
      break;
    case ReqType::REQ_UNDEFINED:
      return Status(Status::Code::SERIALIZATION_FAILURE);
      break;
  }
  return Status();
}

Buffer Request::to_buffer()  const {
  Buffer buffer;
  buffer.writeByte((Buffer::Byte)type_);

  switch(type_) {
    case ReqType::REQ_GET:
      buffer.writeString(get.key, get.key_len);
      break;
    case ReqType::REQ_DEL:
      buffer.writeString(del.key, del.key_len);
      break;
    case ReqType::REQ_SET:
      buffer.writeString(set.key, set.key_len);
      buffer.writeString(set.value, set.val_len);
      break;
    case ReqType::REQ_UPDATE:
      buffer.writeString(update.key, update.key_len);
      buffer.writeString(update.value, update.val_len);
      break;
    case ReqType::REQ_GETS:
      buffer.writeUShort(gets.n);
      for(int i = 0; i < (int)gets.n; i ++) {
        buffer.writeString(gets.keys[i], gets.key_lens[i]);
      }
      break;
    case ReqType::REQ_DELS:
      buffer.writeUShort(dels.n);
      for(int i = 0; i < (int)dels.n; i ++) {
        buffer.writeString(dels.keys[i], dels.key_lens[i]);
      }
      break;
    case ReqType::REQ_SETS:
      buffer.writeUShort(sets.n);
      for(int i = 0; i < (int)sets.n; i ++) {
        buffer.writeString(sets.keys[i], sets.key_lens[i]);
        buffer.writeString(sets.values[i], sets.val_lens[i]);
      }
      break;
    case ReqType::REQ_UPDATES:
      buffer.writeUShort(updates.n);
      for(int i = 0; i < (int)updates.n; i ++) {
        buffer.writeString(updates.keys[i], updates.key_lens[i]);
        buffer.writeString(updates.values[i], updates.val_lens[i]);
      }
      break;
    case ReqType::REQ_UNDEFINED:
      break;
  }
  return buffer;
}

};
