#include "tgbot.hpp"
#include <fstream>
#include <sstream>
#include "stringToWString.hpp"

TgBot::TgBot(const std::string& token)
    : token(token), base_url("https://api.telegram.org/bot" + token + "/sendDocument") {}

Response TgBot::sendFile(const std::string& chat_id, const std::string& file_path) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        return { -1, "Failed to open file" };
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();
    std::string file_data = buffer.str();

    std::string boundary = "------------------------boundary12345";
    std::ostringstream data_stream;
    data_stream << "--" << boundary << "\r\n"
        << "Content-Disposition: form-data; name=\"chat_id\"\r\n\r\n"
        << chat_id << "\r\n"
        << "--" << boundary << "\r\n"
        << "Content-Disposition: form-data; name=\"document\"; filename=\""
        << file_path.substr(file_path.find_last_of("/\\") + 1) << "\"\r\n"
        << "Content-Type: application/octet-stream\r\n\r\n"
        << file_data << "\r\n"
        << "--" << boundary << "--\r\n";

    std::string data = data_stream.str();
    std::map<std::wstring, std::wstring> headers = {
        {L"Content-Type", L"multipart/form-data; boundary=" + stringToWString(boundary)},
        {L"Content-Length", stringToWString(std::to_string(data.size()))}
    };

    Post_get http;
    return http.Post(base_url, data, headers);
}
