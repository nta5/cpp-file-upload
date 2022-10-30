#pragma once

#include "ServletRequest.hpp"
#include "ServletResponse.hpp"

class Servlet{
public:
    virtual void doGet(int sock, ServletRequest request, ServletResponse response) = 0;
    virtual void doPost(int sock, ServletRequest request, ServletResponse response) = 0;
};
