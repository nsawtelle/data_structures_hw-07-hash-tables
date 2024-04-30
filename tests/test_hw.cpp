#include "../code/Hash.h"
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

class test_Hash : public ::testing::Test {
protected:
  // This function runs only once before any TEST_F function
  static void SetUpTestCase() {
    ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
      outgrade << (int)0;
      outgrade.close();
    }
  }

  // This function runs after all TEST_F functions have been executed
  static void TearDownTestCase() {
    ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
      outgrade << (int)ceil(100 * total_grade / max_grade);
      outgrade.close();
      cout << "Points earned:   " << total_grade << endl;
      cout << "Points possible: " << max_grade << endl;
      cout << "Total Grade as a percentage is:  "
           << (int)ceil(100 * total_grade / max_grade) << "%" << endl;
    }
  }

  void add_points_to_grade(double points) {
    if (!::testing::Test::HasFailure()) {
      total_grade += points;
    }
  }

  // this function runs before every TEST_F function
  void SetUp() override {}

  // this function runs after every TEST_F function
  void TearDown() override {
    ofstream outgrade("./total_grade.txt");
    if (outgrade.is_open()) {
      outgrade.clear();
      outgrade << (int)ceil(100 * total_grade / max_grade);
      outgrade.close();
    }
  }

  static double total_grade;
  static double max_grade;
};

double test_Hash::total_grade = 0;
double test_Hash::max_grade = 166;

/////////////////////////////////////////
// here are some helper functions for tests
string mkstring(string label, int num) {
  ostringstream k;
  k << label << " " << num;
  return k.str();
}

string random_string(size_t length) {
  auto randchar = []() -> char {
    const char charset[] = "0123456789"
                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                           "abcdefghijklmnopqrstuvwxyz";
    const size_t max_index = (sizeof(charset) - 1);
    return charset[rand() % max_index];
  };
  string str(length, 0);
  generate_n(str.begin(), length, randchar);
  return str;
}

void print_short(hash_table* tbl, string msg) {
  printf(
      "hash_table_load: %4.2f, size: %8d, capacity: %8d, occupied: %8d (%s)\n",
      hash_table_load(tbl), tbl->size, tbl->capacity, tbl->occupied,
      msg.c_str());
}

/////////////////////////////////////////

TEST_F(test_Hash, init_hash_node) {

  hash_node* n = init_hash_node("some key", 1234, "some value");
  ASSERT_TRUE(n);
  add_points_to_grade(2);
  ASSERT_EQ(n->key, "some key");
  add_points_to_grade(2);
  ASSERT_EQ(n->hashcode, 1234);
  add_points_to_grade(2);
  ASSERT_EQ(n->value, "some value");
  add_points_to_grade(2);
  ASSERT_EQ(n->deleted, false);
  add_points_to_grade(2);
}

TEST_F(test_Hash, init_hash_table) {

  hash_table* tbl = init_hash_table(16);
  ASSERT_TRUE(tbl);
  add_points_to_grade(1);
  ASSERT_EQ(tbl->capacity, 16);
  add_points_to_grade(1);
  ASSERT_EQ(tbl->size, 0);
  add_points_to_grade(1);
  ASSERT_EQ(tbl->occupied, 0);
  add_points_to_grade(1);
  ASSERT_TRUE(tbl->table);
  add_points_to_grade(1);
  ASSERT_TRUE(tbl->hash_func);
  add_points_to_grade(1);
  ASSERT_TRUE(tbl->bucket_func);
  add_points_to_grade(1);
  unsigned int numCorrect = 0;
  for (unsigned int i = 0; i < tbl->capacity; i++) {
    ASSERT_FALSE(tbl->table->at(i));
    numCorrect++;
  }
  add_points_to_grade(numCorrect == tbl->capacity ? 1 : 0);
}
TEST_F(test_Hash, HashFunction) {
  // requires init_table

  hash_table* tbl = init_hash_table(16);
  ASSERT_TRUE(tbl);
  ASSERT_EQ(tbl->hash_func(""), 5381);
  add_points_to_grade(4);
  ASSERT_EQ(tbl->hash_func("sample text"), 3112731660);
  add_points_to_grade(4);
}

TEST_F(test_Hash, BucketFunction) {
  // requires init_table

  hash_table* tbl = init_hash_table(16);
  ASSERT_TRUE(tbl);
  ASSERT_EQ(tbl->bucket_func(40, tbl->capacity), 8);
  add_points_to_grade(8);
}

TEST_F(test_Hash, SetKeyValuePair) {
  // requires init_table, hash_func, bucket_func
  bool ret_val;

  hash_table* tbl = init_hash_table(16);
  ASSERT_TRUE(tbl);
  ret_val = set_kvp(tbl, "fantastic key", "value for fantastic key");
  ASSERT_TRUE(ret_val);
  add_points_to_grade(3);
  unsigned int hashcode = tbl->hash_func("fantastic key");
  unsigned int bucket_idx = tbl->bucket_func(hashcode, tbl->capacity);
  ASSERT_GE(bucket_idx, 0); // Expect bucket_idx >= 0
  add_points_to_grade(3);
  ASSERT_LT(bucket_idx, tbl->capacity); // expect bucket_idx < tbl->capacity
  add_points_to_grade(3);
  ASSERT_TRUE(tbl->table->at(bucket_idx));
  add_points_to_grade(3);
  ASSERT_EQ(tbl->table->at(bucket_idx)->value, "value for fantastic key");
  add_points_to_grade(3);

  // same string means same hashcode; should place it in same spot
  ret_val = set_kvp(tbl, "fantastic key", "value for the second fantastic key");
  ASSERT_TRUE(ret_val);
  add_points_to_grade(3);
  ASSERT_TRUE(tbl->table->at(bucket_idx));
  add_points_to_grade(3);
  ASSERT_EQ(tbl->table->at(bucket_idx)->value,
            "value for the second fantastic key");
  add_points_to_grade(3);

  // now fill the thing up. there should be room for all of this.
  int numCorrect = 0;
  for (int i = 0; i < 15; i++) {
    string k = mkstring("some key", i);
    string v = mkstring("some value", i);
    ret_val = set_kvp(tbl, k, v);
    ASSERT_TRUE(ret_val);
    numCorrect++;
  }

  add_points_to_grade(min(numCorrect, 5));

  // the next set_kvp should be rejected because it is 100% full.
  ret_val = set_kvp(tbl, "wafer", "just one more little waferrrrr");
  ASSERT_FALSE(ret_val);
  add_points_to_grade(3);
}

TEST_F(test_Hash, hash_table_load) {

  // requires init_table and set_kvp
  hash_table* tbl = init_hash_table(16);
  ASSERT_TRUE(tbl);
  float last_load = 0;
  float this_load = 0;
  ASSERT_EQ(hash_table_load(tbl), last_load); // sanity check
  add_points_to_grade(1);
  int numCorrect = 0;
  for (unsigned int i = 0; i < tbl->capacity; i++) {
    string k = mkstring("some key", i);
    string v = mkstring("some value", i);
    set_kvp(tbl, k, v);
    this_load = hash_table_load(tbl);
    ASSERT_LT(last_load, this_load); // expect (last_load < this_load) , load
                                     // should go up every time

    numCorrect++;
    last_load = this_load;
  }
  add_points_to_grade(min(16, numCorrect));
  set_kvp(tbl, "last string", "first string");
  this_load = hash_table_load(tbl);
  ASSERT_EQ(last_load, this_load); // load stays same since tbl is full
  add_points_to_grade(1);
}

TEST_F(test_Hash, get_value) {
  // requires init_table and set_kvp

  hash_table* tbl = init_hash_table(16);
  ASSERT_TRUE(tbl);

  // first ensure getting a value that isn't there returns empty string
  string response;
  response = get_value(tbl, "favorite color");
  add_points_to_grade(10);
  ASSERT_EQ(response, "");

  // now set that key/value pair so it has a value
  set_kvp(tbl, "favorite color", "green");
  response = get_value(tbl, "favorite color");
  add_points_to_grade(10);
  ASSERT_EQ(response, "green");

  // now overwrite that key/value pair and be sure it sticks.
  set_kvp(tbl, "favorite color", "puce");
  response = get_value(tbl, "favorite color");
  ASSERT_EQ(response, "puce");
  add_points_to_grade(10);
}

TEST_F(test_Hash, contains) {
  // requires init_table and set_kvp

  hash_table* tbl = init_hash_table(16);
  ASSERT_TRUE(tbl);

  ASSERT_FALSE(contains(tbl, "say what"));
  add_points_to_grade(8);

  set_kvp(tbl, "say what", "oh hi there's a value now");

  ASSERT_TRUE(contains(tbl, "say what"));
  add_points_to_grade(8);
}

TEST_F(test_Hash, remove) {
  // requires init_table and set_kvp

  hash_table* tbl = init_hash_table(16);
  ASSERT_TRUE(tbl);
  ASSERT_FALSE(
      remove(tbl, "quay")); // ensure removing non-existing key yields false
  add_points_to_grade(4);

  set_kvp(tbl, "quay", "on down yon in temple bar"); // add kvp
  ASSERT_EQ(tbl->size, 1);                           // ensure size is now 1
  add_points_to_grade(4);

  ASSERT_EQ(tbl->occupied, 1); // occupied also 1
  add_points_to_grade(4);

  ASSERT_FALSE(remove(tbl, "no such key")); // remove a key that's not there
  add_points_to_grade(4);

  ASSERT_EQ(tbl->size, 1); // don't touch size if nothing was removed
  add_points_to_grade(4);

  ASSERT_EQ(tbl->occupied, 1); // same with occupied
  add_points_to_grade(4);

  ASSERT_TRUE(remove(tbl, "quay")); // now remove the key that we know is there
  add_points_to_grade(4);

  ASSERT_EQ(tbl->size, 0); // be sure to reduce map size when removing a key
  add_points_to_grade(4);

  ASSERT_EQ(tbl->occupied, 1); // the removed node is still taking up space
  add_points_to_grade(4);
}

TEST_F(test_Hash, resize) {
  // requires:
  //   - init_table
  //   - set_kvp
  //   - remove
  //   - resize

  hash_table* tbl = init_hash_table(16);
  ASSERT_TRUE(tbl);

  string keys[10];
  for (unsigned int i = 0; i < 10; i++) {
    keys[i] = random_string(8);
    set_kvp(tbl, keys[i], random_string(16));
  }

  // these two just establish a baseline
  ASSERT_EQ(tbl->capacity, 16);
  ASSERT_EQ(tbl->size, 10);

  // Now resize the table by doubling the capacity
  resize(tbl, tbl->capacity * 2);
  ASSERT_EQ(tbl->capacity, 32); // capacity should double
  // add_points_to_grade(1);

  ASSERT_EQ(tbl->size, 10); // but number of items should stay same
  // add_points_to_grade(1);

  // Now remove four items, then resize back to 16
  remove(tbl, keys[2]);
  remove(tbl, keys[4]);
  remove(tbl, keys[6]);
  remove(tbl, keys[8]);
  ASSERT_EQ(tbl->capacity, 32);
  // add_points_to_grade(1);

  ASSERT_EQ(tbl->size, 6);
  // add_points_to_grade(1);

  resize(tbl, tbl->capacity / 2);
  ASSERT_EQ(tbl->capacity, 16);
  // add_points_to_grade(1);

  ASSERT_EQ(tbl->size, 6);
  // add_points_to_grade(1);
}
