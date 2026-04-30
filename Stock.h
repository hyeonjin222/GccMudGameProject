#pragma once
#include "Investment.h"

// Investment 클래스를 상속받는 클래스
class Stock : public Investment {
private:
    float limitPercent; // 가격 변동 제한폭 (예: 0.3f = 30%)

public:
    // 주식 종목을 생성합니다. (종목명, 초기 가격, 소속 테마, 변동 제한폭)
    Stock(const string& n, float p, const string& t, float limit = 0.3f);

    // 가상 소멸자
    virtual ~Stock() {}

    // 주식의 일일 가격 변동을 처리합니다.
    void UpdatePrice() override; 

    // 뉴스 영향에 따른 주가 변동을 처리합니다.
    void ApplyNews(float impact) override;

    // 설정된 가격 변동 제한폭을 반환합니다.
    float GetLimitPercent() const { return limitPercent; }
};
