#include "dkv/client.hpp"
#include "dkv/status.hpp"
#include "common/all.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <cassert>
#include "sys/time.h"

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


float diff_time(struct timeval start, struct timeval end) {
  return end.tv_sec - start.tv_sec + (end.tv_usec - start.tv_usec) / 1000000.0;
}

int main(int argc, char** argv) {
  struct timeval start, end;
  Client clt("127.0.0.1:12321");

  std::vector<std::string> keys;
  std::vector<std::string> values;

  std::string key, value;
  Status st;
  // test for set
  std::cout << "Start set operations" << std::endl;
  gettimeofday(&start, nullptr);
  for(int i = 0; i < MAJOR_TEST_SIZE; i ++ ) {
    key = rand_string(rand() % (MAX_KEY_SIZE - MIN_KEY_SIZE ) + MIN_KEY_SIZE);
    value = rand_string(rand() % (MAX_VALUE_SIZE - MIN_VALUE_SIZE ) + MIN_VALUE_SIZE);
    keys.push_back(key);
    values.push_back(value);
    //std::cout << "Set [" << key << "| value ]" << std::endl;

    st = clt.set(key, value);
    LOG_IF(FATAL, !st.ok());
  }
  std::cout << "End set operations" << std::endl;
  gettimeofday(&end, nullptr);
  std::cout << "Set operations take " << diff_time(start, end) << "s" << std::endl;

  // test for get
  std::cout << "Start get operations" << std::endl;
  gettimeofday(&start, nullptr);
  for(int i = 0; i < MAJOR_TEST_SIZE; i ++ ) {
    key = keys[i];
    //std::cout << "Get value for key [" << key << "]" << std::endl;
    st = clt.get(key, &value);
    assert(st.ok());
    LOG_IF(FATAL, !st.ok());
    LOG_IF(FATAL, value != values[i]);
  }
  std::cout << "End of get operations" << std::endl;
  gettimeofday(&end, nullptr);
  std::cout << "Get operations take " << diff_time(start, end) << "s" << std::endl;

  // test for update
  std::cout << "Start update operations" << std::endl;
  gettimeofday(&start, nullptr);
  for(int i = 0; i < MAJOR_TEST_SIZE; i ++ ) {
    values[i] = rand_string(rand() % (MAX_VALUE_SIZE - MIN_VALUE_SIZE ) + MIN_VALUE_SIZE);
    //std::cout << "Update [" << keys[i] << "| value ]" << std::endl;
    st = clt.update(keys[i], values[i]);
    LOG_IF(FATAL, !st.ok());
  }
  std::cout << "End of update operations" << std::endl;
  gettimeofday(&end, nullptr);
  std::cout << "Update operations take " << diff_time(start, end) << "s" << std::endl;

  std::cout << "Start get operations" << std::endl;
  gettimeofday(&start, nullptr);
  for(int i = 0; i < MAJOR_TEST_SIZE; i ++ ) {
    key = keys[i];
    //std::cout << "Get value for key [" << key << "]" << std::endl;
    st = clt.get(key, &value);
    assert(st.ok());
    LOG_IF(FATAL, !st.ok());
    LOG_IF(FATAL, value != values[i]);
  }
  std::cout << "End of get operations" << std::endl;
  gettimeofday(&end, nullptr);
  std::cout << "Get operations take " << diff_time(start, end) << "s" << std::endl;

  // test for del
  std::cout << "Start del operations" << std::endl;
  gettimeofday(&start, nullptr);
  for(int i = 0; i < MAJOR_TEST_SIZE; i ++ ) {
    key = keys[i];
    //std::cout <<"Delete k/v [" << key << "]" << std::endl;
    st = clt.del(key);
    LOG_IF(FATAL, !st.ok());
  }
  std::cout << "End of del operations" << std::endl;
  gettimeofday(&end, nullptr);
  std::cout << "Del operations take " << diff_time(start, end) << "s" << std::endl;

  /*
  // test for sets
  std::vector<std::string> batch_keys;
  std::vector<std::string> batch_values;
  std::vector<std::string> rkeys;
  for(int i = 0; i < MAJOR_TEST_SIZE; i ++ ) {
    key = rand_string(rand() % (MAX_KEY_SIZE - MIN_KEY_SIZE ) + MIN_KEY_SIZE);
    value = rand_string(rand() % (MAX_VALUE_SIZE - MIN_VALUE_SIZE ) + MIN_VALUE_SIZE);
    keys.push_back(key);
    values.push_back(value);

    batch_keys.push_back(key);
    batch_values.push_back(value);
    if((i + 1) % 100 == 0 || i + 1 == MAJOR_TEST_SIZE) {
      std::cout << "Batch Set " << (i+1) / 100 << std::endl;
      st = clt.sets(batch_keys, batch_values, rkeys);

      LOG_IF(FATAL, rkeys.size() != batch_keys.size());
      for(int j = 0; j < rkeys.size(); j ++ ) {
        LOG_IF(FATAL, rkeys[j] != batch_keys[j]);
      }

      batch_keys.clear();
      batch_values.clear();
    }
  }
  */
  
}
