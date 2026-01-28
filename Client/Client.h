#pragma once
#include "Headers.h"
#include "Defines.h"

class Client final 
{
public:
    explicit Client(const std::string& _strIP, uint16_t _iPort);
    ~Client();

public:
    bool InitWinsock();
    bool CreateSocket();
    bool ConnectToServer();
    void Run();

private:
    bool SendExact(const char* _szBuffer, const uint32_t& _iSize);
    bool SendPacketToServer(const std::array<char, MAX_SEND_SIZE>& _arrSendBuffer, const uint32_t& _iSizeMsg);

    bool RecvExact(char* _szBuffer, uint32_t _iSize);
    bool RecvPacketFromServer(std::array<char, MAX_RECV_SIZE>& _arrRecvBuffer);

    ELANGUAGE_TYPE ChooseLanguage();
    void RequestNicknameRegistration(const ELANGUAGE_TYPE& _eLanguageType);
    void ChatLoop(const ELANGUAGE_TYPE& _eLanguageType);

    std::string WrapText(const std::string& _strInput, const size_t& _iMaxWidth);
    void RecvThread(const ELANGUAGE_TYPE& _eLanguageType);
    std::string FilterMessage(const std::string& _strMsg); 

private:
    SOCKET m_hSocket;

    std::string m_strIP;
    uint16_t m_iPort;

    std::string m_strNickName;

    int m_iCurrentUsers = 1;
    std::vector<std::string> m_vecChatLog;
    std::mutex m_mtxChatLog;
};
