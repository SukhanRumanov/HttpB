#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wininet.h>
#include <string>
#include <map>
#include "Http.hpp"
#include "stringToWString.hpp"

Response Send(std::wstring method, std::wstring url, std::string data, std::map<std::wstring, std::wstring> headers) {
    Response response;

    wprintf(L"Request URL: %s\n", url.c_str());

    HINTERNET hSession = InternetOpen(L"HttpClient", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hSession) {
        response.status = -1;
        response.body = "Не удалось открыть интернет-сессию";
        return response;
    }

    HINTERNET hConnect = InternetOpenUrl(hSession, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hConnect) {
        InternetCloseHandle(hSession);
        response.status = -1;
        response.body = "Не удалось открыть URL";
        return response;
    }

    for (const auto& header : headers) {
        std::wstring fullHeader = header.first + L": " + header.second + L"\r\n";
        HttpAddRequestHeaders(hConnect, fullHeader.c_str(), -1, HTTP_ADDREQ_FLAG_ADD);
    }

    BOOL bRequestSent = HttpSendRequest(hConnect, NULL, 0, (LPVOID)data.c_str(), data.size());
    if (!bRequestSent) {
        DWORD dwError = GetLastError();
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hSession);
        response.status = -1;
        response.body = "Не удалось отправить HTTP-запрос, код ошибки: " + std::to_string(dwError);
        return response;
    }

    DWORD statusCode = 0;
    DWORD statusCodeSize = sizeof(DWORD);
    if (!HttpQueryInfo(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &statusCodeSize, NULL)) {
        DWORD dwError = GetLastError();
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hSession);
        response.status = -1;
        response.body = "Не удалось получить код статуса, код ошибки: " + std::to_string(dwError);
        return response;
    }
    response.status = static_cast<int>(statusCode);

    char buffer[4096];
    DWORD bytesRead;
    while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
        response.body.append(buffer, bytesRead);
    }

    InternetCloseHandle(hConnect);
    InternetCloseHandle(hSession);

    return response;
}

Response Post_get::Get(const std::string& url, const std::map<std::wstring, std::wstring>& headers) {
    return Send(L"GET", stringToWString(url), "", headers);
}

Response Post_get::Post(const std::string& url, const std::string& data, const std::map<std::wstring, std::wstring>& headers) {
    return Send(L"POST", stringToWString(url), data, headers);
}
