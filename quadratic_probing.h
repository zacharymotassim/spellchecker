//Zachary Motassim
//Hash table using quadratic probing algorithm
#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

namespace {

// Internal method to test if a positive number is prime.
bool IsPrime(size_t n) {
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
int NextPrime(size_t n) {
  if (n % 2 == 0)
    ++n;
  while (!IsPrime(n)) n += 2;
  return n;
}

}  // namespace


// Quadratic probing implementation.
template <typename HashedObj>
class HashTable {
 public:

  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTable(size_t size = 101) : array_(NextPrime(size))
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

  size_t GetSize(){ //access table size from outside of class
    return array_.size();
  }
  int GetProbeCount(){//access probe count from outside of class
    return Total_Probes_;
  }

  void ResetProbeCount(){
    Total_Probes_=1;//bring probes back to its initial value of 1 to count how many probes it took to find a certain word
  }

  int GetCollisions(){//access collision count from outside of class
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
  //mutable int Probes=1;
  mutable int Total_Probes_=1;//start at 1 to account for the initial move in table

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }
//a key starts at i. if i is full we go i^2

  size_t FindPos(const HashedObj & x) const {
    size_t offset = 1;
    size_t current_pos = InternalHash(x);
    //if all of this is not true it will skip to bottom and return first hash value
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) { //check to see if position of hash is full and doesnt already contain x
      current_pos += offset;  // Compute ith probe. //move up on
      offset += 2; //add to to offset
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
    array_.resize(NextPrime(2 * old_array.size()));
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
    return hf(x) % array_.size( );
  }
};

#endif  // QUADRATIC_PROBING_H
