#pragma once
#include "Investment.h"

class Coin : public Investment {
public:
    // 코인 객체를 생성 (종목명, 초기 가격, 소속 테마)
    Coin(const string& n, float p, const string& t);
    
    // 가상 소멸자
    virtual ~Coin() {}

    // 코인 일일 변동폭 추가 로직
    void UpdatePrice() override; 

    // 뉴스 영향에 따른 가격 변동을 처리합니다. (주식보다 민감하게 반응)
    void ApplyNews(float impact) override;
};
