//
// Created by Sangram Ghuge on 10/7/18.
//
#include <sys/types.h>
#include <dirent.h>
#include "utils.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>

using namespace std;

// word, docID tuples
struct Tuple{
public:
    string word;
    int docID;

    Tuple(string wd, int dID){
        this->word = wd;
        this->docID = dID;
    }
};

// used to order tuples
bool compareTuple(Tuple a, Tuple b){
    if(a.word==b.word){
        return a.docID < b.docID;
    }
    return a.word < b.word;
}


int generatePostings() {

    unordered_map<int,string> urlMap;
    vector<string> wetfilePaths = fetchWetFilePaths();
    string target = "WARC-Target-URI:";
    unordered_map<int, string> urlTable;
    auto file = wetfilePaths.begin();
    int result = generateFilePosting(*file, urlMap);



    return  result;
}

// lists out all wet files paths
vector<string> fetchWetFilePaths() {
    vector<string> fileNames;
    string path = "../wet_files";
    DIR* dirp = opendir(path.c_str());
    struct dirent * dp;
    while ((dp = readdir(dirp)) != NULL) {
        string fileName(dp->d_name);
        if(fileName.find("warc.wet") != string::npos){
            fileNames.push_back("../wet_files/"+fileName);
        }
    }
    closedir(dirp);
    return fileNames;
}


// generates posting for a particular wet file
int generateFilePosting(string filePath, unordered_map<int,string> &urlMap){
    string target = "WARC-Target-URI:";
    int bufferLength = 9000;
    ifstream in;
    in.open(filePath);
    char *buffer = new char[bufferLength];
    vector<Tuple> postings;

    cout << "reading "+filePath;
    // jump to first url
    in.read(buffer,bufferLength);
    string haystack(buffer);
    size_t urlStart = 0;
    size_t urlEnd = 0;
    size_t contentEnd = 0;

    while(!in.eof()){
        urlStart = haystack.find(target, urlStart);

        while(!in.eof() && urlStart == string::npos){

            urlStart=0;
            in.read(buffer,bufferLength);
            haystack = string(buffer);
            urlStart = haystack.find(target, urlStart);
        }


        urlEnd = haystack.find('\r',urlStart);
        string url = haystack.substr(urlStart+target.length(),(urlEnd-1)-(urlStart+target.length()-1));
        while(!in.eof() && urlEnd == string::npos){

            in.read(buffer,bufferLength);
            haystack = string(buffer);
            urlEnd = haystack.find('\r',0);
            url.append(haystack.substr(0,urlEnd));
        }

        int docID = urlMap.size();
        urlMap.insert(pair<int,string>(docID,url));

        urlEnd = haystack.find("\r\n\r\n",urlEnd);
        while(!in.eof() && urlEnd == string::npos){
            in.read(buffer,bufferLength);
            haystack = string(buffer);
            urlEnd = haystack.find("\r\n\r\n",urlStart);
        }


        contentEnd = haystack.find("\n\r\n\r\n",urlEnd);
        string content = haystack.substr(urlEnd+1,contentEnd);
        while(!in.eof() && contentEnd == string::npos){
            in.read(buffer,bufferLength);
            haystack = string(buffer);
            contentEnd = haystack.find("\n\r\n\r\n",0);
            content.append(haystack.substr(0,contentEnd));
        }
        replace_if(content.begin() , content.end() ,
                   [] (const char& c) { return (ispunct(c)) ;},' ');

        stringstream contentStream(content); // Turn the string into a stream.
        string tok;

        while(getline(contentStream, tok, ' ')) {
            postings.push_back(Tuple(tok, docID));
        }

        urlStart = contentEnd;
    }

    cout << " sorting " << endl;
    sort(postings.begin(),postings.end(),compareTuple);
    cout << "sort complete" << endl;
    for(auto i=postings.begin();i!=postings.end();++i){
        cout << i->word << " , " << i->docID << endl;
    }

    in.close();
    return 0;
}

// unit test to see if tuple sort is working fine
void tupleTest(){

    vector<Tuple> vc;
    vc.push_back(Tuple("z",10));
    vc.push_back(Tuple("za",20));
    vc.push_back(Tuple("za",10));
    vc.push_back(Tuple("s",10));
    vc.push_back(Tuple("a",10));
    vc.push_back(Tuple("ch",30));
    vc.push_back(Tuple("b",40));
    vc.push_back(Tuple("b",10));
    sort(vc.begin(),vc.end(),compareTuple);
    for(auto i = vc.begin();i!=vc.end();++i){
        cout << i->word << " , " << i->docID << endl;
    }
    return;

}
