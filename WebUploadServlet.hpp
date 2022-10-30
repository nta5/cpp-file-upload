#pragma once

#include "Servlet.hpp"
#include <iostream>

using namespace std;

class WebUploadServlet : public Servlet{
public:
    void doGet(ServletRequest request, ServletResponse response) override;
    void doPost(ServletRequest request, ServletResponse response) override;
};