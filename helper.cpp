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
    string docID;
    string freq;

    wordPair(string docID, int freq){
        this->docID = docID;
        this->freq = to_string(freq);
    }
};

int createInvertedIndex(){

    string fileName = "sorted_posting";
    int bufferLength = 9000;
    string filePath = "../unix_sorted_postings/"+fileName;
    string indexName = "index";
    string indexPath = "../inverted_index/";
    ifstream in;
    in.open(filePath);
    ofstream out;
    out.open(indexPath+indexName);
    char *buffer = new char[bufferLength];
    vector<wordPair> document;
    string prevWord = "0";
    string prevDoc = "0";
    string tok;
    string word;
    string docID;
    int freq = 1;
    while(in >> word  >> docID){

        if (word == prevWord) {
            if (document.size()==0 || docID != document.back().docID) {
                document.push_back(wordPair(docID,freq));
                freq = 1;
            } else {
                freq++;
            }
            prevWord = word;
            continue;
        }
        //out << word << " different from " << prevWord;
        out << prevWord << " ";
        for (auto i = document.begin(); i != document.end(); ++i) {
            out << ":" << i->docID << "," << i->freq;
        }
        out << ":\n";
        prevWord = word;
        document.clear();
        document.push_back(wordPair(docID,1));
    }

    in.close();
    out.close();
    return 0;
}