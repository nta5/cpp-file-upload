#include "ServletRequest.hpp"

ServletRequest::ServletRequest(char *header) : mHeader(header), mContentLength(-1), BEFORE_FILE(4){
    parseHeader();
}

//parse info from header
void ServletRequest::parseHeader() {
    mHeaderByLine = separateLine(mHeader);
    const int CONTENT_POS = 3;

    string method = mHeaderByLine.at(0);
    mMethod = MyUtil::myTrim(method.substr(0, method.find('/')));

    if(mMethod == "POST"){
        string content = mHeaderByLine.at(CONTENT_POS);
        string length = MyUtil::myTrim(content.substr(content.find(':') + 1, content.length()));
//        cout << "POST: " << length << endl;
//        mContentLength = 45200;
        mContentLength = stoi(length);
    }
}

//split original char array by '\n'
vector<char*> ServletRequest::separateLine(char* res) {
    vector<char*> result;
    int range = mContentLength == -1 ? strlen(res) : mContentLength;

    char* tmp = res;
    vector<int> linePos = getLinePos(range, tmp);

    for(int i = 0; i < linePos.size() - 1; ++i){
        int beg = linePos[i];
        int end = linePos[i + 1];
        int size = end - beg + 1;
        char* line = new char[size];
        int index = 0;
        for(int j = beg; j < end; j++){
            line[index] = res[j];
            index++;
        }
        result.push_back(line);
    }
    return result;
}

//parse file part
void ServletRequest::parseFilePart() {
    mBodyByLine = separateLine(mBody);

    //extract file name
    parseFileName();

    //extract file info
    int endFile = parseFileInfo();

    //create file char
    createFileBytes(endFile);

    printf("Filename: %s, caption: %s, date: %s\n", mFileName.c_str(), mCaption.c_str(), mDate.c_str());
}

void ServletRequest::parseFileName() {
    int count = 1;
    string delimiter = "filename=\"";
    string line = mBodyByLine.at(count);
    int beg = line.find(delimiter) + delimiter.length();
    int range = line.size() - beg - 2;
    mFileName = line.substr(beg, range);
}

int ServletRequest::parseFileInfo() {
    const string BOUNDARY = "------WebKitFormBoundary";
    vector<int> linePos = getLinePos(mContentLength, mBody);

    int begLinePos = BEFORE_FILE;
    bool found = false;
    int endFile;
    int lineCount = 0;
    while (begLinePos < linePos.size() - 1 && !found) {
        int beg = linePos.at(begLinePos);
        int end = linePos.at(begLinePos + 1);
        int size = end - beg;
        char line[size];
        int index2 = 0;
        for (int i = beg; i < end; ++i) {
            line[index2] = mBody[i];
            index2++;
        }
        found = findString(line, BOUNDARY);
        if (found) {
            endFile = linePos.at(begLinePos) - 1;
        }
        begLinePos++;
        lineCount++;
    }

    int lineNum = lineCount + BEFORE_FILE;
    while (lineNum < mBodyByLine.size() && !findString(mBodyByLine.at(lineNum), BOUNDARY)) {
        ++lineNum;
    }
    mCaption = MyUtil::leftNewLineTrim(mBodyByLine.at(lineNum - 1));
    if(mCaption == ""){
        int pos = mFileName.find(".");
        mCaption = mFileName.substr(0, pos);
    }

    ++lineNum;
    while (lineNum < mBodyByLine.size() && !findString(mBodyByLine.at(lineNum), BOUNDARY)) {
        ++lineNum;
    }

    mDate = MyUtil::leftNewLineTrim(mBodyByLine.at(lineNum - 1)) == "" ?
            currentDateTime() :
            MyUtil::leftNewLineTrim(mBodyByLine.at(lineNum - 1));

    return endFile;
}

void ServletRequest::createFileBytes(int end) {
    int beg = 0;
    for (int i = 0; i < BEFORE_FILE; ++i) {
        beg += strlen(mBodyByLine.at(i));
    }
    mFileSize = end - beg - 1;

    mFile = new unsigned char[mFileSize];

    int pos = 0;
    for (int i = beg + 1; i < end; i++) {
        mFile[pos] = mBody[i];
        ++pos;
    }

    //print to check bytes - try comparing with wireshark packet info
//    for(int i = 0; i < mFileSize; i++){
//        printf("%x ", mFile[i]);
//    }
//    printf("\n");
}

bool ServletRequest::findString(char* str, string cmp){
    char temp[cmp.length() + 1];
    strcpy(temp, cmp.c_str());
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

vector<int> ServletRequest::getLinePos(int range, char* res) {
    vector<int> linePos;
    linePos.push_back(0);
    for(int i = 0; i < range; ++i){
        if(res[i] == '\n') {
            linePos.push_back(i);
        }
    }

    return linePos;
}

const string ServletRequest::currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    // Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
    // for more information about date/time format
    strftime(buf, sizeof(buf), "%Y-%m-%d", &tstruct);

    return buf;
}