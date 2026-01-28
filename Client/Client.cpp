#include "Client.h"
#include "Headers.h"
#include "Defines.h"
#include "DataSheets.h"

using namespace std;

Client::Client(const std::string& _strIP, uint16_t _iPort)
    : m_strIP(_strIP), m_iPort(_iPort), m_hSocket(INVALID_SOCKET), m_vecChatLog(MAX_CHAT_LINES)
{
}

Client::~Client()
{
    if (m_hSocket != INVALID_SOCKET)
    {
        closesocket(m_hSocket);
    }
    WSACleanup();
}

bool Client::InitWinsock()
{
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) 
    {
        cerr << "WSAStartup failed: " << iResult << endl;
        return false;
    }
    return true;
}

bool Client::CreateSocket()
{
    m_hSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_hSocket == INVALID_SOCKET)
    {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        return false;
    }
    return true;
}

bool Client::ConnectToServer()
{
    SOCKADDR_IN servAddr{};
    {
        servAddr.sin_family = AF_INET;
        servAddr.sin_port = htons(m_iPort);

        if (inet_pton(AF_INET, m_strIP.c_str(), &servAddr.sin_addr) <= 0) 
        {
            cerr << "Invalid IP address: " << m_strIP << endl;
            return false;
        }
    }
    
    int iResult = connect(m_hSocket, reinterpret_cast<SOCKADDR*>(&servAddr), sizeof(servAddr));
    if (iResult == SOCKET_ERROR) 
    {
        cerr << "Unable to connect to server! Error: " << WSAGetLastError() << endl;
        closesocket(m_hSocket);
        m_hSocket = INVALID_SOCKET;
        return false;
    }

    cout << "Connected to server: " << m_strIP << ":" << m_iPort << endl;
    return true;
}

bool Client::SendExact(const char* _szBuffer, const uint32_t& _iSize)
{
    int iTotalSent = 0;
    while (iTotalSent < _iSize)
    {
        int iSent = send(m_hSocket, _szBuffer + iTotalSent, _iSize - iTotalSent, 0);
        if (iSent == SOCKET_ERROR)
        {
            cerr << "Send failed: " << WSAGetLastError() << endl;
            return false;
        }
        iTotalSent += iSent;
    }
    return true;
}

bool Client::SendPacketToServer(const std::array<char, MAX_SEND_SIZE>& _arrSendBuffer, const uint32_t& _iSizeMsg)
{
    if (_iSizeMsg > MAX_SEND_SIZE)
    {
        cerr << "Message too large to send! size=" << _iSizeMsg << endl;
        return false;
    }

    // 1) 본문
    if (!SendExact(_arrSendBuffer.data(), _iSizeMsg))
        return false;

    return true;
}

bool Client::RecvExact(char* _szBuffer, uint32_t _iSize)
{
    int iTotalRecv = 0;
    while (iTotalRecv < _iSize)
    {
        int iReceived = recv(m_hSocket, _szBuffer + iTotalRecv, _iSize - iTotalRecv, 0);
        if (iReceived > 0)
        {
            iTotalRecv += iReceived;
        }
        else if (iReceived == 0)
        {
            cout << "Connection closed by server" << endl;
            return false;
        }
        else
        {
            cerr << "Recv failed: " << WSAGetLastError() << endl;
            return false;
        }
    }
    return true;
}

bool Client::RecvPacketFromServer(std::array<char, MAX_RECV_SIZE>& _arrRecvBuffer)
{
    // 1) 헤더 먼저 받기
    if (!RecvExact(_arrRecvBuffer.data(), sizeof(PACKET_HEADER)))
        return false;

    // 2) 헤더 역직렬화
    PACKET_HEADER tHeader{};
    tHeader.Deserialize(_arrRecvBuffer.data());

    // 사이즈 예외처리
    if (tHeader.iSize > MAX_RECV_SIZE)
    {
        cerr << "Message too large! size=" << tHeader.iSize << endl;
        return false;
    }

    // 3) 남은 본문 받기 (헤더 제외)
    if (!RecvExact(_arrRecvBuffer.data() + sizeof(PACKET_HEADER), tHeader.iSize - sizeof(PACKET_HEADER)))
        return false;

    return true;
}

ELANGUAGE_TYPE Client::ChooseLanguage()
{
    // 1) 언어 선택
    system("cls");
    cout << "====================================================\n";
    cout << "             Multilingual Chat Client               \n";
    cout << "----------------------------------------------------\n";
    cout << "   [ 언어 / Language / 语言 / 言語 ]\n";
    cout << "----------------------------------------------------\n";
    cout << "   1) KR 한국어\n";
    cout << "   2) US English\n";
    cout << "   3) CN 中文\n";
    cout << "   4) JP 日本語\n";
    cout << "----------------------------------------------------\n";
    
    int iSelect = 0;
    while (true) 
    {
        cout << "   Select (1-4): ";
        cin >> iSelect;
        cin.ignore();

        if (iSelect >= static_cast<int>(ELANGUAGE_TYPE::KOREAN) &&
            iSelect <= static_cast<int>(ELANGUAGE_TYPE::JAPANESE))
        {
            return static_cast<ELANGUAGE_TYPE>(iSelect);
        }
        else 
        {
            cerr << " Invalid input. Try again.\n";
        }
    }
}

void Client::ChatLoop(const ELANGUAGE_TYPE& _eLanguageType)
{
    // RecvThread 시작
    std::thread tRecvThread(&Client::RecvThread, this, _eLanguageType);

    string strInput;
    while (true)
    {
        // 화면 갱신
        system("cls");
        cout << g_umapChatHeader[_eLanguageType][0] << "\n";
        cout << g_umapChatHeader[_eLanguageType][1] << "\n";
        cout << g_umapChatHeader[_eLanguageType][2] << "\n";
        cout << g_umapChatHeader[_eLanguageType][3] << m_iCurrentUsers << "\n";
        cout << g_umapChatHeader[_eLanguageType][4] << "\n";

        // 최근 MAX_CHAT_LINES 개만 출력
        int iStart = max(0, m_vecChatLog.size() - MAX_CHAT_LINES);
        for (int i = iStart; i < m_vecChatLog.size(); i++)
        {
            cout << m_vecChatLog[i] << "\n";
        }

        cout << "==========================================" << "\n";
        cout << "[" << g_umapMe[_eLanguageType] << "] > ";

        // 입력
        if (!getline(cin, strInput))
            break;

        if (strInput == "exit")
            break;

        // 서버로 전송
        array<char, MAX_SEND_SIZE> arrSendBuffer{};
        PACKET_CHAT_MSG_SEND tSendPacket{};
        {
            tSendPacket.tHeader.iSize = sizeof(PACKET_CHAT_MSG_SEND);
            tSendPacket.tHeader.ePacketType = EPACKET_TYPE::CHAT_MESSAGE;
            tSendPacket.eChatType = ECHAT_TYPE::MESSAGE;
            std::copy(m_strNickName.begin(), m_strNickName.end(), tSendPacket.arrNickName.data());
            std::copy(strInput.begin(), strInput.end(), tSendPacket.arrMessage.data());
            tSendPacket.Serialize(arrSendBuffer.data());

            if (!SendPacketToServer(arrSendBuffer, sizeof(PACKET_CHAT_MSG_SEND)))
                break;
        }
    }
    tRecvThread.join();
}

std::string Client::WrapText(const std::string& _strInput, const size_t& _iMaxWidth)
{
    std::string strResult;
    size_t iLineLength = 0;
    size_t iLastSpacePos = std::string::npos;

    for (int i = 0; i < _strInput.size(); ++i)
    {
        char szChar = _strInput[i];
        strResult.push_back(szChar);
        ++iLineLength;

        if (szChar == ' ')
            iLastSpacePos = strResult.size() - 1;

        if (iLineLength >= _iMaxWidth)
        {
            if (iLastSpacePos != std::string::npos)
            {
                // 공백에서 개행
                strResult[iLastSpacePos] = '\n';
                iLineLength = strResult.size() - iLastSpacePos - 1;
                iLastSpacePos = std::string::npos;
            }
            else
            {
                // 공백이 없으면 현재 위치에서 개행
                strResult.push_back('\n');
                iLineLength = 0;
            }
        }
    }
    return strResult;
}

void Client::RecvThread(const ELANGUAGE_TYPE& _eLanguageType)
{
    while (true) 
    {
        array<char, MAX_RECV_SIZE> arrRecvBuffer{};
        if (!RecvPacketFromServer(arrRecvBuffer))
            break;

        // 서버로부터 응답 받기
        PACKET_CHAT_MSG_RECV tRecvPacket{};
        tRecvPacket.Deserialize(arrRecvBuffer.data());

        // Render Text
        string strRenderText;
        string strRecvNickName(tRecvPacket.arrNickName.data());

        switch (tRecvPacket.eChatType)
        {
        case ECHAT_TYPE::JOIN:
        {
            m_iCurrentUsers = tRecvPacket.iUserCount;
            strRenderText = g_umapJoinMsg[_eLanguageType].first + strRecvNickName + g_umapJoinMsg[_eLanguageType].second;
        }
        break;

        case ECHAT_TYPE::LEAVE:
        {
            strRenderText = g_umapLeaveMsg[_eLanguageType].first + strRecvNickName + g_umapLeaveMsg[_eLanguageType].second;
        }
        break;

        case ECHAT_TYPE::MESSAGE:
        {
            string strRecvMsg(tRecvPacket.arrMessage.data());
            if (strRecvNickName == m_strNickName)
                strRenderText = "[" + g_umapMe[_eLanguageType] + "] : " + strRecvMsg;
            else
                strRenderText = "[" + strRecvNickName + "] : " + strRecvMsg;
        }
        break;

        default:
            break;
        }

        // 길이가 너비를 넘을 경우, 개행 처리
        if (_eLanguageType == ELANGUAGE_TYPE::ENGLISH
            && strRenderText.length() >= MAX_CHAT_WIDTH)
        {
            strRenderText = WrapText(strRenderText, MAX_CHAT_WIDTH);
        }

        // 로그 업데이트는 mutex로 보호
        {
            std::lock_guard<std::mutex> lock(m_mtxChatLog);
            m_vecChatLog.push_back(FilterMessage(strRenderText));
        }
    }
}

std::string Client::FilterMessage(const std::string& _strMsg)
{
    string strFilteredMsg = _strMsg;

    for (const auto& strForbiddenWord : g_vecForbidden) 
    {
        size_t iPos = 0;
        while ((iPos = strFilteredMsg.find(strForbiddenWord, iPos)) != string::npos) 
        {
            strFilteredMsg.replace(iPos, strForbiddenWord.size(), string(strForbiddenWord.size(), '*'));
            iPos += strForbiddenWord.size();
        }
    }
    return strFilteredMsg;
}

void Client::RequestNicknameRegistration(const ELANGUAGE_TYPE& _eLanguageType)
{    
    while (true) 
    {
        system("cls");

        cout << g_umapEnterNickName[_eLanguageType];

        getline(cin, m_strNickName);

        // 요청 패킷 보내기
        std::array<char, MAX_SEND_SIZE> arrSendRequestBuffer{};
        PACKET_NICKNAME_REGISTER_REQ tRequestPacket{};
        {
            tRequestPacket.tHeader.iSize = sizeof(PACKET_NICKNAME_REGISTER_REQ);
            tRequestPacket.tHeader.ePacketType = EPACKET_TYPE::NICKNAME_REGISTER_REQ;
            std::copy(m_strNickName.begin(), m_strNickName.end(), tRequestPacket.arrNickName.data());
            tRequestPacket.Serialize(arrSendRequestBuffer.data());

            if (!SendPacketToServer(arrSendRequestBuffer, sizeof(PACKET_NICKNAME_REGISTER_REQ)))
                break;
        }
        
        
        // 응답 받기
        PACKET_NICKNAME_REGISTER_ACK tAcknowledgePacket{};
        {
            std::array<char, MAX_RECV_SIZE> arrRecvAcknowledgeBuffer{};
            if (!RecvPacketFromServer(arrRecvAcknowledgeBuffer))
                break;

            tAcknowledgePacket.Deserialize(arrRecvAcknowledgeBuffer.data());
        }

        // 응답 받은 메시지 출력
        if (tAcknowledgePacket.bSuccess)
        {
            cout << g_umapLocalRegistrationSuccessMsg[_eLanguageType] << '\n';
            cin.get();
            break;
        }
        else
        {
            cout << g_umapLocalRegistrationFailureMsg[_eLanguageType] << '\n';
            cin.get();
            continue;
        }        
    }
}

void Client::Run()
{
    // 1. 언어 선택
    ELANGUAGE_TYPE eLanguageType = ChooseLanguage();

    // 2. 닉네임 등록 요청
    RequestNicknameRegistration(eLanguageType);

    // 3. 채팅 루프
    ChatLoop(eLanguageType);
}
