#pragma once
#include <string>
#include <vector>

// 콘솔 화면 출력, 입력 및 시스템 설정을 담당하는 유틸리티 네임스페이스입니다.
// ANSI Escape Sequence를 사용하여 색상 출력을 지원합니다.
namespace ConsoleUI {
    // 콘솔 환경을 설정합니다. (UTF-8 출력 및 ANSI 색상 활성화)
    void Setup();

    // 숫자를 세 자리마다 콤마가 포함된 문자열로 변환합니다. (예: 1,234,567)
    std::string Format(long long value);

    // 사용자로부터 안전하게 정수 입력을 받습니다. (잘못된 입력 시 무한 루프 방지)
    int Input();

    // ANSI 색상 코드를 제외한 문자열의 실제 출력 너비를 계산합니다. (한글 2칸, 영문 1칸)
    int GetWidth(const std::string& s);

    // 지정한 너비에 맞춰 문자열 오른쪽에 공백을 채웁니다. (우측 정렬용)
    std::string PadR(std::string s, int target);

    // 지정한 너비에 맞춰 문자열 왼쪽에 공백을 채웁니다. (좌측 정렬용)
    std::string PadL(std::string s, int target);

    // 메뉴 버튼의 숫자 부분만 노란색으로 강조합니다. (예: 1. 매수)
    std::string ColorBtn(std::string c);

    // 콘솔 화면을 깨끗하게 지웁니다.
    void Clear();

    // 사용자가 아무 키나 누를 때까지 대기합니다.
    void Pause();

    // 콘솔 커서를 숨기거나 표시합니다.
    void SetCursorVisible(bool visible);

    // 텍스트를 화면 중앙에 정렬하여 출력합니다.
    void CenterText(std::string text, int width = 80, bool newLine = true);

    // 게임의 메인 화면 프레임을 그립니다.
    void DrawMain(int day, float cash, float totalEval, float totalInvested, 
                  const std::string& news, const std::string& l1, const std::string& l2, 
                  const std::string& c1, const std::string& c2, const std::string& c3, const std::string& c4,
                  const std::vector<std::pair<std::string, std::pair<float, float>>>& marketData,
                  int page = -1, const std::string& centerMsg = "", 
                  const std::vector<std::vector<std::string>>& inventoryData = {});

    // ANSI 색상 상수 정의
    const std::string RESET = "\033[0m";   // 색상 초기화
    const std::string BOLD = "\033[1m";    // 굵게
    const std::string RED = "\033[31m";    // 빨간색 (하락/경고)
    const std::string GREEN = "\033[32m";  // 초록색 (보유량)
    const std::string YELLOW = "\033[33m"; // 노란색 (강조/번호)
    const std::string BLUE = "\033[34m";   // 파란색 (상승/정보)
    const std::string CYAN = "\033[36m";   // 청록색 (헤더 정보)
}
