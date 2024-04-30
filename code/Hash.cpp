#include "Hash.h"

using namespace std;

// implemented for you - don't change this one
unsigned int djb2(string key) {
  unsigned int hash = 5381;
  for (size_t i = 0; i < key.length(); i++) {
    char c = key[i];
    hash = ((hash << 5) + hash) + c;
  }
  return hash;
}

// implemented for you - don't change this one
unsigned int mod_bucket_func(unsigned int hashcode, unsigned int cap) {
  unsigned int b = hashcode % cap;
  return b;
}

hash_table* init_hash_table(unsigned int cap) {
  hash_table* ret(new hash_table);
  ret->capacity = cap;
  ret->size = 0;
  ret->occupied = 0;
  ret->table = new htable(cap, nullptr);
  ret->hash_func = djb2;
  ret->bucket_func = mod_bucket_func;
  return ret;
}

hash_node* init_hash_node(string key, unsigned int hashcode, string val) {
  hash_node* ret = new hash_node;
  ret->deleted = false;
  ret->key = key;
  ret->hashcode = hashcode;
  ret->value = val;
  return ret;
}

bool set_kvp(hash_table* tbl, string key, string value) {
  unsigned int hashcode = tbl->hash_func(key);
  unsigned int index = tbl->bucket_func(hashcode, tbl->capacity);

  unsigned int original_index = index;
  while ((*tbl->table)[index]!= nullptr && ((*tbl->table)[index]->key != key || (*tbl->table)[index]->deleted)) {
    index = (index + 1) % tbl->capacity;
    if (index == original_index)
      return false;
    }
    if ((*tbl->table)[index] == nullptr) {
      (*tbl->table)[index] = init_hash_node(key, hashcode, value);
      tbl->size++;
      tbl->occupied++;
      return true;
    } else {
      (*tbl->table)[index]->value = value;
      return true;
  }
}

float hash_table_load(hash_table* tbl) {
  return static_cast<float>(tbl->occupied) / tbl->capacity;
}

string get_value(hash_table* tbl, string key) {
  unsigned int hashcode = tbl->hash_func(key);
  unsigned int index = tbl->bucket_func(hashcode, tbl->capacity);

  unsigned int original_index = index;
  while ((*tbl->table)[index]!= nullptr) {
    if ((*tbl->table)[index]->key == key && !(*tbl->table)[index]-> deleted) {
      return (*tbl->table)[index]->value;
    }
    index = (index + 1) % tbl->capacity;
    if (index == original_index)
      break;
  }
  return "";
  }

bool contains(hash_table* tbl, string key) {
  unsigned int hashcode = tbl->hash_func(key);
  unsigned int index = tbl->bucket_func(hashcode, tbl->capacity);

  unsigned int original_index = index;
  while ((*tbl->table)[index]!= nullptr) {
    if ((*tbl->table)[index]->key == key &&!(*tbl->table)[index]-> deleted) {
      return true;
    }
    index = (index + 1) % tbl->capacity;
    if (index == original_index)
      break;
  }
  return false;
}

bool remove(hash_table* tbl, string key) {
  unsigned int hashcode = tbl->hash_func(key);
  unsigned int index = tbl->bucket_func(hashcode, tbl->capacity);

  unsigned int original_index = index;
  while ((*tbl->table)[index]!= nullptr) {
    if ((*tbl->table)[index]->key == key &&!(*tbl->table)[index]-> deleted) {
      (*tbl->table)[index]->deleted = true;
      tbl->size--;
      return true;
    }
    index = (index + 1) % tbl->capacity;
    if (index == original_index)
      break;
  }
  return false;
}

void resize(hash_table* tbl, unsigned int new_capacity) {
  htable* new_table = new htable(new_capacity, nullptr);

  for(unsigned int i = 0; i < tbl->capacity; i++) {
    if ((*tbl->table)[i] != nullptr && !(*tbl->table)[i]->deleted) {
      unsigned int hashcode = (*tbl->table)[i]->hashcode;
      unsigned int index = tbl->bucket_func(hashcode, new_capacity);

      while ((*new_table)[index]!= nullptr) {
        index = (index + 1) % new_capacity;
      }
      (*new_table)[index] = (*tbl->table)[i];
    }
  }
  delete tbl->table;
  tbl->table = new_table;
  tbl->capacity = new_capacity;
  tbl->occupied = tbl->size;
}

// implemented for you - feel free to change this one if you like
void print_hash_table(hash_table* tbl) {
  cout << "Hashtable:" << endl;
  cout << "  capacity: " << tbl->capacity << endl;
  cout << "  size:     " << tbl->size << endl;
  cout << "  occupied: " << tbl->occupied << endl;
  cout << "  load:     " << hash_table_load(tbl) << endl;
  if (tbl->capacity < 130) {
    for (unsigned int i = 0; i < tbl->capacity; i++) {
      cout << "[" << i << "]    ";
      if (!tbl->table->at(i)) {
        cout << "<empty>" << endl;
      } else if (tbl->table->at(i)->deleted) {
        cout << "<deleted>" << endl;
      } else {
        cout << "\"" << tbl->table->at(i)->key << "\" = \""
             << tbl->table->at(i)->value << "\"" << endl;
      }
    }
  } else {
    cout << "    <hashtable too big to print out>" << endl;
  }
}
