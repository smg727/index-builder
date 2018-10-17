//
// Created by Sangram Ghuge on 10/7/18.
//
#include <vector>
#include <unordered_map>
using  namespace std;
#ifndef INDEX_BUILDER_UTILS_H
#define INDEX_BUILDER_UTILS_H

int generatePostings(unordered_map<string,string> &urlMap);
vector<string> fetchWetFilePaths();
int generateFilePosting(string, unordered_map<string,string> &urlMap);
int writeUrlMapToDisk(unordered_map<string,string> &urlMap);
int loadUrlMapFromDisk(unordered_map <string,string> &urlMap);
#endif //INDEX_BUILDER_UTILS_H

