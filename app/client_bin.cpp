#include "dkv/client.hpp"
#include "dkv/status.hpp"
#include "common/all.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

using dkv::Client;
using dkv::Status;

const char* alphabet = "abcedefghijklmnopqrstuvwxyz";

const int MIN_KEY_SIZE = 100;
const int MAX_KEY_SIZE = 250;
const int MIN_VALUE_SIZE = 1;
const int MAX_VALUE_SIZE = 4 * 1024;

const int MINOR_TEST_SIZE = 100;
const int MAJOR_TEST_SIZE = 100 * 100;

std::string rand_string(int len) {
  char* arr = new char[len + 1];
  int total_len = strlen(alphabet);

  for (int i = 0; i < len ; i ++) {
    arr[i] = alphabet[rand() % total_len];
  }
  arr[len] = 0;
  std::string result(arr);
  delete arr;
  return result;
}


int main(int argc, char** argv) {
  Client clt("127.0.0.1:12321");

  std::vector<std::string> keys;
  std::vector<std::string> values;

  std::string key, value;
  Status st;
  // test for set
  for(int i = 0; i < MAJOR_TEST_SIZE; i ++ ) {
    key = rand_string(rand() % (MAX_KEY_SIZE - MIN_KEY_SIZE ) + MIN_KEY_SIZE);
    value = rand_string(rand() % (MAX_VALUE_SIZE - MIN_VALUE_SIZE ) + MIN_VALUE_SIZE);
    keys.push_back(key);
    values.push_back(value);
    std::cout << "Set [" << key << "| value ]" << std::endl;

    st = clt.set(key, value);
    LOG_IF(FATAL, !st.ok());
  }

  // test for get
  for(int i = 0; i < MAJOR_TEST_SIZE; i ++ ) {
    key = keys[i];
    std::cout << "Get value for key [" << key << "]" << std::endl;
    st = clt.get(key, &value);
    assert(st.ok());
    LOG_IF(FATAL, !st.ok());
    LOG_IF(FATAL, value != values[i]);
  }

  // test for update
  for(int i = 0; i < MAJOR_TEST_SIZE; i ++ ) {
    values[i] = rand_string(rand() % (MAX_VALUE_SIZE - MIN_VALUE_SIZE ) + MIN_VALUE_SIZE);
    std::cout << "Update [" << keys[i] << "| value ]" << std::endl;
    st = clt.update(keys[i], values[i]);
    LOG_IF(FATAL, !st.ok());
  }

  for(int i = 0; i < MAJOR_TEST_SIZE; i ++ ) {
    key = keys[i];
    std::cout << "Get value for key [" << key << "]" << std::endl;
    st = clt.get(key, &value);
    assert(st.ok());
    LOG_IF(FATAL, !st.ok());
    LOG_IF(FATAL, value != values[i]);
  }

  // test for del
  for(int i = 0; i < MAJOR_TEST_SIZE; i ++ ) {
    key = keys[i];
    std::cout <<"Delete k/v [" << key << "]" << std::endl;
    st = clt.del(key);
    LOG_IF(FATAL, !st.ok());
  }


  
}
