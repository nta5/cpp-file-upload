#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "MyUtil.hpp"

using namespace std;

class ServletRequest{
    char* mRes;
    vector<string> mResString;
    string mMethod;
    string mFileName;
    string mCaption;
    string mDate;
public:
    ServletRequest(char *header);
    void parseInfo();
    string getMethod() { return mMethod; }
    string getFileName() { return mFileName; }
    string getCaption() { return mCaption; }
    string getDate() { return mDate; }
};
