//
// Created by Sangram Ghuge on 10/7/18.
//
#include <vector>
#include <unordered_map>
using  namespace std;
#ifndef INDEX_BUILDER_UTILS_H
#define INDEX_BUILDER_UTILS_H

int generatePostings();
vector<string> fetchWetFilePaths();
int generateFilePosting(string, unordered_map<int,string> &urlMap);
void tupleTest();
#endif //INDEX_BUILDER_UTILS_H

