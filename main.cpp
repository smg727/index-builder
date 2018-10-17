#include <iostream>
#include "utils.h"
#include "helper.h"
#include <fstream>
#include <cstdlib>
int main() {
    cout << "Warning: Huge program starting" << std::endl;



    unordered_map<string,string> urlMap;
    unordered_map<string,lexiconData> lexicon;


    //generatePostings(urlMap);
    int result;
    //result = writeUrlMapToDisk(urlMap);
    //result  = loadUrlMapFromDisk(urlMap);



    // create one large posting file using unix sort
    string command = "sort --merge -k1,1 -k2n,2 ../posting_files/postings_00011.warc.wet ../posting_files/postings_00049.warc.wet -o ../unix_sorted_postings/sorted_posting";
    result = system(command.c_str());
    cout << result;

    // transform one large posting file to an inverted index
    //createInvertedIndex(lexicon);
    //result = writeLexiconToDisk(lexicon);


    // TODO: test


//    int x = 123;
//    ofstream outfile;
//    outfile.open("junk.dat", ios::binary | ios::out);
//    outfile.write((char *)&x, sizeof(int)); // sizeof can take a type
//    x = 256;
//    outfile.write((char *)&x, sizeof(int));
//
//    outfile.close();
//    int x;
//    ifstream in;
//    in.open("junk.dat", ios::binary | ios::in);
//    in.read((char*)&x, sizeof(x));
//    cout << x;
//    in.read((char*)&x, sizeof(x));
//    cout << x;


    return result;
}