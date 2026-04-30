#include "Player.h"
#include <iostream>
#include <iomanip>

Player::Player(const string& n, float money) : name(n), cash(money) {}

/**
 * 종목을 매수합니다. 평단가(Average Price)를 자동으로 계산합니다.
 */
bool Player::Buy(Investment* target, int count) {
    if (count <= 0) return false;
    float cost = target->GetPrice() * count;
    
    // 자금이 충분한지 확인
    if (cash >= cost) {
        cash -= cost; // 현금 차감
        
        // 이미 보유 중인 종목인지 확인하여 평단가 업데이트
        for (auto& h : inventory) {
            if (h.inv->GetName() == target->GetName()) {
                float totalCost = (h.avgPrice * h.quantity) + cost; // 기존 매수금 + 신규 매수금
                h.quantity += count;
                h.avgPrice = totalCost / h.quantity; // 새로운 평단가 계산
                return true;
            }
        }
        // 처음 사는 종목이면 리스트에 추가
        inventory.push_back({ target, count, target->GetPrice() });
        return true;
    }
    return false; // 잔액 부족
}

/**
 * 종목을 매도합니다. 보유 수량이 충분해야 성공합니다.
 */
bool Player::Sell(Investment* target, int count) {
    if (count <= 0) return false;

    for (auto it = inventory.begin(); it != inventory.end(); ++it) {
        if (it->inv->GetName() == target->GetName()) {
            if (it->quantity >= count) {
                it->quantity -= count;
                cash += target->GetPrice() * count; // 현재가로 현금화
                
                // 수량이 0이 되면 리스트에서 삭제
                if (it->quantity == 0) {
                    inventory.erase(it);
                }
                return true;
            }
            return false; // 보유 수량 부족
        }
    }
    return false; // 해당 종목 없음
}

/**
 * 현금과 주식 평가 금액을 합산한 총자산을 반환합니다.
 */
float Player::GetAssets() const {
    return cash + GetTotalEval();
}

/**
 * 특정 종목명을 이름으로 찾아 보유 수량을 반환합니다.
 */
int Player::GetHoldingQty(const string& invName) const {
    for (const auto& h : inventory) {
        if (h.inv->GetName() == invName) return h.quantity;
    }
    return 0;
}

/**
 * 현재 보유 중인 모든 종목의 순수 매수 원금을 계산합니다.
 */
float Player::GetTotalInvested() const {
    float total = 0;
    for (const auto& h : inventory) {
        total += (h.avgPrice * h.quantity);
    }
    return total;
}

/**
 * 현재 보유 중인 모든 종목의 시장 가치(현재가 기준)를 합산합니다.
 */
float Player::GetTotalEval() const {
    float total = 0;
    for (const auto& h : inventory) {
        total += (h.inv->GetPrice() * h.quantity);
    }
    return total;
}

void Player::ShowStatus() const {
    // GameManager의 Inventory()에서 직접 UI를 그립니다.
}
