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
//
//    result = startSearch(lexicon);
    List list;
    result = open("lagom",lexicon,&list);
    uint64_t output = 0;
    uint64_t term = 1;
    int index = 0;
    while(index>=0){
        cout << term << output << index << endl;
        index = nextGEQ(&list,term,&output);
        term = output+1;
    }
//    index = nextGEQ(&list,term,&output);
//    cout << index << output <<"term "<< term << endl;
//    term = output+1;
//    cout << index << output <<"term "<< term << endl;
//    index = nextGEQ(&list,term,&output);
//    cout << index << output << term <<endl;
//    term = output+1;
//    index = nextGEQ(&list,term,&output);
//    cout << index << output << term << endl;
//    term = output+1;
//    index = nextGEQ(&list,term,&output);
//    cout << index << output << term <<  endl;
//    term = output+1;





    cout << "starting custom tests" << endl;

    // Everything under this is code in test
    // Not an actual part of the codebase
    // Ignore lines below this
//    ofstream output;
//    output.open("out", ios::binary | ios::out);
//    int i = 10;
//    output.write((char *)&i, sizeof(i));
//
//    vector<uint64_t > test;
//    test.push_back(1);
//    test.push_back(3);
//    test.push_back(5);
//    test.push_back(7);
//    test.push_back(9);
//    test.push_back(100);
//
//    uint8_t out[test.size()*4];
//
//    size_t len =  vbyte_compress_sorted64(&test[0],&out[0],0,test.size());
//
//
//
//    size_t start = output.tellp();
//    output.write((char *)&out,len);
//    size_t end = output.tellp();
//    int j = 20;
//    output.write((char *)&j, sizeof(j));
//    output.close();
//    cout << "read from " << start;
//    ifstream input;
//    input.open("out",  ios::binary | ios::in);
//    input.seekg(start);
//    input.read((char *)&out[0],len);
//    input.close();
//
//    ofstream intOutput;
//    output.open("intOut", ios::out);
//
//    cout << len << endl;
//    cout << end-start << endl;
//    cout << "ghuge" << endl;
//    vector<uint64_t> outTest;
//    outTest.resize(end-start);
//    size_t newLen = vbyte_uncompress_sorted64(&out[0], &outTest[0], 0,len);
//    cout << "new len " << newLen << endl;
//
//    for(int i=0;i<outTest.size()+3;i++){
//        cout << outTest[i] << endl;
//    }
//    uint64_t sol;
//    size_t k = vbyte_search_lower_bound_sorted64(&out[0],len,101,0,&sol);
//    cout <<"lets see"<<endl;
//    cout << k << endl;
//    cout << sol << endl;
//    cout << (size_t)-1 << endl;
//    cout << "sangram" << endl;







    return result;
}