#pragma once
#include <string>
#include <map>
#include "Http.hpp" 

class TgBot {
public:
    TgBot(const std::string& token);
    Response sendFile(const std::string& chat_id, const std::string& file_path);

private:
    std::string token;
    std::string base_url;
};
