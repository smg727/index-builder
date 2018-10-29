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
    cout << "generating postings" << endl;
    result = generatePostings(urlMap);
    assert(result==0);
    cout << "posting generation complete" << endl;

    cout<< "writing url map to disk" << endl;
    result = writeUrlMapToDisk(urlMap);
    assert(result==0);
    cout<< "writing url map to disk complete" << endl;

    cout<< "loading url map from disk" << endl;
    result  = loadUrlMapFromDisk(urlMap);
    assert(result==0);
    cout<< "loading url map from disk complete" << endl;

    cout << "merging postings" <<endl;
    result = system(mergeCommand.c_str());
    assert(result==0);
    cout << "merge complete" << endl;

    cout << "creating inverted index" << endl;
    result = createInvertedIndex(lexicon);
    assert(result==0);
    cout << "index creation complete" << endl;

    cout << "writing lexicon to disk" << endl;
    result = writeLexiconToDisk(lexicon);
    assert(result==0);
    cout << "writing lexicon to disk complete";

    return result;
}