// Zachary Motassim
// spell_check.cc: A simple spell checker.
//this file implements a hashtable as a spell checker. A dictionary is read into the hash table. a transcript is then read in. Each word
//is compared to see if it is in the hashtable which is now acting as a dictionary. If it is the word is spelled correctly. if it isnt in
//our hashtable that means it is spelled wrong. From there each word will enter one of the tree cases. a) Adding one character in any possible position
//b) Removing one character from the word c) Swapping adjacent characters in the word. these cases will continuously be applied
//onje by one. after a case is applied, we check to see if it is in our dictionary. these steps will continuously be applied until the
//correct word is found
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
using namespace std;
#include "quadratic_probing.h"
//uncomment when testing double_hashing.h
//#include "c=double_hashing.h"
//uncomment when testing linear_probing.h
//#include "linear_probing.h"
using namespace std;
//create our dictionary
HashTable<string> MakeDictionary(const string &dictionary_file) {
  HashTable<string> dictionary_hash;
  // Fill dictionary_hash.
  fstream dictionary; //instance of fstream class, allow us to do operation
  string word;//will store the text word by word as we place into table
  dictionary.open(dictionary_file); //open file using class instance
  while (dictionary >> word) //spaces as delimiters
  {
      dictionary_hash.Insert(word); //insert each word in hash table
       }
  dictionary.close(); //close file to save resouces
  return dictionary_hash;
}
void CaseA(const HashTable<string>& dictionary,string &word){
    std::vector<string> corrections;//vector to hold all corrections
    if (!dictionary.Contains(word)) {
        string alphabet = "abcdefghijklmnopqrstuvwxyz";
        //cout << alphabet.size() << endl;
        string str2 = word; //temp string for manipulation
        string str_backup2 = str2;//keep original in a temp for cout
        // int stringsize=str2.size(); //size of initial string
        for (size_t i = 0; i < str2.size() + 1; i++) { //loop through current word
            int alphabetsize=alphabet.size();
            for (int k = 0; k < alphabetsize; k++) {//loop through aplhabet
                char sub = alphabet[k];//start at kth letter
                str2 = str2.substr(0, i) + sub + str2.substr(i);//insert it at ith position
                if (dictionary.Contains(str2)) {//compare
                    corrections.push_back(str2);//if it is correct put into vector
                }//if
                // cout << "After: " << str2 << endl;
                str2 = str_backup2;//reset value
            }//if it was not in dictionary we start back at the top
            //cout << endl;
        }//middle for
        // formatted output of corrections for case A
        //output corrections
        std::cout<<str_backup2<<" is INCORRECT"<<endl;
        int Corrections_Length_=corrections.size();
        for (int i = 0; i < Corrections_Length_; i++) {
          //output corrections from vector
            std::cout << "*** " << str_backup2 << " -> " << corrections[i] << " *** case A" << endl;
        }//last for
    }//first for ends
}//function ends
void CaseB(const HashTable<string>& dictionary,string &word){ //passes all cases
    std::vector<string> corrections;
    if (!dictionary.Contains(word)) {
        string str2 = word; //temp sting for manipulation and comparison
        string str_backup2 = str2; //temp string for output
        int stringsize=str2.size();
        for (int i = 0; i < stringsize; i++) {
            str2 = str2.substr(0, i) + str2.substr(i + 1); //strip character at ith index
            if (dictionary.Contains(str2)) {
                corrections.push_back(str2);//if this produces a word that is in dictionary push back into array
            }
            str2 = str_backup2; //reset word to orginal form to delete next character and compare
        }
        // formatted output of corrections for case B
        std::cout<<str_backup2<<" is INCORRECT"<<endl;
        //output corrections
        int Corrections_Length_=corrections.size();
        for (int i = 0; i < Corrections_Length_; i++) {
            std::cout << "*** " << str_backup2 << " -> " << corrections[i] << " *** case B" << endl;
        }
    }
}
void CaseC(const HashTable<string>& dictionary,string &word){ //not fully right yet
    std::vector <string> corrections;
    // if the word isn't spelt correctly, go through suggestions
    if (!dictionary.Contains(word)){ //check if current word is not in dictonary
        string str = word; //first temp string variable
        string str_backup = str; //second temp string variable
        int stringsize=str.size();
        for (int i = 0; i < (stringsize); i++) {//loop through string
            // complete a swap to test case
            swap(str[i], str[i + 1]);//swap i and i+1 character
            // if the test case is found in the dictionary, add it to the corrections suggestions
            if (dictionary.Contains(str)) {//if it contains this we push back into solutions vector
                corrections.push_back(str);
            }
            // reset to test a new case
            str = str_backup; //if it was not found we go back to our orignal word and start back at top of loop
        }
        std::cout<<str_backup<<" is INCORRECT"<<endl;
        int Corrections_Length_=corrections.size();
        for (int i = 0; i < Corrections_Length_; i++) {
            std::cout << "*** " << str_backup << " -> " << corrections[i] << " *** case C" << endl;
    }

  }
}
void SpelledCorrectly(const HashTable<string>& dictionary, string& word){
  if(dictionary.Contains(word))
  cout<<word<<" is CORRECT"<<endl;
}
// For each word in the document_file, it checks the 3 cases for a word being
// misspelled and prints out possible corrections
void SpellChecker(const HashTable<string>& dictionary,
   const string &document_file) {
        fstream file; //creating instance of fstream class
        string word;//will store word from file one at a time as we manipulate and compare
        file.open(document_file);
        while (file >> word){//space as delimiters
          int wordlength=word.size();
                      if (ispunct(word[0]))//strip punctuation from beggining or word
                      {
                          word.erase(0, 1);
                      }
                      if(ispunct(word[wordlength-1])){//strip punctuation from end of word
                        word.pop_back();
                      }
              transform(word.begin(), word.end(), word.begin(), ::tolower);//make all characters lowercase
                //call functions made above to test for cases
                  SpelledCorrectly(dictionary,word);
                   CaseA(dictionary,word);
                   CaseB(dictionary,word);
                   CaseC(dictionary,word);
        }
        file.close(); //close file to save resources
        }
int testSpellingWrapper(int argument_count, char** argument_list) {
    const string document_filename(argument_list[1]);
    const string dictionary_filename(argument_list[2]);
    // Call functions implementing the assignment requirements.
    HashTable<string> dictionary = MakeDictionary(dictionary_filename);
    //uncomment when testing linear probing
    //HashTableLinear<string> dictionary = MakeDictionary(dictionary_filename);
    //uncomment when testing double hashing
    //HashTableDouble<string> dictionary = MakeDictionary(dictionary_filename);
    //double_probing_table.R = R;
    SpellChecker(dictionary, document_filename);
    return 0;
}
// Sample main for program spell_check.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testSpellingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char** argv) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " <document-file> <dictionary-file>"
         << endl;
    return 0;
  }

  testSpellingWrapper(argc, argv);

  return 0;
}
