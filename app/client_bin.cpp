#include "dkv/client.hpp"
#include "dkv/status.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

using dkv::Client;
using dkv::Status;

const char* alphabet = "abcedefghijklmnopqrstuvwxyz";

const int MIN_KEY_SIZE = 10;
const int MAX_KEY_SIZE = 250;
const int MIN_VALUE_SIZE = 250;
const int MAX_VALUE_SIZE = 1024;

const int MINOR_TEST_SIZE = 100;
const int MAJOR_TEST_SIZE = 100 * 100;

std::string rand_string(int len) {
  char* arr = new char[len];
  int total_len = strlen(alphabet);

  for (int i = 0; i < len ; i ++) {
    arr[i] = alphabet[rand() % total_len];
  }
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
  for(int i = 0; i < MINOR_TEST_SIZE; i ++ ) {
    key = rand_string(rand() % (MAX_KEY_SIZE - MIN_KEY_SIZE ) + MIN_KEY_SIZE);
    value = rand_string(rand() % (MAX_VALUE_SIZE - MIN_VALUE_SIZE ) + MIN_VALUE_SIZE);
    keys.push_back(key);
    values.push_back(value);

    st = clt.set(key, value);
    assert(st.ok());
  }

  // test for get
  for(int i = 0; i < MINOR_TEST_SIZE; i ++ ) {
    key = keys[i];
    st = clt.get(key, &value);
    assert(st.ok());
    assert(value == values[i]);
  }

  // test for del
  for(int i = 0; i < MINOR_TEST_SIZE; i ++ ) {
    key = keys[i];
    st = clt.del(key);
    assert(st.ok());
  }
  
}
