#include "dkv/client.hpp"
#include "dkv/request.hpp"

namespace dkv {

Client::Client(std::string addr)
    : ch_(nullptr) {
  ch_ = new Channel(addr);
}

Status Client::get(std::string& key, std::string* value) {
  // send out get request
  Request req = Request::Get(key);
  
  int st = ch_->send(req.to_buffer());
  if (st != 0) {
    return Status(Status::Code::NETWORKING_ERROR);
  }

  // receive response
  Buffer resp;
  st = ch_->recv(&resp);
  if (st != 0) {
    return Status(Status::Code::NETWORKING_ERROR);
  }

  Status status;
  Status tmp_st = status.from_buffer(resp);
  if (!tmp_st.ok()) {
    return tmp_st;
  }

  if (!status.ok()) {
    return status;
  }

  st = resp.readString(value);
  if (st != 0) {
    return Status(Status::Code::SERIALIZATION_FAILURE);
  }
  return Status();
 
}

Status Client::del(std::string& key) {
  // send out del request
  Request req = Request::Get(key);

  int st = ch_->send(req.to_buffer());
  if (st != 0) {
    return Status(Status::Code::NETWORKING_ERROR);
  }

  // receive response
  Buffer resp;
  st = ch_->recv(&resp);
  if (st != 0) {
    return Status(Status::Code::NETWORKING_ERROR);
  }

  Status status;
  Status tmp_st = status.from_buffer(resp);
  if (!tmp_st.ok()) {
    return tmp_st;
  }
  return status;
}

Status Client::set(std::string& key, std::string& value) {
  // send out del request
  Request req = Request::Set(key, value);

  int st = ch_->send(req.to_buffer());
  if (st != 0) {
    return Status(Status::Code::NETWORKING_ERROR);
  }

  // receive response
  Buffer resp;
  st = ch_->recv(&resp);
  if (st != 0) {
    return Status(Status::Code::NETWORKING_ERROR);
  }

  Status status;
  Status tmp_st = status.from_buffer(resp);
  if (!tmp_st.ok()) {
    return tmp_st;
  }
  return status;
}

}
