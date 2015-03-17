#include "dkv/client.hpp"
#include "dkv/request.hpp"

namespace dkv {

Client::Client(std::string addr)
    : ch_(nullptr) {
  ch_ = new Channel(addr);
}

Status Client::send_and_recv(const Buffer& sb, Buffer& rb) {
  int st = ch_->send(sb);
  if (st) {
    return Status(Status::Code::NETWORKING_ERROR);
  }

  st = ch_->recv(&rb);
  if (st != 0) {
    return Status(Status::Code::NETWORKING_ERROR);
  }
  return Status();

}

Status Client::get(std::string& key, std::string* value) {
  // send out get request
  Request req = Request::Get(key);
  Buffer resp;

  Status st = send_and_recv(req.to_buffer(), resp);
  if (!st.ok()){
    return st;
  }
  
  Status status;
  st = status.from_buffer(resp);
  if (!st.ok()) {
    return st;
  }

  if (!status.ok()) {
    return status;
  }

  int ist = resp.readString(value);
  if (ist != 0) {
    return Status(Status::Code::SERIALIZATION_FAILURE);
  }
  return Status();
 
}

Status Client::del(std::string& key) {
  // send out del request
  Request req = Request::Del(key);
  Buffer resp;

  Status st = send_and_recv(req.to_buffer(), resp);
  if (!st.ok()) {
    return st;
  }

  Status status;
  st = status.from_buffer(resp);
  if (!st.ok()) {
    return st;
  }
  return status;
}

Status Client::set(std::string& key, std::string& value) {
  // send out del request
  Request req = Request::Set(key, value);
  Buffer resp;

  Status st = send_and_recv(req.to_buffer(), resp);
  if (!st.ok()) {
    return st;
  }
  
  Status status;
  st = status.from_buffer(resp);
  if (!st.ok()) {
    return st;
  }
  return status;
}

Status Client::update(std::string& key, std::string& value) {
  Request req = Request::Update(key, value);
  Buffer resp;

  Status st = send_and_recv(req.to_buffer(), resp);
  if (!st.ok()) {
    return st;
  }

  Status status;
  st = status.from_buffer(resp);
  if (!st.ok()) {
    return st;
  }
  return status;
}


Status Client::gets(std::vector<std::string>& keys,
                    std::vector<std::string>& rkeys,
                    std::vector<std::string>& rvalues) {
  Request req = Request::Gets(keys);
  Buffer resp;

  Status st = send_and_recv(req.to_buffer(), resp);
  if (!st.ok()) {
    return st;
  }

  // parse status
  Status status;
  st = status.from_buffer(resp);
  if (!st.ok()) {
    return st;
  } 

  rkeys.clear();
  rvalues.clear();

  std::string v;
  std::string k;
  while (resp.peek() > 0){
    int ist = resp.readString(&k);
    if (ist != 0) {
      return Status(Status::Code::SERIALIZATION_FAILURE);
    }
    ist = resp.readString(&v);
    if (ist != 0) {
      return Status(Status::Code::SERIALIZATION_FAILURE);
    }
    rkeys.push_back(k);
    rvalues.push_back(v);
  }
  return status;
}

Status Client::sets(std::vector<std::string>& keys,
                    std::vector<std::string>& values,
                    std::vector<std::string>& rkeys) {
  Request req = Request::Sets(keys, values);
  return multi_key_op(req, rkeys);
}

Status Client::dels(std::vector<std::string>& keys,
                    std::vector<std::string>& rkeys) {
  Request req = Request::Dels(keys);
  return multi_key_op(req, rkeys);
}

Status Client::updates(std::vector<std::string>& keys,
                       std::vector<std::string>& values,
                       std::vector<std::string>& rkeys) {
  Request req = Request::Updates(keys, values);
  return multi_key_op(req, rkeys);
}

Status Client::multi_key_op(const Request& req,
                            std::vector<std::string>& rkeys) {
  Buffer resp;
  Status st = send_and_recv(req.to_buffer(), resp);
  if (!st.ok()) {
    return st;
  }

  // parse status
  Status status;
  st = status.from_buffer(resp);
  if (!st.ok()) {
    return st;
  } 

  rkeys.clear();
  std::string k;

  while (resp.peek() > 0){
    int ist = resp.readString(&k);
    if (ist != 0) {
      return Status(Status::Code::SERIALIZATION_FAILURE);
    }
    rkeys.push_back(k);
  }
  return status;
}

}
