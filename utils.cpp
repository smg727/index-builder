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

    vector<string> wetfilePaths = fetchWetFilePaths();
    string target = "WARC-Target-URI:";
    unordered_map<int, string> urlTable;
    auto file = wetfilePaths.begin();
    int result = generateFilePosting(*file);



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
int generateFilePosting(string filePath){
    string target = "WARC-Target-URI:";
    int bufferLength = 9000;
    ifstream in;
    in.open(filePath);
    char *buffer = new char[bufferLength];

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
            urlEnd = haystack.find('\r',urlStart);
            url.append(haystack.substr(0,urlEnd));
        }

        cout << "\n\n";
        cout<< url << endl;




        contentEnd = haystack.find("\n\r\n\r\n",urlEnd);
        string content = haystack.substr(urlEnd+1,contentEnd);
        while(!in.eof() && contentEnd == string::npos){
            in.read(buffer,bufferLength);
            haystack = string(buffer);
            contentEnd = haystack.find("\r\n\r\n",urlEnd);
            content.append(haystack.substr(0,contentEnd));
        }
        cout << "content found:";
        cout << content.substr(content.length()-50) << endl;

        urlStart = contentEnd;
    }









//    while(!in.eof()){
//        in.read(buffer,bufferLength);
//        string haystack(buffer);
//        // find start of first url
//        size_t pos = haystack.find(target);
//
//        while(pos!=string::npos){
//            cout << "\nfound at";
//            size_t uriEnd = haystack.find('\r',pos);
//            cout << uriEnd << endl;
//            if(pos+18>bufferLength || uriEnd == string::npos){
//                // next url
//                pos = haystack.find(target,pos+1);
//                continue;
//            }
//            string url =  haystack.substr(pos+17,(uriEnd-1)-(pos+16));
//            cout << url << endl;
//            docID = urlTable.size();
//            urlTable[docID] = url;
//            while (){
//
//            }
//            pos = haystack.find(target,pos+1);
//        }
//
//    }

    in.close();
    return 0;
}
