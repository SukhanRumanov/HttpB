#include <iostream>
#include "tgbot.hpp"
#include <locale.h>

int main() {
    setlocale(LC_ALL, "Rus");
    const std::string token = "bot token";
    const std::string chat_id = "you chat id";

    TgBot bot(token);
    std::string file_path = "test.txt";
    Response file_response = bot.sendFile(chat_id, file_path);
    if (file_response.status == 200) {
        std::cout << "File sent successfully: " << file_response.body << std::endl;
    }
    else {
        std::cout << "Failed to send file: " << file_response.body << std::endl;
    }

    return 0;
}
