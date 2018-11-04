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


int open(string word, unordered_map<string,lexiconData> &lexicon, List *list){
    if(lexicon.find(word) == lexicon.end()){
        cout << "word not found";
        return -1;
    }

    list->docIDList.clear();
    list->freqList.clear();
    lexiconData wordData =lexicon.at(word);
    list->docSize = wordData.wordEndOffset-wordData.wordStartOffset;
    list->frequencySize = wordData.frequencyEndOffset - wordData.frequencyStartOffset;
    list->docCount = wordData.docCount;
    cout<< "reading" << list->docSize << "bytes from " << wordData.wordStartOffset << endl;
    list->docIDList.resize(list->docSize);
    list->freqList.resize(list->frequencySize);

    ifstream freqInput;
    freqInput.open(indexFrequencyFile,  ios::binary | ios::in);
    ifstream docIDInput;
    docIDInput.open(indexFile,  ios::binary | ios::in);

    // read data
    docIDInput.seekg(wordData.wordStartOffset);
    freqInput.seekg(wordData.frequencyStartOffset);
    freqInput.read((char *)&list->freqList[0],list->frequencySize);
    docIDInput.read((char *)&list->docIDList[0],list->docSize);


    freqInput.close();
    docIDInput.close();


//    decompress data and test
    vector<uint64_t> output;
    output.resize(list->docCount);
    cout << list->docIDList.size() << " doc list size" <<endl;
    size_t newLen = vbyte_uncompress_sorted64(&list->docIDList[0], &output[0], 0, list->docSize);
//    size_t newLen = vbyte_uncompress_sorted64(&readTest[0], &output[0],, wordData.docCount);
    cout << "new len " << newLen << endl;
    cout << "doc count" << list->docCount << endl;
    for(int i=0;i<list->docCount;i++){
        cout << output[i]<<endl;
    }
    cout << "complete decompress test" << endl;

    //search test
    cout << "start search test" << endl;
    uint64_t sol;
    int term = 1;
    int index = 0;
    while(index<list->docCount){
        cout << "index: " << index << endl << "result: " << sol << endl;
        index = vbyte_search_lower_bound_sorted64(&list->docIDList[0],list->docSize,term,0,&sol);
        term = sol+1;
    }
    cout << "end search test " << endl;




    return 0;
}

// this function return's 0 and the next docID in docID
// if we reach the end of the list this function will return -1
int nextGEQ(List *list, uint64_t k, int *docID){
    vector<uint64_t> output;
    output.resize(list->docCount);
    cout << "geq data " << list->docCount;
    size_t newLen = vbyte_uncompress_sorted64(&list->docIDList[0], &output[0], 0, list->docSize);
    //size_t newLen = vbyte_uncompress_unsorted64(&docIDList[0], &output[0], wordData.docCount);
    cout << "new len " << newLen << endl;
    for(int i=0;i<output.size();i++){
        cout << output[i]<<endl;
    }
    cout << "complete decompress test" << endl;

    uint64_t sol;
    size_t index = vbyte_search_lower_bound_sorted64(&list->docIDList[0],list->docSize,1,0,&sol);
    cout << index << "  " << sol << endl;
    if(index>=list->docCount){
        cout << index << endl;
        return -1;
    }
    return 0;
}











int startSearch(unordered_map<string,lexiconData> &lexicon){

//    // test code
//    vector<uint8_t> docList;
//    vector<uint8_t> freqList;
//    int result = open("lagom",lexicon,docList,freqList);
//
//    uint64_t *output = new uint64_t[docList.size()];
//    cout << docList.size() << "doc list size" <<endl;
////    size_t newLen = vbyte_uncompress_sorted64(&docList[0], &output[0], 0, docList.size());
//    size_t newLen = vbyte_uncompress_unsorted64(&docList[0], &output[0], docList.size());
//    cout << "new len" << newLen << endl;
//    for(int i=0;i<newLen;i++){
//        cout << output[i]<<endl;
//    }
//
//
//    return 0;
}