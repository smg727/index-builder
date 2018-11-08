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
        cout << "word not found" << endl;
        return -1;
    }

    list->docIDList.clear();
    list->freqList.clear();
    lexiconData wordData =lexicon.at(word);
    list->docSize = wordData.wordEndOffset-wordData.wordStartOffset;
    list->frequencySize = wordData.frequencyEndOffset - wordData.frequencyStartOffset;
    list->docCount = wordData.docCount;
    cout<< "reading " << list->docSize << " bytes from " << wordData.wordStartOffset << endl;
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


////    decompress data and test
//    vector<uint64_t> output;
//    output.resize(list->docCount);
//    cout << list->docIDList.size() << " doc list size" <<endl;
//    size_t newLen = vbyte_uncompress_sorted64(&list->docIDList[0], &output[0], 0, list->docSize);
////    size_t newLen = vbyte_uncompress_sorted64(&readTest[0], &output[0],, wordData.docCount);
//    cout << "new len " << newLen << endl;
//    cout << "doc count" << list->docCount << endl;
//    for(int i=0;i<list->docCount;i++){
//        cout << output[i]<<endl;
//    }
//    cout << "complete decompress test" << endl;
//
//    //search test
//    cout << "start search test" << endl;
//    uint64_t sol;
//    int term = 1;
//    int index = 0;
//    while(index<list->docCount){
//        cout << "index: " << index << endl << "result: " << sol << endl;
//        index = vbyte_search_lower_bound_sorted64(&list->docIDList[0],list->docSize,term,0,&sol);
//        term = sol+1;
//    }
//    cout << "end search test " << endl;




    return 0;
}

// this function return's index in the compressed array and next element in docID
// if we reach the end of the list this function will return -1 and junk
int nextGEQ(List *list, uint64_t k, uint64_t *docID){
//    vector<uint64_t> output;
//    output.resize(list->docCount);
//    cout << "geq data " << list->docCount;
//    size_t newLen = vbyte_uncompress_sorted64(&list->docIDList[0], &output[0], 0, list->docSize);
//    //size_t newLen = vbyte_uncompress_unsorted64(&docIDList[0], &output[0], wordData.docCount);
//    cout << "new len " << newLen << endl;
//    for(int i=0;i<output.size();i++){
//        cout << output[i]<<endl;
//    }
//    cout << "complete decompress test" << endl;


    size_t index = vbyte_search_lower_bound_sorted64(&list->docIDList[0],list->docSize,k,0,docID);
    if(index>=list->docCount){
        cout << "reached end of element"<<endl;
        return -1;
    }
    return index;
}

// finds the frequency for docID
// returns -1 if docID is not in the list
int getFreq(List *list, uint64_t docID){
    uint64_t result;
    size_t index = vbyte_search_lower_bound_sorted64(&list->docIDList[0],list->docSize,docID,0,&result);
    if(index>=list->docCount || docID!=result){
        cout << "docID not in list"<<endl;
        return -1;
    }
    uint64_t frequency = vbyte_select_unsorted64(&list->freqList[0],(size_t)list->frequencySize,index);
    return frequency;

}

//close list
int close(List *list){
    list->freqList.clear();
    list->docIDList.clear();
    list = NULL;
    return 0;
}











int startSearch(unordered_map<string,lexiconData> &lexicon){

    cout << "starting search " << endl;
    string searchQuery = "lagom lako";
    // TODO: clean and process query
    // TODO: remove stop words
    std::istringstream iss(searchQuery);
    std::vector<std::string> searchTerms((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());

    for(auto it = searchTerms.begin(); it!=searchTerms.end(); ++it ){
        cout << *it <<endl;

    }

    auto lp = new List[searchTerms.size()];
    auto geq = new uint64_t[searchTerms.size()];

    // set up lists
    int listLocation = 0;
    List list;
    for(auto it = searchTerms.begin(); it!=searchTerms.end(); ++it){
        int result = open(*it,lexicon, &list);
        cout << "loading list for " << *it << endl;
        if(result!=0){
            cout << "word " << *it << " does not exist in the lexicon, try another search " << endl;
            return result;
        }
        lp[listLocation] = list;
        listLocation++;
    }

    int m_c = 1;
    uint64_t test;
    int result = nextGEQ(&lp[0],1,&geq[0]);
    if(result==-1){
        cout << "reached end of list for " << searchTerms[0] << endl;
        return result;
    }
    bool eol = false;


    while(!eol){

        result = nextGEQ(&lp[m_c],geq[0],&geq[m_c]);
        if(result==-1){
            cout << "reached end of list for " << searchTerms[m_c] << endl;
            eol = true;
            continue;
        }

        if(geq[m_c]==geq[0]){

            m_c++;
            if(m_c==searchTerms.size()){
                cout << "This document matches the search terms " << geq[0] << endl;
                // TODO: process this doc
                result = nextGEQ(&lp[0],geq[0]+1,&geq[0]);
                if(result==-1){
                    cout << "reached end of list for " << searchTerms[0] << endl;
                    eol = true;
                    continue;
                }
                m_c = 1;
            }
            continue;
        }

        result = nextGEQ(&lp[0],geq[m_c],&geq[0]);
        if(result==-1){
            cout << "reached end of list for " << searchTerms[0] <<  endl;
            eol = true;
            continue;
        }
        m_c=1;
    }




    return result;

}