#include <iostream>
#include "utils.h"
#include "helper.h"
#include <fstream>
#include <cstdlib>
#include "flags.h"

int bufferLength;
string wetFilesPath;
string persistedURLMap;
string mergeCommand;
bool writeBinary;
string mergedPostings;
string indexFile;
string indexFrequencyFile;
string persistedLexicon;

int main() {
    cout << "Warning: Huge program starting" << std::endl;

    writeBinary = true;
    bufferLength = 9000;
    wetFilesPath = "../wet_files";
    persistedURLMap = "../inverted_index/UrlMap";
    mergeCommand = "sort --merge -k1,1 -k2n,2 ../posting_files/postings* -o ../unix_sorted_postings/sorted_posting";
    mergedPostings = "../unix_sorted_postings/sorted_posting";
    indexFile = "../inverted_index/index";
    indexFrequencyFile = "../inverted_index/index_frequency";
    persistedLexicon = "../inverted_index/Lexicon";


    unordered_map<string,string> urlMap;
    unordered_map<string,lexiconData> lexicon;

    int result;
//    result = generatePostings(urlMap);
//    assert(result==0);

//    result = writeUrlMapToDisk(urlMap);
//    assert(result==0);

//    result  = loadUrlMapFromDisk(urlMap);
//    assert(result==0);
//
//    result = system(mergeCommand.c_str());
//    assert(result==0);

    result = createInvertedIndex(lexicon);
    assert(result==0);

    result = writeLexiconToDisk(lexicon);
    assert(result==0);


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