//
// Created by Sangram Ghuge on 10/11/18.
//

#ifndef INDEX_BUILDER_HELPER_H
#define INDEX_BUILDER_HELPER_H

#include <unordered_map>
#include <string>
#include <vector>
using namespace std;

#endif //INDEX_BUILDER_HELPER_H
struct lexiconData{
    int wordStartOffset;
    int wordEndOffset;
    int frequencyStartOffset;
    int frequencyEndOffset;
    int docCount;

    lexiconData(int wordStartOffset, int wordEndOffset, int frequencyStartOffset, int frequencyEndOffset , int docCount){
        this->wordStartOffset = wordStartOffset;
        this->wordEndOffset = wordEndOffset;
        this->frequencyStartOffset = frequencyStartOffset;
        this->frequencyEndOffset = frequencyEndOffset;
        this->docCount = docCount;

        return;
    }
};
int createInvertedIndex(unordered_map<string,lexiconData> &lexicon);
int writeLexiconToDisk(unordered_map<string,lexiconData> &lexicon);
int readLexiconFromDisk(unordered_map<string,lexiconData> &lexicon);
string getUrlData(const char *url);

struct List{

    vector<uint8_t> docIDList;
    vector<uint8_t> freqList;
    int docSize;
    int frequencySize;
    int docCount;

};


struct bm25URL {

    float score;
    string url;

    bm25URL(float score,string url){
        this->score = score;
        this->url = url;
    }

    friend bool operator<(const bm25URL& lhs,const bm25URL& rhs){
        return rhs.score < lhs.score;
    }


};

