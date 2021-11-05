//Zachary Motassim
//This file tests the performance of our hashing algorithms. We read in two files, one to be stored
//in our hash tableby word and the other to read in word by word and see if our hashtable Contains
//the current word being read in. We also keep track of statistics on the hashtable being used.
//these statistics are Thetotalnumberofelementsinthetable(N),thesizeofthetable(T),theloadfactor (N/T),
//the total number of collisions (C), and the average number of collisions (C/N).
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
// Uncomment when you implemented linear probing & double hashing
#include "linear_probing.h"
#include "double_hashing.h"
#include "quadratic_probing.h"
using namespace std;

template <typename HashTableType>
void TestFunctionForHashTable(HashTableType &hash_table,
			      const string &words_filename,
			      const string &query_filename) {
							hash_table.MakeEmpty();
							float number_of_elements_ = 0.0;
							fstream file; //instance of fstream class, allow us to do operation
							string word;//will store the text word by word as we place into table
							file.open(words_filename);
							while (file >> word) //spaces as delimiters
							{
									hash_table.Insert(word);
									number_of_elements_=number_of_elements_+1;
							     }
							file.close(); //close file when words are no longer being read in
							//table statistics
				float Load_Factor_ = 0.0;
				float Average_collisions_ = 0.0;
			  cout<<"number_of_elements: "<<number_of_elements_<<endl;
				cout<<"size_of_table: "<<hash_table.GetSize()<<endl;
				Load_Factor_= (number_of_elements_ / hash_table.GetSize());
				cout<<"load_factor: "<<Load_Factor_<<endl;
				Average_collisions_ = (hash_table.GetCollisions() / number_of_elements_);
				cout<<"average_collisions: "<<Average_collisions_<<endl;
				cout<<"total_collisions: "<<hash_table.GetCollisions()<<endl;
				//finding words and outputting the amount of probes along the hash table it took to find it
				cout<<endl;
				hash_table.ResetProbeCount();
		fstream qfile; //instance of fstream class, allow us to do operation
		string qword;//will store the text word by word as we read
		//size_t probes=1;
		qfile.open(query_filename);
		while (qfile >> qword) //spaces as delimiters
		{
			if(hash_table.Contains(qword)){
			cout<< qword<< " Found "<<hash_table.GetProbeCount()<<endl;
		}
		else{
			cout<<qword<< " Not_found "<<hash_table.GetProbeCount()<<endl;
		}
					hash_table.ResetProbeCount();//reset probe caount here to start count of new word.
		}
qfile.close();
}
// @argument_count: argc as provided in main
// @argument_list: argv as provided in imain
// Calls the specific testing function for hash table (linear, quadratic, or double).
int testHashingWrapper(int argument_count, char **argument_list) {
    const string words_filename(argument_list[1]);
    const string query_filename(argument_list[2]);
    const string param_flag(argument_list[3]);
    int R = 89;
    if (argument_count == 5) {
	const string rvalue(argument_list[4]);
	R = stoi(rvalue);
    }
    if (param_flag == "linear") {
      // Uncomment below when you have implemented linear probing.
      HashTableLinear<string> linear_probing_table;
      TestFunctionForHashTable(linear_probing_table, words_filename,
       			 query_filename);
    } else if (param_flag == "quadratic") {
	HashTable<string> quadratic_probing_table;
	TestFunctionForHashTable(quadratic_probing_table, words_filename,
				 query_filename);
    } else if (param_flag == "double") {
	cout << "r_value: " << R << endl;
       //Uncomment below when you have implemented double hashing.
	HashTableDouble<string> double_probing_table;
	double_probing_table.R = R;
	TestFunctionForHashTable(double_probing_table, words_filename,
	 			 query_filename);
    } else {
	cout << "Unknown tree type " << param_flag
	     << " (User should provide linear, quadratic, or double)" << endl;
    }
    return 0;
}
// Sample main for program create_and_test_hash. DO NOT CHANGE IT.
// WE WILL NOT USE YOUR MAIN IN TESTING. DO NOT CODE FUNCTIONALITY INTO THE
// MAIN. WE WILL DIRECTLY CALL testHashingWrapper. ALL FUNCTIONALITY SHOULD BE
// THERE. This main is only here for your own testing purposes.
int main(int argc, char **argv) {
    if (argc != 4 and argc != 5) {
	cout << "Usage: " << argv[0]
	     << " <wordsfilename> <queryfilename> <flag>" << endl;
	cout << "or Usage: " << argv[0]
	     << " <wordsfilename> <queryfilename> <flag> <rvalue>" << endl;
	return 0;
    }
    testHashingWrapper(argc, argv);
    return 0;
}
