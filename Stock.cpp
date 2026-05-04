#include "Stock.h"
#include <cstdlib>

/**
 * 주식 생성자: 종목명, 초기 가격, 테마, 가격 변동 제한폭을 설정합니다.
 */
Stock::Stock(const string& n, float p, const string& t, float limit)
    : Investment(n, p, t), limitPercent(limit) {}
/**
 * 주식의 일일 가격 변동 로직입니다.
 */
void Stock::UpdatePrice() {
    // 주식은 보통 -5% ~ +5% 사이에서 완만하게 움직입니다.
    float change = ((rand() % 101) - 50) / 1000.0f; // -0.05 ~ +0.05
    price += (price * change);

    // [상하한가 제한 적용]
    // 전일 종가(prevPrice) 기준으로 limitPercent 범위 내로 가격을 제한합니다.
    float maxPrice = prevPrice * (1.0f + limitPercent);
    float minPrice = prevPrice * (1.0f - limitPercent);

    if (price > maxPrice) price = maxPrice;
    if (price < minPrice) price = minPrice;

    // 상장폐지 방지 (최소 가격 100원 유지)
    if (price < 100) price = 100;
}

/**
 * 뉴스 호재/악재가 주가에 즉각 반영됩니다.
 */
void Stock::ApplyNews(float impact) {
    // 뉴스 영향력에 약간의 랜덤성을 더해 반영합니다. (영향력의 80%~120% 수준)
    float randomFactor = (80 + (rand() % 41)) / 100.0f;
    price += (price * impact * randomFactor);

    // [뉴스 반영 후에도 상하한가 제한 적용]
    float maxPrice = prevPrice * (1.0f + limitPercent);
    float minPrice = prevPrice * (1.0f - limitPercent);

    if (price > maxPrice) price = maxPrice;
    if (price < minPrice) price = minPrice;
}
