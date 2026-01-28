#pragma once
#include "Headers.h"
#include "Defines.h"

std::unordered_map<ELANGUAGE_TYPE, std::string> g_umapEnterNickName =
{
    {ELANGUAGE_TYPE::KOREAN, "닉네임을 입력하세요 : "},
    {ELANGUAGE_TYPE::ENGLISH, "Please enter your nickname : "},
    {ELANGUAGE_TYPE::CHINESE, "请输入昵称 : "},
    {ELANGUAGE_TYPE::JAPANESE, "ニックネームを入力してください : "},
};

// 다국어 닉네임 등록 메시지 맵
std::unordered_map<ELANGUAGE_TYPE, std::string> g_umapLocalRegistrationSuccessMsg =
{
    {ELANGUAGE_TYPE::KOREAN,  "닉네임 등록 성공!"},
    {ELANGUAGE_TYPE::ENGLISH, "Nickname registered successfully!"},
    {ELANGUAGE_TYPE::CHINESE, "昵称注册成功!"},
    {ELANGUAGE_TYPE::JAPANESE,"ニックネームの登録に成功しました！"}
};
std::unordered_map<ELANGUAGE_TYPE, std::string> g_umapLocalRegistrationFailureMsg =
{
    {ELANGUAGE_TYPE::KOREAN,  "닉네임이 이미 사용 중입니다."},
    {ELANGUAGE_TYPE::ENGLISH, "This nickname is already taken."},
    {ELANGUAGE_TYPE::CHINESE, "昵称已被使用."},
    {ELANGUAGE_TYPE::JAPANESE,"このニックネームは既に使用されています。"}
};

// 채팅방
std::unordered_map<ELANGUAGE_TYPE, std::string> g_umapChat =
{
    {ELANGUAGE_TYPE::KOREAN,  "닉네임이 이미 사용 중입니다."},
    {ELANGUAGE_TYPE::ENGLISH, "This nickname is already taken."},
    {ELANGUAGE_TYPE::CHINESE, "昵称已被使用."},
    {ELANGUAGE_TYPE::JAPANESE,"このニックネームは既に使用されています。"}
};

std::unordered_map<ELANGUAGE_TYPE, std::vector<std::string>> g_umapChatHeader =
{
    {ELANGUAGE_TYPE::KOREAN, {
        "==========================================",
        "          채팅방에 오신 것을 환영합니다",
        "------------------------------------------",
        " 현재 접속자 수 : ",
        "=========================================="
    }},
    {ELANGUAGE_TYPE::ENGLISH, {
        "==========================================",
        "          Welcome to Chat",
        "------------------------------------------",
        " Current Users : ",
        "=========================================="
    }},
    {ELANGUAGE_TYPE::CHINESE, {
        "==========================================",
        "          欢迎进入聊天室",
        "------------------------------------------",
        " 当前用户数 : ",
        "=========================================="
    }},
    {ELANGUAGE_TYPE::JAPANESE, {
        "==========================================",
        "          チャットへようこそ",
        "------------------------------------------",
        " 現在のユーザー数 : ",
        "=========================================="
    }}
};

// 채팅방 "나"
std::unordered_map<ELANGUAGE_TYPE, std::string> g_umapMe =
{
    {ELANGUAGE_TYPE::KOREAN,  "플레이어"},
    {ELANGUAGE_TYPE::ENGLISH, "Player"},
    {ELANGUAGE_TYPE::CHINESE, "玩家"},
    {ELANGUAGE_TYPE::JAPANESE,"プレイヤー"}
};


// 입장 메시지 (prefix, suffix)
std::unordered_map<ELANGUAGE_TYPE, std::pair<std::string, std::string>> g_umapJoinMsg =
{
    {ELANGUAGE_TYPE::KOREAN,  {"[시스템] 플레이어 \"", "\"님이 입장했습니다."}},
    {ELANGUAGE_TYPE::ENGLISH, {"[System] Player \"", "\" has joined the room."}},
    {ELANGUAGE_TYPE::CHINESE, {"[系统] 玩家\"", "\"已进入聊天室。"}},
    {ELANGUAGE_TYPE::JAPANESE,{"[システム] プレイヤー\"", "\"が入室しました。"}}
};

// 퇴장 메시지 (prefix, suffix)
std::unordered_map<ELANGUAGE_TYPE, std::pair<std::string, std::string>> g_umapLeaveMsg =
{
    {ELANGUAGE_TYPE::KOREAN,  {"[시스템] 플레이어 \"", "\" 님이 퇴장했습니다."}},
    {ELANGUAGE_TYPE::ENGLISH, {"[System] Player \"", "\" has left the room."}},
    {ELANGUAGE_TYPE::CHINESE, {"[系统] 玩家\"", "\"已退出聊天室。"}},
    {ELANGUAGE_TYPE::JAPANESE,{"[システム] プレイヤー\"", "\"が退室しました。"}}
};

std::vector<std::string> g_vecForbidden = 
{
    "바보", "Damn", "笨蛋", "バカ"
};