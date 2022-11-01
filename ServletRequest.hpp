#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include "MyUtil.hpp"

using namespace std;

class ServletRequest{
    const int BEFORE_FILE;
    char* mHeader;
    char* mBody;
    unsigned char* mFile;
    vector<char*> mHeaderByLine;
    vector<char*> mBodyByLine;
    vector<char*> mBodyCharPtr;
    string mMethod;
    int mContentLength;
    string mFileName;
    string mCaption;
    string mDate;
    int mFileSize;

    vector<char*> separateLine(char* res);
    void parseHeader();
    bool findString(char *str, string cmp);
    vector<int> getLinePos(int range, char *res);
    void parseFileName();
    int parseFileInfo();
    void createFileBytes(int end);
public:
    ServletRequest(char *header);
    void parseFilePart();
    string getMethod() { return mMethod; }
    int getContentLength() { return mContentLength; }
    void setBody(char* body) { mBody = body; }
    string getFileName() { return mFileName; }
    string getCaption() { return mCaption; }
    string getDate() { return mDate; }
};
