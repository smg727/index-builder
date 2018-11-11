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

float computeBM25(List *lp, vector<string> &searchTerms, uint64_t docID){
    float bm25 = 0;
    float k1 = 1.2;
    float k = 1.2;

    for(int i=0; i<searchTerms.size();++i){
        int ft = lp[i].docCount;
        int fDT = getFreq(&lp[i],docID);
        float firstHalf = log((totalDocCount - ft + 0.5)/(ft+0.5));
        float secondHalf = ((k1+1)*fDT)/(k+fDT);
        bm25 = bm25 + firstHalf + secondHalf;
    }
    return bm25;
}

void processHeap(vector<bm25URL> &heap, string url, float bm25Score){
    if(heap.size()<15){
        heap.push_back(bm25URL(bm25Score,url));
        push_heap(heap.begin(),heap.end());
        return;
    }

    if(bm25Score<=heap[0].score){
        return;
    }

    pop_heap(heap.begin(),heap.end());
    heap.pop_back();
    heap.push_back(bm25URL(bm25Score,url));
    push_heap(heap.begin(),heap.end());
    return;
}



int startSearch(unordered_map<string,lexiconData> &lexicon, unordered_map<string,string> &urlMap){

    cout << "starting search " << endl;
    cout << "Enter your search: " << endl;
    string searchQuery;
    getline(cin,searchQuery);
//    string searchQuery = "elephant dog";
    // TODO: clean and process query
    // TODO: remove stop words
    std::istringstream iss(searchQuery);
    std::vector<std::string> searchTerms((std::istream_iterator<std::string>(iss)),
                                     std::istream_iterator<std::string>());

    for(auto it = searchTerms.begin(); it!=searchTerms.end(); ++it ) {
        cout << *it << endl;

    }
    List *lp = new List[searchTerms.size()];
    uint64_t *geq = new uint64_t[searchTerms.size()];
    vector<bm25URL> heap;

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
//                cout << "This document matches the search terms " << geq[0] << endl;
                float bm25Score = computeBM25(lp,searchTerms,geq[0]);
//                cout << "bm25 is " << bm25Score;
                auto url = urlMap.find(to_string(geq[0]));
                if(url == urlMap.end()){
                    cout << "doc id " << geq[0] << " not found in URL map" << endl;
                } else {
//                    cout << url->second << endl;
                    processHeap(heap,url->second,bm25Score);
                }
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

    cout << "Upto top 15 URL's are " << endl;
    for(int i=0;i<heap.size();++i){
//        cout << i << " " << heap[i].url << " with score " << heap[i].score << endl;
        bool snippet = true;
        vector<string> snippets;
        string page = getUrlData(heap[i].url.c_str());
        if(!createSnippet)
            continue;
        for(int i=0;i<searchTerms.size();i++){
            size_t pos = page.find(searchTerms[i]);
            if(pos == string::npos){
                snippet = false;
                break;
            }
            snippets.push_back(page.substr(pos-20,60));
//            cout << "....." << page.substr(pos-20,60) << "...." << endl;
        }
        if(snippet){
            cout << i << " " << heap[i].url << " with score " << heap[i].score << endl << endl;
            for(auto it = snippets.begin(); it != snippets.end() ; ++it){
                cout << "...." << *it << "....";
            }
        }
        cout << endl << endl << endl;
    }

    // close all lists
    for(int i=0;i<searchTerms.size();i++){
        close(&lp[i]);
    }


    return result;

}





