#include "GameManager.h"
#include "Stock.h"
#include "Coin.h"
#include "ConsoleUI.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <algorithm>
#include <thread>
#include <chrono>

using namespace std;
using namespace ConsoleUI;

/**
 * 게임 관리자 생성자
 * 랜덤 시드 설정, 플레이어 생성, 시장 및 뉴스 시스템을 초기화합니다.
 */
GameManager::GameManager() : day(1) {
    srand((unsigned int)time(NULL));
    player = make_unique<Player>("투자자", 3000000); // 시작 자금 300만 원
    InitM(); // 시장 종목 초기화
}

/**
 * 게임 관리자 소멸자
 * 동적 할당된 메모리(종목 객체들, 플레이어)를 해제합니다.
 */
GameManager::~GameManager() {
}

/**
 * 시장에 상장된 10개의 기본 종목(주식 8개, 코인 2개)을 설정합니다.
 */
void GameManager::InitM() {
    market.emplace_back(make_unique<Stock>("삼성전자", 75000, "국내/반도체"));
    market.emplace_back(make_unique<Stock>("엔비디아", 1200000, "해외/반도체"));
    market.emplace_back(make_unique<Stock>("네이버", 210000, "국내/IT"));
    market.emplace_back(make_unique<Stock>("카카오", 55000, "국내/IT"));
    market.emplace_back(make_unique<Stock>("테슬라", 250000, "해외/전기차"));
    market.emplace_back(make_unique<Stock>("에코프로", 580000, "국내/전기차"));
    market.emplace_back(make_unique<Stock>("펄어비스", 40000, "국내/게임"));
    market.emplace_back(make_unique<Stock>("크래프톤", 250000, "국내/게임"));
    market.emplace_back(make_unique<Coin>("비트코인", 95000000, "코인"));
    market.emplace_back(make_unique<Coin>("도지코인", 250, "코인"));
}

/**
 * 콘솔 화면 전체를 그리는 통합 드로잉 함수
 * 헤더, 메인 리스트(시장가/보유), 뉴스, 로그/버튼 순으로 출력합니다.
 * centerMsg가 비어있지 않으면 중앙에 해당 메시지를 출력합니다.
 */
void GameManager::Draw(string l1, string l2, string c1, string c2, string c3, string c4, int page, string centerMsg) {
    // 시장 데이터 가공
    vector<pair<string, pair<float, float>>> marketData;
    for (const auto& inv : market) {
        marketData.push_back({ inv->GetName(), { inv->GetPrice(), inv->GetPrevPrice() } });
    }

    // 인벤토리 데이터 가공
    vector<vector<string>> inventoryData;
    if (page != -1) {
        const auto& inv = player->GetInventory();
        for (const auto& h : inv) {
            float p = h.inv->GetPrice(), ap = h.avgPrice;
            int q = h.quantity;
            float r = (ap > 0) ? ((p - ap) / ap) * 100.0f : 0;
            long long prof = (long long)((p - ap) * q);
            string color = (r > 0) ? RED : (r < 0) ? BLUE : "";
            
            vector<string> row;
            row.push_back(BOLD + "[" + to_string(&h - &inv[0] + 1) + "] " + RESET + h.inv->GetName());
            row.push_back(GREEN + "[" + to_string(q) + "주]" + RESET);
            row.push_back(Format((long long)(p * q)) + "원");
            row.push_back(color + (prof >= 0 ? "+" : "") + Format(prof) + "(" + to_string((int)abs(r)) + "%)" + RESET);
            inventoryData.push_back(row);
        }
    }

    ConsoleUI::DrawMain(day, player->GetCash(), player->GetTotalEval(), player->GetTotalInvested(),
                        news, l1, l2, c1, c2, c3, c4, marketData, page, centerMsg, inventoryData);
}

/**
 * 게임 메인 실행 루프
 */
void GameManager::Start() {
    ShowIntro();

    // 부팅 연출
    Draw("", "", "", "", "", "", -1, "컴퓨터 키는중...");
    this_thread::sleep_for(chrono::milliseconds(2000));

    string log = "시장이 열렸습니다.";
    while (true) {
        Draw(log, "", "1. 매수", "2. 매도", "3. 정보", "4. 다음날", -1);
        cout << " >> 선택 : ";
        int choice = Input();
        if (choice == 1) log = Store();
        else if (choice == 2) log = Sell();
        else if (choice == 3) Inventory();
        else if (choice == 4) log = Next();
        else log = RED + "올바른 메뉴 번호를 선택하세요." + RESET;
    }
}

/**
 * 상세 보유 현황을 보여주는 인벤토리 화면 처리
 */
void GameManager::Inventory() {
    while (true) {
        const auto& inv = player->GetInventory();
        long long prof = (long long)(player->GetTotalEval() - player->GetTotalInvested());
        float totalInv = player->GetTotalInvested();
        float r = (totalInv > 0) ? ((float)prof / totalInv) * 100.0f : 0;
        string log = "총수익: " + (prof >= 0 ? RED : BLUE) + Format(prof) + "원 (" + to_string((int)abs(r)) + "%)" + RESET;
        
        Draw(log, "", "0. 뒤로", "", "", "", 0);
        cout << " >> 선택 : ";
        if (Input() == 0) break;
    }
}

/**
 * 종목 매수 프로세스
 */
string GameManager::Store() {
    Draw("매수할 종목 번호를 입력하세요.", "", "0. 뒤로", "", "", "", -1);
    cout << " >> 번호 : ";
    int num = Input();
    if (num <= 0 || num > (int)market.size()) return RED + "매수를 취소했습니다." + RESET;
    
    int idx = num - 1;
    if (market[idx]->GetPrice() <= 0) return RED + "상장폐지 종목은 구매할 수 없습니다." + RESET;
    
    int curH = player->GetHoldingQty(market[idx]->GetName());
    int maxC = (int)(player->GetCash() / market[idx]->GetPrice());
    
    string l1 = market[idx]->GetName() + " " + GREEN + "[보유:" + to_string(curH) + "]" + RESET;
    string l2 = "몇 개 사시겠습니까?(최대 " + YELLOW + to_string(maxC) + RESET + "개)";
    Draw(l1, l2, "0. 취소", "", "", "", -1);
    
    cout << " >> 수량 : ";
    int count = Input();
    if (count <= 0) return RED + "매수를 취소했습니다." + RESET;
    
    long long cost = (long long)market[idx]->GetPrice() * count;
    string c1 = market[idx]->GetName() + " " + to_string(count) + "개";
    string c2 = "총 " + YELLOW + Format(cost) + RESET + "원. 구매합니까?";
    Draw(c1, c2, "1. 확인", "2. 취소", "", "", -1);
    
    cout << " >> 선택 : ";
    if (Input() == 1) {
        if (player->Buy(market[idx].get(), count)) return GREEN + market[idx]->GetName() + " " + to_string(count) + "개 매수 성공!" + RESET;
        else return RED + "자금이 부족합니다." + RESET;
    }
    return RED + "매수를 취소했습니다." + RESET;
}

/**
 * 종목 매도 프로세스
 */
string GameManager::Sell() {
    const auto& inv = player->GetInventory();
    if (inv.empty()) return RED + "매도할 종목이 없습니다. (보유 종목 없음)" + RESET;
    
    Draw("매도할 종목 번호를 입력하세요.", "", "0. 뒤로", "", "", "", 0);
    cout << " >> 번호 : ";
    int num = Input();
    if (num <= 0 || num > (int)inv.size()) return RED + "매도를 취소했습니다." + RESET;
    
    int idx = num - 1;
    Investment* target = inv[idx].inv;
    int hold = inv[idx].quantity;
    
    string l1 = target->GetName() + " " + GREEN + "[보유:" + to_string(hold) + "]" + RESET;
    string l2 = "몇 개 파시겠습니까?";
    Draw(l1, l2, "0. 취소", "", "", "", 0);
    
    cout << " >> 수량 : ";
    int count = Input();
    if (count <= 0) return RED + "매도를 취소했습니다.";
    
    long long income = (long long)target->GetPrice() * count;
    string c1 = target->GetName() + " " + to_string(count) + "개";
    string c2 = "총 " + YELLOW + Format(income) + RESET + "원. 판매합니까?";
    Draw(c1, c2, "1. 확인", "2. 취소", "", "", 0);
    
    cout << " >> 선택 : ";
    if (Input() == 1) {
        if (player->Sell(target, count)) return GREEN + target->GetName() + " " + to_string(count) + "개 매도 성공!" + RESET;
        else return RED + "보유 수량이 부족합니다." + RESET;
    }
    return RED + "매도를 취소했습니다." + RESET;
}

/**
 * 날짜를 변경하고 주가 업데이트 및 무작위 뉴스를 발생시킵니다.
 */
string GameManager::Next() {
    // 1. 자러 가는 멘트 출력 후 1.5초 대기
    Draw("", "", "", "", "", "", -1, "자러 가는 중...");
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    // 2. 일어나는 멘트 출력 후 1.5초 대기
    Draw("", "", "", "", "", "", -1, "침대에서 일어나는 중...");
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    day++;
    for (auto& inv : market) inv->RecordPrevPrice(); // 전일 종가 기록

    // 90% 확률로 경제 뉴스 발생
    if (rand() % 10 < 9) {
        const News& n = newsData.GetRandomNews();
        string color = (n.impact > 0) ? RED : (n.impact < 0) ? BLUE : "";
        news = color + "[속보] " + n.headline + RESET;
        
        // 뉴스 범위에 따른 효과 적용
        for (auto& inv : market) {
            if (n.type == 0) inv->ApplyNews(n.impact);
            else if (n.type == 1 && inv->GetTheme().find(n.target) != string::npos) inv->ApplyNews(n.impact);
            else if (n.type == 2 && inv->GetName() == n.target) inv->ApplyNews(n.impact);
        }
    } else {
        news = ""; 
    }

    // 모든 종목의 주가 업데이트 (자연 변동폭 포함)
    for (auto& inv : market) inv->UpdatePrice();

    return "오늘의 주식 장이 열렸습니다!";
}

void GameManager::ShowIntro() {
    while (true) {
        SetCursorVisible(true);
        Clear();
        cout << "\n\n\n\n\n";
        ConsoleUI::CenterText(BOLD + RED + "[ 주식 시뮬레이터 ]" + RESET);
        cout << "\n";
        ConsoleUI::CenterText(YELLOW + "1. 게임 시작" + RESET);
        ConsoleUI::CenterText("2. 게임 종료");
        cout << "\n\n";
        ConsoleUI::CenterText(">> 선택 : ", 80, false); 
        int choice = Input();
        if (choice == 1) break;
        if (choice == 2) exit(0);
    }

    SetCursorVisible(false);
    vector<string> stories = {
        "부자가 되고 싶다",
        "내 전재산 300만원..",
        "인생은 한방이야" ,
        "[ 주식 시뮬레이터 ]"
    };

    for (const string& s : stories) {
        Clear();
        cout << "\n\n\n\n\n\n\n\n";
        ConsoleUI::CenterText(BOLD + s + RESET);
        this_thread::sleep_for(chrono::milliseconds(2500));
    }

    while (true) {
        SetCursorVisible(true);
        Clear();
        cout << "\n\n\n";
        ConsoleUI::CenterText(BOLD + RED + "[ 투자 위험 고지 및 이용 약관 ]" + RESET);
        cout << "\n";
        ConsoleUI::CenterText("1. 본 프로그램은 모의 투자 체험을 목적으로 합니다.");
        ConsoleUI::CenterText("2. 모든 투자 판단의 책임은 이용자에게 귀속됩니다.");
        ConsoleUI::CenterText("3. 발생한 손실에 대해 제작자는 책임지지 않습니다.");
        ConsoleUI::CenterText("4. 과도한 몰입은 일상 생활에 지장을 줄 수 있습니다.");
        cout << "\n\n";
        ConsoleUI::CenterText(YELLOW + "1. 약관에 동의하고 입장" + RESET);
        cout << "\n";
        ConsoleUI::CenterText(">> 선택 : ", 80, false);
        if (Input() == 1) break;
    }
}
