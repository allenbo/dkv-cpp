#ifndef __DKV_SERVER_HPP__
#define __DKV_SERVER_HPP__

#include <string>
#include "dkv/networking/buffer.hpp"
#include "dkv/networking/channel.hpp"
#include "dkv/networking/acceptor.hpp"
#include "dkv/request.hpp"
#include "dkv/response.hpp"
#include "dkv/cache.hpp"

#include "common/all.hpp"

namespace dkv {

using networking::Asio;
using networking::Buffer;
using networking::Channel;
using networking::Acceptor;

class Server : public Asio {
  public:

    /**
     * Create a server instance
     *
     * @param addr port for server to listen to
     */
    Server(std::string addr = "12321");

    ~Server();

    /**
     * Server start to process request from clients
     *
     * @return status of this method
     */
    Status start();

    /**
     * Server stop  processing
     *
     * @return Status of this method
     */
    Status stop();

    /**
     * Callback of asynchronous receive
     *
     * @param ch channel that finishes async recv
     * @param buf buffer received
     */
    void on_recv_complete(Channel* ch, Buffer buf);

    /**
     * Callback of asynchronous send
     *
     * @param ch channel that finishes async send
     */
    void on_send_complete(Channel* ch);

    /**
     * Callback of remote channel closed
     *
     * @param ch channel that closed by remote node
     */
    void on_channel_close(Channel* ch);

  private:

    Cache                   *cache_;   // a thread safe cache
    Acceptor                acceptor_; // accept remote connections
    std::map<int, Channel*> channels_; // keep alive connections into channels

    common::ThreadPool      pool_;

    bool                    stop_;
    common::Mutex           map_change_mutex_;

    void handle_request(Channel* ch, Buffer& buf);

    // no copy
    Server(const Server&);
    Server& operator=(const Server&);
};

}

#endif
