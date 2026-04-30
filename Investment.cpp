#include "Investment.h"

Investment::Investment(const string& n, float p, const string& t) 
    : name(n), price(p), prevPrice(p), theme(t) {}

Investment::~Investment() {}

// 현재 가격을 전일 가격으로 기록
void Investment::RecordPrevPrice() {
    prevPrice = price;
}
