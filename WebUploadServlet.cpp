#include "WebUploadServlet.hpp"
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

using std::filesystem::recursive_directory_iterator;

void WebUploadServlet::doGet(int sock, ServletRequest request, ServletResponse response) {
    int rval;
    string resString = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-length: 513\r\n\r\n <!DOCTYPE html>\r\n<html>\n   <head>\n       <title>File Upload Form</title>\n   </head>\n   <body>\n       <h1>Upload file</h1>\n       <form method=\"POST\" action=\"upload\" enctype=\"multipart/form-data\">\n           <input type=\"file\" name=\"fileName\"/><br/><br/>\n           Caption: <input type=\"text\" name=\"caption\"<br/><br/><br/>\n           Date: <input type=\"date\" name=\"date\"<br/><br/><br/>\n           <input type=\"submit\" value=\"Submit\"/>\n       </form>\n   </body>\n</html>\r\n\r\n";
    char res[resString.length() + 1];
    strcpy(res, resString.c_str());

    if ((rval = write(sock, res, strlen(res))) < 0){
        perror("writing socket");
    }else  {
        printf("%s\n",res);
    }

    close (sock);
    return;
}

void WebUploadServlet::doPost(int sock, ServletRequest request, ServletResponse response) {
    int rval;
    string resString = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html>\r\n<html>\n   <head>\n       <title>File Upload Form</title>\n   </head>\n   <body>\n       <h3>File Uploaded: "
            + request.getFileName() + ", " + request.getCaption() + ", " + request.getDate() + "</h3>\n <h4>Server Files:</h4>\n" ;

    ofstream fout;
    fout.open("images/" +request.getDate()+"_"+request.getCaption()+"_"+request.getFileName(), ios::binary | ios::out);
    unsigned char *image = request.getFileByte();

    fout.write((char *)&image[0], request.getFileSize());
    fout.close();

    string path = "./images/";

    for (const auto & file : recursive_directory_iterator(path)) {
        string str(file.path().filename());
        resString += "<h4>" + str + "</h4>\n";
    }

    resString += "</body>\n</html>\r\n";
    if(request.getHost() == 1){
        resString += "<JSON>\n{\n";

        for (const auto & file : recursive_directory_iterator(path)) {
            string str(file.path().filename());
            if(str.at(0) != '.'){
                int findDate = str.find("_");
                string date = str.substr(0, findDate);

                string tmp = str.substr(findDate + 1, str.length() - findDate - 1);
                int findCaption = tmp.find("_");
                string caption = tmp.substr(0, findCaption);

                string fileName = tmp.substr(findCaption + 1, tmp.length() - findCaption - 1);

                resString += "{fileName: " + fileName + ", caption: " + caption + ", date: " + date + "}\n";
            }
        }
        resString += "}\r\n\r\n";
    }else{
        resString += "\r\n";
    }

    char res[resString.length() + 1];
    strcpy(res, resString.c_str());

    if ((rval = write(sock, res, strlen(res))) < 0){
        perror("writing socket");
    }else  {
//        printf("%s\n",res);
    }

    close (sock);
    return;
}