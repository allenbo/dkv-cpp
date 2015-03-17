#ifndef __DKV_REQUEST_HPP__
#define __DKV_REQUEST_HPP__

#include "dkv/status.hpp"
#include "networking/buffer.hpp"
#include <string>
#include <vector>

namespace dkv {

class Cache;

enum class ReqType : unsigned char {
  REQ_UNDEFINED = 0,

  REQ_GET,
  REQ_GETS,

  REQ_SET,
  REQ_SETS,

  REQ_DEL,
  REQ_DELS,

  REQ_UPDATE,
  REQ_UPDATES,
};

class Request {
  public:
    Request();
    ~Request();

    networking::Buffer to_buffer() const ;

    Status from_buffer(networking::Buffer& buffer);

    static Request Get(std::string& key);
    static Request Del(std::string& key);
    static Request Set(std::string& key, std::string& value);
    static Request Update(std::string& key, std::string& value);

    static Request Gets(std::vector<std::string>& keys);
    static Request Dels(std::vector<std::string>& keys);
    static Request Sets(std::vector<std::string>& keys, std::vector<std::string>& values);
    static Request Updates(std::vector<std::string>& keys, std::vector<std::string>& values);

    friend class Cache;

  private:

    ReqType type_;
    bool    isowner_;

    union {

      struct {
        char*    key;
        uint16_t key_len;
      } get, del;

      struct {
        char*    key;
        uint16_t key_len;
        char*    value;
        uint16_t val_len;
      } set, update;

      struct {
        uint16_t n;            // max number 256
        char*    keys[256];
        uint16_t key_lens[256];
      } gets, dels;

      struct {
        uint16_t n;
        char*    keys[256];
        uint16_t key_lens[256];
        char*    values[256];
        uint16_t val_lens[256];
      } sets, updates;
    };

};

}

#endif
