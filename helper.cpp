//
// Created by Sangram Ghuge on 10/11/18.
// This file contains functions to transform data form
// 1 large unix sorted posting file --> one large inverted index

#include "helper.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

struct wordPair{
public:
    int docID;
    int freq;

    wordPair(string docID, int freq){
        this->docID = stoi(docID);
        this->freq = freq;
    }
};


int createInvertedIndex(unordered_map<string,lexiconData> &lexicon){
    bool writeBinary = false;
    // sorted postings to read from
    string fileName = "sorted_posting";
    string filePath = "../unix_sorted_postings/"+fileName;
    // index
    string indexName = "index";
    string indexPath = "../inverted_index/";
    string index_frequency = "index_frequency";
    ifstream in;
    in.open(filePath);
    ofstream out;
    ofstream outFrequency;

    if(writeBinary){
        out.open(indexPath+indexName,  ios::binary | ios::out);
        outFrequency.open(indexPath+index_frequency,  ios::binary | ios::out);
    } else {
        out.open(indexPath+indexName);
        outFrequency.open(indexPath+index_frequency);
    }
    vector<wordPair> document;
    string prevWord = "0";
    string prevDoc = "0";
    string tok;
    string word;
    string docID;
    int freq = 1;
    while(in >> word  >> docID){

        if (word == prevWord) {
            if (document.empty() || stoi(docID) != document.back().docID) {
                document.push_back(wordPair(docID,freq));
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
        for (auto i = document.begin(); i != document.end(); ++i) {

            if(writeBinary){
                out.write((char *)&i->docID, sizeof(int));
                outFrequency.write((char *)&i->freq, sizeof(int));
            }
            else{
                out << i->docID << " ";
                outFrequency << i->freq << " ";
            }
        }

        if(!writeBinary){
            out<<endl;
            outFrequency<<endl;
        }
        int wordEndPosition = out.tellp();
        int freqEndPosition = outFrequency.tellp();
        lexicon.insert(pair<string,lexiconData>(prevWord,lexiconData(wordStartPosition,
                wordEndPosition,freqStartPosition,freqEndPosition)));
        prevWord = word;
        document.clear();
        document.push_back(wordPair(docID,1));
    }

    in.close();
    out.close();
    return 0;
}

int writeLexiconToDisk(unordered_map<string,lexiconData> &lexicon){
    string serializedLexicon = "Lexicon";
    string path = "../inverted_index/";
    ofstream out;
    out.open(path+serializedLexicon);

    for(auto i=lexicon.begin();i!=lexicon.end();++i){
        out<<i->first<<" "<<i->second.wordStartOffset<<" "<<i->second.wordEndOffset<<" "
        <<i->second.frequencyStartOffset<<" "<<i->second.frequencyEndOffset<<endl;
    }
    out.close();
    return 0;

}