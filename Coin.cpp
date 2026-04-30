#include "Coin.h"
#include <cstdlib>


    //코인 생성자(종목명, 초기 가격, 테마)
Coin::Coin(const string& n, float p, const string& t)
    : Investment(n, p, t) {}

    // 코인 일일 변동폭 추가 로직
void Coin::UpdatePrice() {
    // 코인 일일 변동폭 -15% ~ +15%
    float change = ((rand() % 301) - 150) / 1000.0f;
    price += (price * change);

    // 최소 가격을 1원으로
    if (price < 1) price = 1;
}


    //뉴스에 대한 코인의 변동폭 추가 로직
void Coin::ApplyNews(float impact) {
    //코인은 뉴스에 주식보다 1.5배 더 민감하게 반응
    float randomFactor = (100 + (rand() % 101)) / 100.0f; // 1.0 ~ 2.0
    price += (price * impact * 1.5f * randomFactor);
}
