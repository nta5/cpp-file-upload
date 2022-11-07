# COMP-3940 Assignment 3

#### Group 3
Term: Set 3G, Fall 2022 \
Members: Chengyang Li, Sehyun Park, Alex Pu, Melanie Ta, Shen Yen

### How to run
- Stay in the root folder cpp-file-upload
#### Server
- To compile server:
```
g++-12 -o server main.cpp Thread.cpp SimpleThread.cpp ServerSocket.cpp Socket.cpp ServletRequest.cpp ServletResponse.cpp WebUploadServlet.cpp MyUtil.cpp ConsoleUploadServlet.cpp ServerThread.cpp
```
- To run the server: `./server`

#### Client
- First, find the statement below on the first line from `main()` in  `SocketClient.cpp` and update the value to your ip address. 
```
const char MY_IP[] = "Put your ip address here!";
```
- To compile client:
```
g++-12 -o client SocketClient.cpp
```

- To start the console client: `./client`

### Description
- Launch an upload form in http://localhost:8888/. Through the web form, users can upload their images of choice. Upon submission, the server will display a list of images in the html page.
- Users can also connect to the server using the console client. Upon sending an image, the server will return a JSON string of all the current images.

### Contribution
- Chengyang Li
    - Create and connect Console app to server
    - Develop user input for Console App
    - Test and debug
- Sehyun Park
    - Parse HTTP Request for Web and Console app
    - Create Socket to read request from the socket
    - Create JSON response for Console app
- Alex Pu:
    - Create request and response for Web and Console app
    - Write image to local directory
    - Demo video
- Melanie Ta
    - Implement Multithreading
    - Establish and test connections to Assignment2 Java Server/Client 
    - Planning and organizing tasks 
- Shen Yen
    - Develop user input for Console App
    - Test and debug
    - Demo video
