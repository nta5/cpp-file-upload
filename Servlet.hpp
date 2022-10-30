#pragma once

#include "ServletRequest.hpp"
#include "ServletResponse.hpp"

class Servlet{
public:
    virtual void doGet(ServletRequest request, ServletResponse response) = 0;
    virtual void doPost(ServletRequest request, ServletResponse response) = 0;
};
