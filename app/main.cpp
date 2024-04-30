#include "../code/Hash.h"
#include <bitset>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

unsigned int annotated_djb2(string key) {
  unsigned int hash = 5381;
  cout << "Hashing string \"" << key << "\"" << endl;
  cout << bitset<32>(hash) << " " << hash << endl;
  for (size_t i = 0; i < key.length(); i++) {
    char c = key[i];
    hash = ((hash << 5) + hash) + c;
    cout << bitset<32>(hash) << " " << hash << endl;
  }
  cout << "Hashed string \"" << key << "\" to " << hash << endl;
  //
  // for light reading on why djb2 might not be the best
  // production-grade hash function, go to
  // http://dmytry.blogspot.com/2009/11/horrible-hashes.html
  return hash;
}

string mkstring(string label, int num) {
  std::ostringstream k;
  k << label << " " << num;
  return k.str();
}

int main() {
  cout << "For 'fun', let's hash some text and see the bit patterns " << endl
       << "produced by an annotated form of the djb2 hash function:" << endl
       << endl;
  string words = "Excellent words";
  annotated_djb2(words);
  cout << endl << endl;
  if (false) { // set to true to have the following print
    auto tbl = init_hash_table(16);
    print_hash_table(tbl);
    set_kvp(tbl, "fantastic key", "value for fantastic key");
    print_hash_table(tbl);
    set_kvp(tbl, "fantastic key", "value for the second fantastic key");
    print_hash_table(tbl);
    for (int i = 0; i < 15; i++) {
      string k = mkstring("some key", i);
      string v = mkstring("some value", i);
      set_kvp(tbl, k, v);
      print_hash_table(tbl);
    }
  }
}
