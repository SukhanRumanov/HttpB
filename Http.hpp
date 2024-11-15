#pragma once
#include <string>
#include <map>
#include <windows.h>
#include <wininet.h>
#include <iostream>

#pragma comment (lib, "Wininet.lib")

struct Response {
    int status;
    std::string body;
};

class Post_get {
public:
    Response Get(const std::string& url, const std::map<std::wstring, std::wstring>& headers);
    Response Post(const std::string& url, const std::string& data, const std::map<std::wstring, std::wstring>& headers);
};
