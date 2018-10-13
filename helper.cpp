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
    vector<string> docIDs;
    string prevWord = "0";
    string prevDoc = "0";

    while(!in.eof()){

        in.read(buffer,bufferLength);
        stringstream lineStream;
        lineStream << buffer;
        string tok;

        int freq = 0;
        while(getline(lineStream, tok, '\n') && !lineStream.eof()) {

            size_t commaIndex = tok.find(',', 0);
            if(commaIndex==string::npos)
                continue;
            string word = tok.substr(0, commaIndex);
            string docID = tok.substr(commaIndex + 1, tok.length());

            if (word == prevWord) {
                if (docIDs.size()==0 || docID != docIDs.back()) {
                    docIDs.push_back(docID);
                }
                prevWord = word;
                continue;
            }
            out << word << " different from " << prevWord;
            out << prevWord << " ";
            for (auto i = docIDs.begin(); i != docIDs.end(); ++i) {
                out << *i << ",";
            }
            out << "\n";
            prevWord = word;
            docIDs.clear();
            docIDs.push_back(docID);
        }

    }
    in.close();
    out.close();
    return 0;
}