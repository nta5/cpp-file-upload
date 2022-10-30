#include "ServletRequest.hpp"

ServletRequest::ServletRequest(char *res) : mRes(res){
    stringstream ss(res);
    string line;
    while(getline(ss, line, '\n')){
        mResString.push_back(line);
    }

    printf("%s\n", res);
    printf("%lu\n", strlen(res));

    parseInfo();
}

void ServletRequest::parseInfo() {
    stringstream ss(mResString.at(0));
    string temp;
    getline(ss, temp, '/');
    mMethod = MyUtil::myTrim(temp);
}
