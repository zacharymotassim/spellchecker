//zachary motassim
//Hash table using double hashing algorithm
#ifndef DOUBLE_HASHING_H
#define DOUBLE_HASHING_H

#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

namespace {

// Internal method to test if a positive number is prime.
bool IsPrimeDouble(size_t n) {
  if( n == 2 || n == 3 )
    return true;

  if( n == 1 || n % 2 == 0 )
    return false;

  for( int i = 3; i * i <= n; i += 2 )
    if( n % i == 0 )
      return false;

  return true;
}


// Internal method to return a prime number at least as large as n.
int NextPrimeDouble(size_t n) {
  if (n % 2 == 0)
    ++n;
  while (!IsPrimeDouble(n)) n += 2;
  return n;
}

}  // namespace


// Quadratic probing implementation.
template <typename HashedObj>
class HashTableDouble {
 public:
   int R=0; //R value is a public function

  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableDouble(size_t size = 101) : array_(NextPrimeDouble(size))
    { MakeEmpty(); }

  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }

  void MakeEmpty() {
    current_size_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;

    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;


    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();
    return true;
  }

  bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x);
    if (IsActive(current_pos))
      return false;

    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;
    //Amount_Of_Elements=Amount_Of_Elements+1;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }

  int GetProbeCount(){
    return Total_Probes_;
  }

  void ResetProbeCount(){
    Total_Probes_=1;
  }

  size_t GetSize(){
    return array_.size();
  }

  int GetCollisions(){
    return Total_Collisions_;
  }

 private:
  struct HashEntry {
    HashedObj element_;
    EntryType info_;

    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }

    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
  std::vector<HashEntry> array_;
  size_t current_size_;
  //static int Amount_Of_Elements;
  mutable int Total_Collisions_=0;
  mutable int Total_Probes_=1;

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }
//a key starts at i. if i is full we go i^2

  size_t FindPos(const HashedObj & x) const {
    size_t offset = SecondHash(x);
    //size_t offset = R- x % R;
    size_t current_pos = InternalHash(x);
    //if all of this is not true it will skip to bottom and return first hash value
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) { //check to see if position of hash is full and doesnt already contain x
      current_pos += offset;  // Compute ith probe. //move up on
      if (current_pos >= array_.size())//if the current position is greater than size of array we subtract length of array
	current_pos -= array_.size();
  Total_Collisions_++;
  Total_Probes_++;
    }
    return current_pos;
    //this will keep happening till empty spot is found
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrimeDouble(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;

    // Copy table over.
    current_size_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }

  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size();
  }
  //second hash function
  size_t SecondHash(const HashedObj & x) const {
  static std::hash<HashedObj> hf;
  return R - hf(x) % R;
}
};

#endif  // QUADRATIC_PROBING_H
