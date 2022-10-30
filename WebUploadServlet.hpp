#pragma once

#include "Servlet.hpp"
#include "Socket.h"
#include <iostream>

using namespace std;

class WebUploadServlet : public Servlet{
public:
    void doGet(int sock, ServletRequest request, ServletResponse response);
    void doPost(int sock, ServletRequest request, ServletResponse response);
};