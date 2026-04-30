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
    InitN(); // 뉴스 시스템 초기화
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
    market.emplace_back(make_unique<Stock>("삼성전자", 75000, "반도체"));
    market.emplace_back(make_unique<Stock>("엔비디아", 1200000, "반도체"));
    market.emplace_back(make_unique<Stock>("네이버", 210000, "IT"));
    market.emplace_back(make_unique<Stock>("카카오", 55000, "IT"));
    market.emplace_back(make_unique<Stock>("테슬라", 250000, "전기차"));
    market.emplace_back(make_unique<Stock>("에코프로", 580000, "전기차"));
    market.emplace_back(make_unique<Stock>("펄어비스", 40000, "게임"));
    market.emplace_back(make_unique<Stock>("크래프톤", 250000, "게임"));
    market.emplace_back(make_unique<Coin>("비트코인", 95000000, "코인"));
    market.emplace_back(make_unique<Coin>("도지코인", 250, "코인"));
}

/**
 * 게임의 재미와 몰입도를 높이기 위한 60여 종의 현실 반영 뉴스를 초기화합니다.
 */
void GameManager::InitN() {
    // [전체 시장 관련 뉴스]
    pool.emplace_back("트럼프 당선 확률 폭등! '강력한 관세' 예고에 시장 발칵.", 0, "", -0.15f );
    pool.emplace_back("연준 파월 의장, '비둘기' 변신! 유동성 파티가 다시 시작된다.", 0, "", 0.12f );
    pool.emplace_back("전 세계적 AI 거품론 확산! 자산 시장 일제히 '패닉 셀' 발생.", 0, "", -0.20f );
    pool.emplace_back("환율 1,500원 돌파 위기! 외인들의 '코리아 엑소더스' 가속화.", 0, "", -0.10f );
    pool.emplace_back("워렌 버핏, 한국 주식 비밀 매수설?! '코리아 디스카운트' 해소 기대.", 0, "", 0.15f );
    pool.emplace_back("중동 분쟁 격화로 국제 유가 폭등! 스태그플레이션 공포 확산.", 0, "", -0.18f );
    pool.emplace_back("양자 컴퓨팅 기술 돌파구 마련! 전 산업군 디지털 혁명 예고.", 0, "", 0.14f );
    pool.emplace_back("글로벌 전염병 재유행 조짐에 비대면 관련 자산 급등.", 0, "", 0.08f );
    pool.emplace_back("G7 정상회의, 전 세계적 법인세 인상 합의! 기업 수익성 악화 우려.", 0, "", -0.07f );
    pool.emplace_back("한국은행, 깜짝 금리 인하 단행! 부동산 및 자산 시장 온기 돌까.", 0, "", 0.10f );
    pool.emplace_back("미국 고용지표 예상치 대폭 상회! '노랜딩' 시나리오에 시장 안도.", 0, "", 0.09f );
    pool.emplace_back("유럽 에너지 위기 재발? 가스 가격 폭등에 글로벌 공급망 비상.", 0, "", -0.12f );

    // [테마별 뉴스: 반도체]
    pool.emplace_back("샘 올트먼, '7조 달러' 펀딩 추진 소식에 반도체 섹터 광풍!", 1, "반도체", 0.25f );
    pool.emplace_back("미국, 대중국 반도체 수출 전면 금지 검토! 국내 업계 타격 우려.", 1, "반도체", -0.18f );
    pool.emplace_back("차세대 HBM 칩 없어서 못 판다! '반도체 슈퍼 사이클' 진입.", 1, "반도체", 0.15f );
    pool.emplace_back("반도체 핵심 원자재 '네온 가스' 공급 중단 위기! 생산 차질 우려.", 1, "반도체", -0.12f );
    pool.emplace_back("애플, 자체 칩 생산 비중 확대 선언! 파운드리 업계 대형 호재.", 1, "반도체", 0.13f );
    pool.emplace_back("반도체 미세공정 한계 돌파! 2나노 양산 경쟁 본격화 소식.", 1, "반도체", 0.11f );
    pool.emplace_back("PC 및 스마트폰 수요 회복 지연... 메모리 반도체 재고 누적 경고.", 1, "반도체", -0.09f );
    pool.emplace_back("인텔-ARM 동맹 체결? 설계부터 생산까지 반도체 지형도 재편 예고.", 1, "반도체", 0.16f );

    // [테마별 뉴스: IT]
    pool.emplace_back("EU, 네이버·카카오 정조준! '디지털시장법' 위반 시 대규모 과징금.", 1, "IT", -0.12f );
    pool.emplace_back("생성형 AI 'K-GPT' 정식 출시! 토종 플랫폼의 반격 시작.", 1, "IT", 0.15f );
    pool.emplace_back("정부, 데이터 센터 지방 분산법 통과... IT 기업 규제 리스크 부각.", 1, "IT", -0.10f );
    pool.emplace_back("메타버스 시장의 부활? 애플 비전프로 후속작 기대감에 IT주 강세.", 1, "IT", 0.11f );
    pool.emplace_back("구글 해체 판결 임박! 글로벌 빅테크 기업들 공포에 질리다.", 1, "IT", -0.14f );
    pool.emplace_back("전 국민 메신저 장애 보상 규모 눈덩이... 플랫폼 신뢰도 타격.", 1, "IT", -0.08f );
    pool.emplace_back("정부, 클라우드 산업 육성에 5조 투자! 국내 IT 기업들 수혜 전망.", 1, "IT", 0.14f );
    pool.emplace_back("AI 사이버 보안 위협 급증! 보안 솔루션 업체들 주가 반등.", 1, "IT", 0.10f );

    // [테마별 뉴스: 전기차]
    pool.emplace_back("트럼프 '내 사전엔 전기차 없다'... 보조금 폐지 법안 발의 소식.", 1, "전기차", -0.25f );
    pool.emplace_back("꿈의 '전고체 배터리' 개발 성공 소식에 이차전지주 일제히 폭등!", 1, "전기차", 0.22f );
    pool.emplace_back("전 세계 전기차 캐즘(수요 정체) 심화... 재고가 산더미처럼 쌓여간다.", 1, "전기차", -0.15f );
    pool.emplace_back("테슬라 자율주행 택시 '로보택시' 전국 상용화 승인!", 1, "전기차", 0.20f );
    pool.emplace_back("전기차 화재 공포 확산에 지하주차장 출입 금지... 배터리주 투매.", 1, "전기차", -0.18f );
    pool.emplace_back("폐배터리 재활용 기술 상용화 임박! 순환경제 핵심 테마로 부상.", 1, "전기차", 0.12f );
    pool.emplace_back("리튬 가격 바닥 찍었나? 양극재 업체들 마진 개선 기대감.", 1, "전기차", 0.08f );
    pool.emplace_back("주요 완성차 업체, 하이브리드 비중 확대 발표... 전기차 속도 조절.", 1, "전기차", -0.11f );

    // [테마별 뉴스: 게임]
    pool.emplace_back("중국 판호 대량 개방! K-게임의 '대륙 습격'이 시작된다.", 1, "게임", 0.20f );
    pool.emplace_back("확률형 아이템 규제안 국회 통과! 게임사 수익성 악화 불가피.", 1, "게임", -0.15f );
    pool.emplace_back("글로벌 게임 어워드 K-게임 싹쓸이! 종목별 매수세 유입.", 1, "게임", 0.12f );
    pool.emplace_back("사우디 국부펀드(PIF), 국내 게임사 지분 추가 매입 소식!", 1, "게임", 0.18f );
    pool.emplace_back("신작 모바일 게임 '리니지W2' 매출 순위 1위 등극 실패 소식.", 1, "게임", -0.10f );
    pool.emplace_back("게임 산업 내 AI 번역 기술 도입으로 해외 진출 비용 대폭 절감.", 1, "게임", 0.07f );
    pool.emplace_back("콘솔 시장 확대 소식! PC 위주 국내 게임사들에게 새로운 기회.", 1, "게임", 0.09f );
    pool.emplace_back("대형 신작 출시 연기 잇따라... 게임 섹터 투자 심리 위축.", 1, "게임", -0.13f );

    // [테마별 뉴스: 코인]
    pool.emplace_back("머스크 '내 아들도 도지홀더'... 밈코인 투기 광풍 재점화!", 1, "코인", 0.35f );
    pool.emplace_back("미 SEC, 알트코인 증권성 조사 착수! 코인 시장 '검은 수요일'.", 1, "코인", -0.30f );
    pool.emplace_back("비트코인 전략 자산화 법안 미 의회 제출! '디지털 금' 시대 열리나.", 1, "코인", 0.20f );
    pool.emplace_back("비트코인 반감기 도래! 공급 감소에 따른 가격 폭등 시나리오.", 1, "코인", 0.28f );
    pool.emplace_back("아마존, 비트코인 결제 시스템 도입 루머에 시장 후끈!", 1, "코인", 0.25f );
    pool.emplace_back("유명 코인 거래소 '해킹' 발생... 고객 자산 수천억 증발 소식.", 1, "코인", -0.25f );
    pool.emplace_back("이더리움 현물 ETF 승인 완료! 기관 자금 유입 본격화 전망.", 1, "코인", 0.18f );
    pool.emplace_back("주요국 중앙은행 디지털화폐(CBDC) 속도전... 가상자산 입지 위협.", 1, "코인", -0.15f );
    pool.emplace_back("비트코인 1억 돌파! 포모(FOMO) 현상에 신규 개미들 대거 유입.", 1, "코인", 0.22f );
    pool.emplace_back("유명 코인 유튜버 '선취매' 적발... 코인판 도덕적 해이 논란.", 1, "코인", -0.10f );

    // [개별 종목 관련 뉴스]
    pool.emplace_back("삼성전자, '꿈의 1나노' 양산 성공! TSMC 꺾고 파운드리 1위 탈환하나.", 2, "삼성전자", 0.18f );
    pool.emplace_back("삼성전자 노조 창사 이래 첫 전면 파업 선언! 생산 차질 위기.", 2, "삼성전자", -0.12f );
    pool.emplace_back("삼성전자, 갤럭시 S시리즈 역대급 사전 예약... 모바일 부문 실적 견인.", 2, "삼성전자", 0.10f );
    pool.emplace_back("엔비디아 CEO 젠슨 황 은퇴 루머?! 시장은 '포스트 젠슨' 공포 중.", 2, "엔비디아", -0.15f );
    pool.emplace_back("엔비디아, 차세대 블랙웰 칩 주문 폭주! 실적 상향 조정.", 2, "엔비디아", 0.22f );
    pool.emplace_back("테슬라, 완전 자율주행(FSD) 중국 진출 최종 승인! 머스크의 승리.", 2, "테슬라", 0.20f );
    pool.emplace_back("테슬라 '모델2' 저가형 전기차 출시 무기한 연기 소식.", 2, "테슬라", -0.15f );
    pool.emplace_back("도지코인, 화성 가는 스페이스X 로켓에 로고 새긴다! 'Doge to the moon!'", 2, "도지코인", 0.45f );
    pool.emplace_back("도지코인 개발자 '사실 도지는 장난이었다' 발언에 실망 매물 출현.", 2, "도지코인", -0.20f );
    pool.emplace_back("크래프톤, '배틀그라운드2' 플레이 영상 유출! 'K-엘든링' 찬사.", 2, "크래프톤", 0.25f );
    pool.emplace_back("크래프톤 최대 주주 텐센트, 지분 대량 매각 공시! 블록딜 공포.", 2, "크래프톤", -0.18f );
    pool.emplace_back("펄어비스, '붉은사막' 출시일 또 연기... 게이머들 '이제 안 속는다'.", 2, "펄어비스", -0.22f );
    pool.emplace_back("펄어비스, 신작 '도깨비' 모바일 버전 판호 획득 소식에 급등!", 2, "펄어비스", 0.20f );
    pool.emplace_back("카카오 의장, 자본시장법 위반 혐의로 구속 심사... 오너 리스크.", 2, "카카오", -0.25f );
    pool.emplace_back("카카오뱅크, 역대급 실적 달성! 금융 생태계 파괴자로 등극.", 2, "카카오", 0.15f );
    pool.emplace_back("네이버, '라인 야후' 지분 매각 압박에 '탈일본' 선언... 불확실성 증폭.", 2, "네이버", -0.12f );
    pool.emplace_back("네이버 웹툰, 나스닥 상장 대박! 기업 가치 10조 원 평가.", 2, "네이버", 0.16f );
    pool.emplace_back("에코프로, 아프리카 리튬 광산 독점 개발권 획득! 원가 경쟁력 압도.", 2, "에코프로", 0.22f );
    pool.emplace_back("에코프로 전 회장, 내부자 거래 혐의로 실형 확정... 기업 이미지 타격.", 2, "에코프로", -0.15f );
    pool.emplace_back("비트코인, 현물 ETF 거래량 폭증하며 디지털 금으로서의 입지 굳히기.", 2, "비트코인", 0.15f );
    pool.emplace_back("유명 고래들, 비트코인 거래소 입금 급증... 단기 폭락 경고.", 2, "비트코인", -0.12f );
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

    // 80% 확률로 경제 뉴스 발생
    if (rand() % 10 < 8) {
        News n = pool[rand() % pool.size()];
        string color = (n.impact > 0) ? RED : (n.impact < 0) ? BLUE : "";
        news = color + "[속보] " + n.headline + RESET;
        
        // 뉴스 범위에 따른 효과 적용
        for (auto& inv : market) {
            if (n.type == 0) inv->ApplyNews(n.impact);
            else if (n.type == 1 && inv->GetTheme() == n.target) inv->ApplyNews(n.impact);
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
        ConsoleUI::CenterText(BOLD + RED + "[ 야수의 심장 : 영끌 시뮬레이터 ]" + RESET);
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
        "부자가 되고 싶다.",
        "내가 가진 돈 300만원.",
        "주식으로 인생역전을 해보는거야!!"
    };

    for (const string& s : stories) {
        Clear();
        cout << "\n\n\n\n\n\n\n\n";
        ConsoleUI::CenterText(BOLD + s + RESET);
        this_thread::sleep_for(chrono::milliseconds(3500));
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
