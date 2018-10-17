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

    ifstream in;
    in.open(mergedPostings);
    ofstream out;
    ofstream outFrequency;
    if(writeBinary){
        out.open(indexFile,  ios::binary | ios::out);
        outFrequency.open(indexFrequencyFile,  ios::binary | ios::out);
    } else {
        out.open(indexFile);
        outFrequency.open(indexFrequencyFile);
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

    ofstream out;
    out.open(persistedLexicon);

    for(auto i=lexicon.begin();i!=lexicon.end();++i){
        out<<i->first<<" "<<i->second.wordStartOffset<<" "<<i->second.wordEndOffset<<" "
        <<i->second.frequencyStartOffset<<" "<<i->second.frequencyEndOffset<<endl;
    }
    out.close();
    return 0;

}