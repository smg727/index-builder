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

struct List{

    vector<uint8_t> docIDList;
    vector<uint8_t> freqList;
    int docSize;
    int frequencySize;

};

