#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <cstring>
#include "MyUtil.hpp"

using namespace std;

class ServletRequest{
    //4 - the number of lines before the actual file bytes start
    const int BEFORE_FILE;

    //parsed file bytes
    unsigned char* mFile;

    //data read from socket
    char* mHeader;
    char* mBody;
    vector<char*> mHeaderByLine;
    vector<char*> mBodyByLine;

    //info from header
    string mMethod;
    int mContentLength;

    //info from body
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
    unsigned char* getFileByte() { return mFile; }
};
