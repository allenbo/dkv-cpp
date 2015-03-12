#include "dkv/server.hpp"
#include "networking/buffer.hpp"

namespace dkv {

Server::Server(std::string addr)
    : pool_(), stop_(false), map_change_mutex_() {
  cache_    = new Cache();
  acceptor_ = new Acceptor(addr); 
  channels_.clear();
}

Server::~Server() {
  if (!stop_){
    stop();
  }
  
  delete acceptor_;
  delete cache_;
}

Status Server::start() {
  Channel* ch = nullptr;
  bool timeouted = false;
  while (!stop_) {
    if (!timeouted) {
      ch = new Channel(true); //create an async channel
    }
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 1000; // 1ms

    int st = acceptor_->accept(ch, tv);

    if (st != 0) {
      // timeout
      timeouted = true;
      continue;
    }

    timeouted = false;

    ch->set_async_handler(this);

    {
      common::ScopeLock _(&map_change_mutex_);
      channels_[ch->socket()] = ch;
    }

    ch->irecv();
  }
  return Status();
}

Status Server::stop() {
  if (stop_) {
    return Status();
  }

  stop_ = true;

  auto it = channels_.begin();
  for(; it != channels_.end(); it ++) {
    it->second->close();
    delete it->second;
  }
  channels_.clear();
  return Status();
}

void Server::on_recv_complete(Channel* ch, Buffer buf) {
  pool_.add(&Server::handle_request, this, ch, buf);
}

void Server::on_send_complete(Channel* ch) {
  LOG(DEBUG) << "On send complete" << std::endl;
  ch->irecv();
}

void Server::on_channel_close(Channel* ch) {
  ch->close();
  channels_.erase(ch->socket());
  delete ch;
}

void Server::handle_request(Channel* ch, Buffer buf) {
  Request req;
  Status st = req.from_buffer(buf);

  networking::Buffer st_buffer = st.to_buffer();

  if (st.ok()) {
    // send out a buffer
    networking::Buffer resp_buffer;
    st = cache_->process(req, resp_buffer);

    st_buffer = st.to_buffer();
    // concatenate status and response
    if (st.ok()) {
      st_buffer.append(resp_buffer);
    }

  }
  ch->isend(st_buffer);
}

}
