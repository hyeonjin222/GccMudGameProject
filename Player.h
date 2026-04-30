#pragma once
#include <string>
#include <vector>
#include "Investment.h"

using namespace std;

// 특정 종목의 보유 정보를 담는 구조체입니다.
struct Holding {
    Investment* inv; // 투자 종목 객체 포인터
    int quantity;    // 보유 수량
    float avgPrice;  // 평균 매수 단가
};

// 플레이어의 자산, 인벤토리 관리 및 거래 처리를 담당하는 클래스입니다.
class Player {
private:
    string name;                // 플레이어 이름
    float cash;                 // 현재 보유 현금
    vector<Holding> inventory;  // 현재 보유 중인 종목 리스트

public:
    // 플레이어를 생성하고 초기 자금을 설정합니다.
    Player(const string& n, float money);

    // Getter 메서드들
    string GetName() const { return name; }
    float GetCash() const { return cash; }
    const vector<Holding>& GetInventory() const { return inventory; }
    
    // 특정 종목을 매수합니다. 평단가를 자동으로 갱신합니다.
    bool Buy(Investment* target, int count);

    // 특정 종목을 매도합니다. 현재가로 현금화합니다.
    bool Sell(Investment* target, int count);
    
    // 총자산(현금 + 주식 가치)을 계산하여 반환합니다.
    float GetAssets() const;

    // 전체 보유 종목의 총 투자 원금을 계산합니다.
    float GetTotalInvested() const;

    // 전체 보유 종목의 현재 시장 가치를 계산합니다.
    float GetTotalEval() const;

    // 특정 종목의 현재 보유 수량을 반환합니다.
    int GetHoldingQty(const string& invName) const;

    // 플레이어 상태 정보를 출력합니다.
    void ShowStatus() const; 
};
