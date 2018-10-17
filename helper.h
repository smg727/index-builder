//
// Created by Sangram Ghuge on 10/11/18.
//

#ifndef INDEX_BUILDER_HELPER_H
#define INDEX_BUILDER_HELPER_H

#include <unordered_map>
#include <string.h>
using namespace std;

#endif //INDEX_BUILDER_HELPER_H
struct lexiconData{
    int wordStartOffset;
    int wordEndOffset;
    int frequencyStartOffset;
    int frequencyEndOffset;

    lexiconData(int wordStartOffset, int wordEndOffset, int frequencyStartOffset, int frequencyEndOffset){
        this->wordStartOffset = wordStartOffset;
        this->wordEndOffset = wordEndOffset;
        this->frequencyStartOffset = frequencyStartOffset;
        this->frequencyEndOffset = frequencyEndOffset;

        return;
    }
};
int createInvertedIndex(unordered_map<string,lexiconData> &lexicon);
int writeLexiconToDisk(unordered_map<string,lexiconData> &lexicon);

