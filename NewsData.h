#pragma once
#include <vector>
#include <string>

using namespace std;

// 뉴스 데이터를 정의하는 구조체입니다.
struct News {
    string headline; // 뉴스 제목
    int type;        // 적용 범위 (0:전체 시장, 1:특정 테마, 2:개별 종목)
    string target;   // 테마명 또는 종목명
    float impact;    // 가격 변동률 (예: 0.15f -> 15% 상승)

    News(string h, int t, string tg, float i) 
        : headline(h), type(t), target(tg), impact(i) {}
};

class NewsData {
private:
    vector<News> pool;

public:
    NewsData();
    void Init();
    const News& GetRandomNews() const;
};
