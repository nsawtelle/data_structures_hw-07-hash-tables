#ifndef HASH_H__
#define HASH_H__

#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

// Advice for approaching this assignment:
//
// Many of the unit tests depend on more than one function. For example, the
// [hash func] test requires init_table to work. Because of this I recommend you
// proceed in this order:
//
// 1. init node
// 2. init table
// 3. hash func
// 4. bucket func
// 5. set
//
// Once you're there, you can take on the rest: load, get, contains, remove, and
// (if you're feeling saucy) resize.

// hash_node represents the contents of an individual bucket in your hash table.
struct hash_node {
  // deleted indicates if the node was marked as removed from the hash table. it
  // should be initialized to false.
  bool deleted;

  // key is the key the user provides to associate with a value.
  string key;

  // hashcode is the hashcode computed by the hash table's hash_func on the key.
  // it is stored to avoid having to re-compute it.
  unsigned int hashcode;

  // value is the user value associated with the key.
  string value;
};

// You can define custom types like this one using "typedef".
//
// This type definition is for "htable". It is a vector where each element is a
// pointer to a hash_node object. htable is used in hash_table struct below. You
// can define a table dynamically for example:
//
//     // dynamically create a hash_table object first
//     hash_table* myhashtable(new hash_table);
//     // then create an htable for the size you like (e.g. mysize)
//     myhashtable->table = htable*(new htable(mysize));
//
// You can also use resize() property of vector to resize your table later.
//
//     myhashtable->table.resize(new_size);
typedef vector<hash_node*> htable;

// hash_table is the structure that holds your hashed data and associated
// metadata and hash function references.
struct hash_table {
  // capacity current number of addressable buckets in table. kept up-to-date.
  unsigned int capacity;

  // number of actual (not deleted) entries. kept up-to-date.
  unsigned int size;

  // number of non-null buckets. (so all hash_nodes, deleted or otherwise). kept
  // up-to-date.
  unsigned int occupied;

  // table points to dynamic array of hash_node pointers (read comments for
  // htable above). It should be initialized to a dynamic array of pointers to
  // hash_node structs. Those pointers should all initially be NULL.
  htable* table;

  // hash_func is the currently in-use hash function. it should be initialized
  // to the djb2 hash function. This is neat because it shows you how you can
  // use functions as hot-swappable values at runtime!
  //
  // to assign this, simply do:
  //
  // mytable->hash_func = djb2; // any function with signature like djb2 works.
  //
  // to use this:
  //
  // unsigned int hashcode_of_foo = mytable->hash_func("foo");
  unsigned int (*hash_func)(string);

  // bucket_func is the currently in-use bucket indexing function. it should be
  // initialized to the modulo_bucket_func function. assign and use similarly to
  // hash_func.
  unsigned int (*bucket_func)(unsigned int hashcode, unsigned int capacity);
};

// djb2 is a hash function that hashes strings and gives an unsigned integer
// hash code that could take on any value in the 32-bit unsigned integer range.
// It is implemented for you.
unsigned int djb2(string key);

// mod_bucket_func simply returns hashcode % cap. It is provided here as a
// separate function because you *could* write your own hash function for
// putting keys into buckets. It is implemented for you.
unsigned int mod_bucket_func(unsigned int hashcode, unsigned int cap);

// init_hash_table creates and initializes a hash_table structure and returns a
// pointer to it. You must initialize all of the fields of this struct,
// including the two functions. See the hash_table documentation for details on
// the fields and their expected default values.
//
// Use the provided 'cap' paramter as the hash table's initial capacity.
hash_table* init_hash_table(unsigned int cap);

// init_hash_node creates and initializes a hash_node that will occupy a hash
// table bucket. Use the provided key, hashcode, and value; see the hash_node
// documentation for more info about the rest of the initial values.
hash_node* init_hash_node(string key, unsigned int hashcode, string val);

// set_kvp establishes a mapping between the given key and value pair in the
// provided hash table. If the key (as identified by its hash code) is already
// in the hash table, its value is potentially updated. Otherwise, a new
// key/value pair is added to the hash table.
//
// This is where you will use linear probing. You're recommended to make a
// separate "probe" function.
//
// On exit, the size and occupancy of the hash table is increased if a new
// key/value pair was added.
//
// If there is no room in the hash table, return false. Do not resize the table.
//
// The return value should be true unless there was no room in the hash table.
bool set_kvp(hash_table* tbl, string key, string value);

// hash_table_load returns a load factor describing how 'full' the table is.
// because we are using linear probing, which leaves 'deleted' hash nodes
// sitting around, it is reasonable to use occupied divided by capacity. You can
// alternately use size divided by capacity, which is a more canonical
// definition of hashtable load. either will work with the unit tests.
float hash_table_load(hash_table* tbl);

// get_value returns the value associated with the provided key in the table, or
// the empty string "" if no such mapping exists (or if there is a mapping but
// it is deleted).
string get_value(hash_table* tbl, string key);

// contains returns true if the table has a non-deleted node whose hashcode
// matches the given key's hashcode.
//
// You will likely have already written the code to identify the hash node for
// this data when the get_value function is written. Consider re-using that
// code.
bool contains(hash_table* tbl, string key);

// remove marks as 'deleted' any existing non-deleted hash_node with the same
// hashcode as the provided key's hashcode. If it finds such a hash node, it
// decrements the table's size (but not occupancy) and returns true. It returns
// false otherwise.
bool remove(hash_table* tbl, string key);

// resize creates a new underlying backing array (tbl->table) with the provided
// new capacity, rehashes the existing backing array into the new array. On
// exit, the hash table's fields and functions accurately reflect the hash
// table's current state. This one is kinda tricky.
void resize(hash_table* tbl, unsigned int new_capacity);

// print_hash_table is a debugging function. It is implemented for you. Feel
// free use or to change this for your debugging needs. It is not unit tested.
//
// It might help you understand how the hash_table and hash_node structs might
// be used in your code.
void print_hash_table(hash_table* tbl);

#endif // HASH_H__
