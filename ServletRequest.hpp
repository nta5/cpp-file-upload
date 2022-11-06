#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include <time.h>
#include "MyUtil.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;

class ServletRequest{
    //4 - the number of lines before the actual file bytes start
    const int BEFORE_FILE;

    //parsed file bytes
    unsigned char* mFile;

    //data read from socket
    char* mReqeust;
    char* mHeader;
    char* mBody;
    vector<char*> mRequestByLine;
    vector<char*> mHeaderByLine;
    vector<char*> mBodyByLine;

    //info from header
    string mMethod;
    int mContentLength;

    //info from body
    string mFileName;
    string mCaption;
    string mDate;
    size_t mFileSize;

    vector<char*> separateLine(char* res);
    void parseRequest();
    void parseHeader();
    bool findString(char *str, string cmp);
    vector<int> getLinePos(int range, char *res);
    void parseFileName();
    int parseFileInfo();
    void createFileBytes(int end);
public:
    ServletRequest(char *req, int request_length);
    void parseFilePart();
    const string currentDateTime();
    string getMethod() { return mMethod; }
    int getContentLength() { return mContentLength; }
    void setBody(char* body) { mBody = body; }
    string getFileName() { return mFileName; }
    string getCaption() { return mCaption; }
    string getDate() { return mDate; }
    size_t getFileSize() { return mFileSize; }
    unsigned char* getFileByte() { return mFile; }

};
