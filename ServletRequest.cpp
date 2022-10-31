#include "ServletRequest.hpp"

//DONE
ServletRequest::ServletRequest(char *header) : mHeader(header), mContentLength(-1){
    parseHeader();
}

//DONE
vector<char*> ServletRequest::separateLine(char* res) {
    cout << "content length:" << mContentLength << endl;
    vector<char*> result;
    vector<int> newLinePos;
    newLinePos.push_back(0);
    int range = mContentLength == -1 ? strlen(res) : mContentLength;
    for(int i = 0; i < range; ++i){
        if(res[i] == '\n') newLinePos.push_back(i);
    }

    for(int i = 0; i < newLinePos.size() - 1; ++i){
        int beg = newLinePos[i];
        int end = newLinePos[i + 1];
        int size = end - beg + 1;
        char* line = new char[size];
        int index = 0;
        for(int j = beg; j < end; j++){
            line[index] = res[j];
            index++;
        }
        result.push_back(line);
    }

    cout << "separate line result at 0:"<< result.at(0) << ":" << endl;
    cout << "parsed size:" << result.size() << ":" << endl;

    return result;
}

//DONE
void ServletRequest::parseHeader() {
    mHeaderByLine = separateLine(mHeader);
    const int CONTENT_POS = 3;

    string method = mHeaderByLine.at(0);
    mMethod = MyUtil::myTrim(method.substr(0, method.find('/')));

    if(mMethod == "POST"){
        string content = mHeaderByLine.at(CONTENT_POS);
        string length = MyUtil::myTrim(content.substr(content.find(':') + 1, content.length()));
        mContentLength = stoi(length);
    }
}
//
//if (s1.find(s2) != std::string::npos) {
//std::cout << "found!" << '\n';
//}

//Content-Disposition: form-data; name="fileName"; filename="shower.png"

//std::string s = "scott>=tiger";
//std::string delimiter = ">=";
//std::string token = s.substr(0, s.find(delimiter)); // token is "scott"

void ServletRequest::parseFilePart() {
    mBodyByLine = separateLine(mBody);
    int count = 1;
//    while(count < mBodyByLine.size()){
        //file name
        string delimiter = "filename=\"";
        string line = mBodyByLine.at(count);
        int beg = line.find(delimiter) + delimiter.length();
        int range = line.size() - beg - 2;
        mFileName = line.substr(beg, range);

        count = 4;
        string boundary = "------WebKitFormBoundary";
        int begPos = 0;
        for(int i = 0; i <= 3; ++i){
            begPos += strlen(mBodyByLine.at(i)) + 1;
        }

        cout << "beg position:" << begPos << ":" << endl;

        mFile = new unsigned char[mContentLength - begPos];
        unsigned char *temp = new unsigned char [mContentLength - begPos];
        vector<int> newLinePos;
        int endPos = begPos;
        int index = 0;
        int lineCount = 0;
        while(endPos < mContentLength - 1){
            temp[index] = mBody[endPos];
            if(mBody[endPos] == '\n'){
                ++lineCount;
                newLinePos.push_back(endPos);
            }
            ++index;
            ++endPos;
        }
        cout << "line count:" << lineCount << ":" << endl;
        cout << "end position:" << endPos << ":" << endl;

//        cout << "mBodyByLine size:" << mBodyByLine.size() << ":::" << endl;
//        cout << "after while count:" << count << ":::" << endl;
//
//        mFile = new char[endPos - begPos];
//        for(int j = begPos; j < endPos; j++){
//            mFile[j] = mBody[j];
//        }
//
//        count += 3;
//        mCaption = mBodyByLine.at(count);
//        cout << "parse caption:" << mCaption << ":::::::" << endl;
//
//        count += 4;
//        mDate = mBodyByLine.at(count);
//        cout << "parse date:" << mDate << ":::::::" << endl;

//    ------WebKitFormBoundarySPlIEtoR9hTqeXjZ
//    Content-Disposition: form-data; name="caption"
//
//    wefw
//    ------WebKitFormBoundarySPlIEtoR9hTqeXjZ
//    Content-Disposition: form-data; name="date"
//
//    2022-04-18
//            ------WebKitFormBoundarySPlIEtoR9hTqeXjZ--

//    }
}

bool findBoundary(char* str){
    string boundary = "------WebKitFormBoundary";
    char temp[boundary.length() + 1];
    strcpy(temp, boundary.c_str());
    int i=0;
    int j=0;


    while(str[i]!='\0'){
        if(str[i] == temp[j])
        {
            while (str[i] == temp[j] && str[j]!='\0')
            {
                j++;
                i++;
            }
            if(temp[j]=='\0'){
                return true;
            }
            j=0;
        }
        i++;
    }
    return false;
}

