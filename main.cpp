#include <iostream>
#include "utils.h"
#include <cstdlib>
#include "flags.h"
#include "vbyte.h"
#include <fstream>
#include "listOps.h"


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

    // set up flags and critical variables
    writeBinary = true;
    bufferLength = 9000;
    wetFilesPath = "../wet_files";
    persistedURLMap = "../inverted_index/UrlMap";
    mergeCommand = "sort --merge -k1,1 -k2n,2 ../posting_files/postings* -o ../unix_sorted_postings/sorted_posting";
    mergedPostings = "../unix_sorted_postings/sorted_posting";
    indexFile = "../inverted_index/index";
    indexFrequencyFile = "../inverted_index/index_frequency";
    persistedLexicon = "../inverted_index/Lexicon";

    // set up urlMap and Lexicon
    unordered_map<string,string> urlMap;
    unordered_map<string,lexiconData> lexicon;

    int result;
//    cout << "generating postings" << endl;
//    result = generatePostings(urlMap);
//    assert(result==0);
//    cout << "posting generation complete" << endl;
//
//    cout<< "writing url map to disk" << endl;
//    result = writeUrlMapToDisk(urlMap);
//    assert(result==0);
//    cout<< "writing url map to disk complete" << endl;
//
//    cout<< "loading url map from disk" << endl;
//    result  = loadUrlMapFromDisk(urlMap);
//    assert(result==0);
//    cout<< "loading url map from disk complete" << endl;
//
//    cout << "merging postings" <<endl;
//    result = system(mergeCommand.c_str());
//    assert(result==0);
//    cout << "merge complete" << endl;
//
//    cout << "creating inverted index" << endl;
//    result = createInvertedIndex(lexicon);
//    assert(result==0);
//    cout << "index creation complete" << endl;
//
//    cout << "writing lexicon to disk" << endl;
//    result = writeLexiconToDisk(lexicon);
//    assert(result==0);
//    cout << "writing lexicon to disk complete";
//ß
    cout << "reading lexicon from disk"<< endl;
    result = readLexiconFromDisk(lexicon);
    assert(result==0);
    cout << "finished loading lexicon from disk" <<endl;

//    result = startSearch(lexicon);
    vector<uint8_t> docList;
    vector<uint8_t> freqList;
    result = open("lagoenses",lexicon,docList,freqList);






    cout << "starting custom tests" << endl;

    // Everything under this is code in test
    // Not an actual part of the codebase
    // Ignore lines below this
    ofstream output;
    output.open("out", ios::binary | ios::out);
    int i = 10;
    output.write((char *)&i, sizeof(i));

    vector<uint64_t > test;
    test.push_back(5846);
    test.push_back(5847);
    test.push_back(6450);
    uint8_t out[test.size()*4];

    size_t len =  vbyte_compress_unsorted64(&test[0],&out[0],test.size());



    size_t start = output.tellp();
    output.write((char *)&out,len);
    size_t end = output.tellp();
    int j = 20;
    output.write((char *)&j, sizeof(j));
    output.close();
    cout << "read from " << start;
    ifstream input;
    input.open("out",  ios::binary | ios::in);
    input.seekg(start);
    input.read((char *)&out[0],len);
    input.close();

//    ofstream intOutput;
//    output.open("intOut", ios::out);

    cout << len << endl;
    cout << end-start << endl;
    cout << "ghuge" << endl;
    vector<uint64_t> outTest;
    outTest.resize(len*5);
    size_t newLen = vbyte_uncompress_unsorted64(&out[0], &outTest[0], len);
    cout << "new len " << newLen << endl;

    for(int i=0;i<outTest.size();i++){
        cout << outTest[i] << endl;
    }
//    uint64_t sol;
//    size_t i = vbyte_search_lower_bound_sorted64(&out[0],len,9,1,&sol);
//    cout <<"lets see"<<endl;
//    cout << i;
//    cout << sol;
//    cout << "sangram" << endl;





    return result;
}