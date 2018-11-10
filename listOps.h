//
// Created by Sangram Ghuge on 11/2/18.
//

#ifndef INDEX_BUILDER_LISTOPS_H
#define INDEX_BUILDER_LISTOPS_H
#include <unordered_map>
#include "helper.h"
using namespace std;
#endif //INDEX_BUILDER_LISTOPS_H

int open(string word, unordered_map<string,lexiconData> &lexicon, List *list);
int startSearch(unordered_map<string,lexiconData> &lexicon, unordered_map<string,string> &urlMap);
int nextGEQ(List *list, uint64_t k, uint64_t *docID);
int getFreq(List *list, uint64_t docID);
int close(List *list);
int computeBM25(List &lp, vector<string> &searchTerms);
float computeBM25(List *lp, vector<string> &searchTerms, uint64_t docID);
void processHeap(vector<bm25URL> &heap, string url, float bm25Score);