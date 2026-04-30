#pragma once
#include <string>
#include <iostream>

using namespace std;

    // 주식, 코인의 공통 속성과 기능을 정의하는 클래스
class Investment {
protected:
    string name;      // 종목 이름
    float price;      // 현재 시장가
    float prevPrice;  // 전일 종가
    string theme;     // 종목 테마 (예: 반도체, IT, 게임 등)

public:
    // 투자 종목 기본 정보를 초기화합니다.
    Investment(const string& n, float p, const string& t);

    // 가상 소멸자
    virtual ~Investment();

    // 순수 가상 함수: 일일 주가 변동 로직 (자식 클래스에서 구현)
    virtual void UpdatePrice() = 0;

    // 순수 가상 함수: 뉴스 영향에 따른 주가 변동 로직 (자식 클래스에서 구현)
    virtual void ApplyNews(float impact) = 0;

    // 현재 가격을 전일 가격으로 기록합니다.
    void RecordPrevPrice();

    // Getter
    string GetName() const { return name; }
    float GetPrice() const { return price; }
    float GetPrevPrice() const { return prevPrice; }
    string GetTheme() const { return theme; }
};
