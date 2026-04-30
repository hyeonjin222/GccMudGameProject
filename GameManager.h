#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Investment.h"
#include "Player.h"
#include "ConsoleUI.h"
#include "NewsData.h"

using namespace std;

// 게임의 전체 시스템(시장, 플레이어, 뉴스, UI)을 제어하는 관리자 클래스입니다.
class GameManager {
private:
    vector<unique_ptr<Investment>> market; // 시장에 상장된 모든 투자 종목 리스트
    unique_ptr<Player> player;             // 플레이어 객체
    int day;                    // 현재 진행 중인 게임 일수
    NewsData newsData;          // 발생 가능한 뉴스 데이터베이스
    string news;                // 화면에 현재 표시 중인 속보 내용

    // 시장 종목(주식 8개, 코인 2개)을 초기화합니다.
    void InitM();
    
    // 화면 전체 프레임을 그리는 핵심 메서드입니다. (로그, 버튼 메뉴, 페이지 설정, 중앙 메시지)
    void Draw(string l1, string l2, string c1, string c2, string c3, string c4, int page = -1, string centerMsg = "");

    // 인트로 화면과 약관 동의 프로세스를 처리합니다.
    void ShowIntro();

public:
    // 게임 관리자를 생성하고 초기 설정을 진행합니다.
    GameManager();

    // 할당된 자원(플레이어, 종목 객체 등)을 해제합니다.
    ~GameManager();

    // 메인 게임 루프를 시작합니다.
    void Start();

    // 다음 날로 진행하며 주가 변동 및 뉴스를 처리합니다.
    string Next();

    // 매수(상점) 프로세스를 실행합니다.
    string Store();

    // 매도 프로세스를 실행합니다.
    string Sell();

    // 플레이어의 보유 종목 상세 현황 창을 표시합니다.
    void Inventory();
};
