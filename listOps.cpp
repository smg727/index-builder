//
// Created by Sangram Ghuge on 11/2/18.
//

#include "helper.h"
#include <fstream>
#include <vector>
#include <sstream>
#include "flags.h"
#include <iostream>
#include "vbyte.h"

using namespace std;


int open(string word, unordered_map<string,lexiconData> &lexicon, vector<uint8_t> &docIDList, vector<uint8_t> &freqList){

    if(lexicon.find(word) == lexicon.end()){
        cout << "word not found";
        return -1;
    }
    docIDList.clear();
    freqList.clear();
    lexiconData wordData =lexicon.at(word);
    int docIDSize = wordData.wordEndOffset-wordData.wordStartOffset;
    int freqSize = wordData.frequencyEndOffset - wordData.frequencyStartOffset;
    cout<< "reading" << docIDSize << "bytes from " << wordData.wordStartOffset << endl;
    docIDList.resize(docIDSize);
    freqList.resize(freqSize);

    ifstream freqInput;
    freqInput.open(indexFrequencyFile,  ios::binary | ios::in);
    ifstream docIDInput;
    docIDInput.open(indexFile,  ios::binary | ios::in);

    // read data
    docIDInput.seekg(wordData.wordStartOffset);
    freqInput.read((char *)&freqList[0],freqSize);
    docIDInput.read((char *)&docIDList[0],docIDSize);

    freqInput.close();
    docIDInput.close();

    //decompress data
    vector<uint64_t> output;
    output.resize(wordData.docCount);
    cout << docIDList.size() << " doc list size" <<endl;
//    size_t newLen = vbyte_uncompress_sorted64(&docList[0], &output[0], 0, docList.size());
    size_t newLen = vbyte_uncompress_unsorted64(&docIDList[0], &output[0], wordData.docCount);
    cout << "new len " << newLen << endl;
    for(int i=0;i<output.size();i++){
        cout << output[i]<<endl;
    }
    cout << "complete" << endl;


    return 0;
}

int startSearch(unordered_map<string,lexiconData> &lexicon){

    // test code
    vector<uint8_t> docList;
    vector<uint8_t> freqList;
    int result = open("lagom",lexicon,docList,freqList);

    uint64_t *output = new uint64_t[docList.size()];
    cout << docList.size() << "doc list size" <<endl;
//    size_t newLen = vbyte_uncompress_sorted64(&docList[0], &output[0], 0, docList.size());
    size_t newLen = vbyte_uncompress_unsorted64(&docList[0], &output[0], docList.size());
    cout << "new len" << newLen << endl;
    for(int i=0;i<newLen;i++){
        cout << output[i]<<endl;
    }


    return 0;
}