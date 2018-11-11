//
// Created by Sangram Ghuge on 10/11/18.
// This file contains functions to transform data form
// 1 large unix sorted posting file --> one large inverted index

#include "helper.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include "flags.h"
#include <iostream>
#include "vbyte.h"
#include <curl/curl.h>

using namespace std;

// (docID,freq) tuple.
// This is used to simplify the algorithm to track which docID has what frequency
struct wordPair{
public:
    int docID;
    int freq;

    wordPair(string docID, int freq){
        this->docID = stoi(docID);
        this->freq = freq;
    }
};

// create the inverted index
int createInvertedIndex(unordered_map<string,lexiconData> &lexicon){

    vector<uint64_t> test;
    test.push_back(1);
    test.push_back(2);
    test.push_back(3);
    test.push_back(4);
    uint8_t outt[512];
    size_t len =  vbyte_compress_sorted64(&test[0],&outt[0],1,test.size());

    for(int i=0;i<len;i++){
        cout << outt[i] << endl;
    }

    ifstream in;
    in.open(mergedPostings);
    ofstream out;
    ofstream outFrequency;
    // write binary if the writeBinary flag is true
    if(writeBinary){
        out.open(indexFile,  ios::binary | ios::out);
        outFrequency.open(indexFrequencyFile,  ios::binary | ios::out);
    } else {
        out.open(indexFile);
        outFrequency.open(indexFrequencyFile);
    }
//    vector<wordPair> document;
    vector<uint64_t> docIDList;
    vector<uint64_t> freqList;
    string prevWord = "0";
    string prevDoc = "0";
    string tok;
    string word;
    string docID;
    int freq = 1;

    while(in >> word  >> docID){

        if (word == prevWord) {
            if (docIDList.empty() || stoi(docID) != docIDList.back()) {
                docIDList.push_back(stoi(docID));
                freqList.push_back(freq);
                freq = 1;
            } else {
                freq++;
            }
            prevWord = word;
            continue;
        }
        int wordStartPosition = out.tellp();
        int freqStartPosition = outFrequency.tellp();
        //out << prevWord << " ";
        uint8_t compressedDocID[docIDList.size()*4];
        uint8_t compressedFreq[freqList.size()*4];
        size_t docLen = vbyte_compress_sorted64(&docIDList[0],&compressedDocID[0],0,docIDList.size());
        //size_t docLen = vbyte_compress_unsorted64(&docIDList[0],&compressedDocID[0],docIDList.size());
        size_t freqLen = vbyte_compress_unsorted64(&freqList[0],&compressedFreq[0],freqList.size());


        out.write((char *)&compressedDocID,docLen);
        outFrequency.write((char *)&compressedFreq,freqLen);


        int wordEndPosition = out.tellp();
        int freqEndPosition = outFrequency.tellp();
        // store byte offsets into lexicon
        lexicon.insert(pair<string,lexiconData>(prevWord,lexiconData(wordStartPosition,
                wordEndPosition,freqStartPosition,freqEndPosition,docIDList.size())));
        prevWord = word;
        docIDList.clear();
        freqList.clear();
        docIDList.push_back(0);
        freqList.push_back(0);
        docIDList.push_back(stoi(docID));
        freqList.push_back(1);
    }

    in.close();
    out.close();
    return 0;
}

// persists the lexicon by writing it to disk 
int writeLexiconToDisk(unordered_map<string,lexiconData> &lexicon){

    ofstream out;
    out.open(persistedLexicon);

    for(auto i=lexicon.begin();i!=lexicon.end();++i){
        out<<i->first<<" "<<i->second.wordStartOffset<<" "<<i->second.wordEndOffset<<" "
        <<i->second.frequencyStartOffset<<" "<<i->second.frequencyEndOffset<<" "<<i->second.docCount<<endl;
    }
    out.close();
    return 0;

}

int readLexiconFromDisk(unordered_map<string,lexiconData> &lexicon){

    ifstream in;
    in.open(persistedLexicon);
    string word;
    string indexStart;
    string indexEnd;
    string freqStart;
    string freqEnd;
    string docCount;
    while(in>> word >> indexStart >> indexEnd >> freqStart >> freqEnd >> docCount){
        lexicon.insert(pair<string,lexiconData>(word,lexiconData(stoi(indexStart),
                                                                 stoi(indexEnd),stoi(freqStart),stoi(freqEnd),stoi(docCount))));

    }
    return 0;
}

string data;
size_t writeCallback(char* buf, size_t size, size_t nmemb, void* up) {
    //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer

    for (int c = 0; c<size*nmemb; c++)
    {
        data.push_back(buf[c]);
    }
    return size*nmemb; //tell curl how many bytes we handled
}

string getUrlData(const char *url){

    CURL* curl; //our curl object

    curl_global_init(CURL_GLOBAL_ALL); //pretty obvious
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &writeCallback);
   // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); //tell curl to output its progress
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);


    curl_easy_perform(curl);


    curl_easy_cleanup(curl);
    curl_global_cleanup();

    string tmp = data;
    data = "";
    return tmp;;
}

