//
// Created by Sangram Ghuge on 11/2/18.
//

#ifndef INDEX_BUILDER_LISTOPS_H
#define INDEX_BUILDER_LISTOPS_H
#include <unordered_map>
#include "helper.h"
using namespace std;
#endif //INDEX_BUILDER_LISTOPS_H

//int open(string word, unordered_map<string,lexiconData> &lexicon, vector<uint8_t> &docIDList, vector<uint8_t> &freqList,
//        int *docSize, int *frequencySize);
int open(string word, unordered_map<string,lexiconData> &lexicon, List list);
int startSearch(unordered_map<string,lexiconData> &lexicon);