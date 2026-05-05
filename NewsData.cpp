#include "NewsData.h"
#include <cstdlib>

NewsData::NewsData() {
    Init();
}

void NewsData::Init() {
    // [전체 시장 관련 뉴스]
    pool.emplace_back("물가 잡혔다! 소비 심리 폭발적 회복세", 0, "", 0.05f);
    pool.emplace_back("한은, 금리 전격 인하! 시장에 돈 쏟아진다", 0, "", 0.08f);
    pool.emplace_back("정부, 파격적 세제 혜택 발표! 기업들 환호", 0, "", 0.07f);
    pool.emplace_back("경제 성장률 전망 상향! '한국 경제 맑음'", 0, "", 0.06f);
    pool.emplace_back("공포의 경기 침체 습격... 증시 사정권 진입", 0, "", -0.10f);
    pool.emplace_back("유가 100달러 돌파! 물가 비상등 켜졌다", 0, "", -0.08f);
    pool.emplace_back("전쟁 위기 고조! 안전 자산으로 '돈 피난'", 0, "", -0.07f);

    // [국내 관련 뉴스]
    pool.emplace_back("수출 대박! 경상수지 '역대급' 흑자 행진", 1, "국내", 0.10f);
    pool.emplace_back("기업 이익 잭팟! 코스피 상장사 '함박웃음'", 1, "국내", 0.12f);
    pool.emplace_back("K-콘텐츠 전성시대! 관련주 수출 폭주", 1, "국내", 0.08f);
    pool.emplace_back("빚더미에 짓눌린 내수... 소비 절벽 현실화", 1, "국내", -0.06f);
    pool.emplace_back("원자재값 폭등! 제조업 '수익성 비상'", 1, "국내", -0.08f);
    
    // [해외 관련 뉴스]
    pool.emplace_back("빅테크 어닝 서프라이즈! 나스닥 '천장 뚫었다'", 1, "해외", 0.12f);
    pool.emplace_back("미 연준, 긴축 종료 선언! 증시 랠리 시작", 1, "해외", 0.09f);
    pool.emplace_back("고용 지표 강세에 금리 인하 꿈 '와르르'", 1, "해외", -0.07f);
    pool.emplace_back("글로벌 규제 칼날! 기술주 동반 추락 중", 1, "해외", -0.09f);

    // [코인 관련 뉴스]
    pool.emplace_back("가상자산 펀드 승인 임박! 코인 시장 대반등", 1, "코인", 0.15f);
    pool.emplace_back("대형 결제사 코인 도입! '진짜 돈' 되나", 1, "코인", 0.12f);
    pool.emplace_back("거래소 해킹 공포! 코인 시장 순식간에 '빙하기'", 1, "코인", -0.20f);
    pool.emplace_back("금융 당국 규제 폭탄! 가상자산 시장 급랭", 1, "코인", -0.18f);
    pool.emplace_back("묻지마 투기 광풍! '코인판' 변동성 폭발", 1, "코인", 0.25f);

    // [테마별 뉴스: 반도체]
    pool.emplace_back("AI 열풍에 반도체 품귀! '없어서 못 판다'", 1, "반도체", 0.12f);
    pool.emplace_back("초고속 메모리 독점 계약! 기술 격차 벌렸다", 1, "반도체", 0.10f);
    pool.emplace_back("반도체 잔치는 끝났다? 외국인 '탈출' 가속", 1, "반도체", -0.11f);
    pool.emplace_back("반도체 라인 셧다운! 공급 대란 불가피", 1, "반도체", -0.15f);

    // [테마별 뉴스: IT]
    pool.emplace_back("K-웹툰 세계 제패! 글로벌 점유율 1위", 1, "IT", 0.10f);
    pool.emplace_back("AI 수익화 본격 시동! IT 기업들 '돈방석'", 1, "IT", 0.08f);
    pool.emplace_back("데이터 센터 먹통! IT 신뢰도 바닥 쳤다", 1, "IT", -0.08f);
    pool.emplace_back("과징금 폭탄! 개인정보 유출에 IT 업계 '눈물'", 1, "IT", -0.10f);

    // [테마별 뉴스: 전기차]
    pool.emplace_back("보조금 쏟아진다! 전기차 시장 다시 활기", 1, "전기차", 0.15f);
    pool.emplace_back("내연차 종말 가속! 전기차 대세론 확산", 1, "전기차", 0.09f);
    pool.emplace_back("전기차 포비아 확산... 배터리 공포에 수요 급감", 1, "전기차", -0.12f);
    pool.emplace_back("인프라 부족에 발목 잡힌 전기차... 성장 둔화", 1, "전기차", -0.07f);

    // [테마별 뉴스: 게임]
    pool.emplace_back("K-게임 글로벌 제패! 역대급 성과 거뒀다", 1, "게임", 0.12f);
    pool.emplace_back("대작 신작 몰려온다! 게임주 동반 강세", 1, "게임", 0.09f);
    pool.emplace_back("규제 칼날에 게임업계 침울... 주가 하락세", 1, "게임", -0.13f);
    pool.emplace_back("신작 혹평에 매물 폭발... 게임주 '동반 하락'", 1, "게임", -0.11f);

    // [개별 종목 뉴스]
    pool.emplace_back("삼성전자 신형 폰 '완판 행진'... 기록 갈아치웠다", 2, "삼성전자", 0.08f);
    pool.emplace_back("파운드리 수주 밀린다... 삼성전자 점유율 비상", 2, "삼성전자", -0.07f);
    pool.emplace_back("엔비디아 GPU 품귀! 실적 잭팟 터진다", 2, "엔비디아", 0.10f);
    pool.emplace_back("설계 결함 루머에 엔비디아 휘청... 수급 불안", 2, "엔비디아", -0.09f);
    pool.emplace_back("네이버 AI 호평 일색! 시장 선점 '청신호'", 2, "네이버", 0.07f);
    pool.emplace_back("광고 수익 뚝... 네이버 실적 고점 논란", 2, "네이버", -0.06f);
    pool.emplace_back("카카오 체질 개선 성공! 수익성 대폭 개선", 2, "카카오", 0.06f);
    pool.emplace_back("규제 리스크 현실화! 카카오 독과점 논란", 2, "카카오", -0.08f);
    pool.emplace_back("테슬라 자율주행 판매 폭주! 매출 수직 상승", 2, "테슬라", 0.15f);
    pool.emplace_back("테슬라 인도량 쇼크! 증시 하방 압력 가중", 2, "테슬라", -0.10f);
    pool.emplace_back("에코프로 핵심 기술 특허! 리사이클링 제패", 2, "에코프로", 0.12f);
    pool.emplace_back("원자재 수급 비상! 에코프로 수익성 악화", 2, "에코프로", -0.07f);
    pool.emplace_back("펄어비스 신작 출시 '깜깜이'... 투자자들 포기하나", 2, "펄어비스", -0.14f);
    pool.emplace_back("펄어비스 시연회 잭팟! '역대급 기대작' 등극", 2, "펄어비스", 0.15f);
    pool.emplace_back("크래프톤 실적 부진... 실망 매물 쏟아진다", 2, "크래프톤", -0.12f);
    pool.emplace_back("배그 IP의 저력! 크래프톤 동접자 신기록", 2, "크래프톤", 0.10f);
    pool.emplace_back("비트코인 법정화폐 도입? 매수세 미친 듯 유입", 2, "비트코인", 0.12f);
    pool.emplace_back("비트코인 공급 감소! 가격 지지선 탄탄하다", 2, "비트코인", 0.08f);
    pool.emplace_back("고래들이 움직인다! 도지코인 폭락 주의보", 2, "도지코인", -0.15f);
    pool.emplace_back("머스크 입 열었다! 도지 결제 검토에 '폭등'", 2, "도지코인", 0.30f);

    // [이슈 및 재미 관련 뉴스]
    pool.emplace_back("버핏, 한국 주식 싹쓸이! 국장 대반등 시작", 1, "국내", 0.15f);
    pool.emplace_back("빅테크 코인 보유 선언! 제도권 편입 마침표", 1, "코인", 0.18f);
    pool.emplace_back("거물 전문가 '버블 붕괴 임박' 경고에 시장 투매", 1, "코인", -0.25f);
    pool.emplace_back("첨단 기술 전쟁 발발! 글로벌 증시 '피바다'", 1, "해외", -0.15f);
    pool.emplace_back("고물가 공포 지속... 개미들 '관망세' 확산", 0, "", -0.10f);
}

const News& NewsData::GetRandomNews() const {
    return pool[rand() % pool.size()];
}
