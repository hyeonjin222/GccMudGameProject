#include "ConsoleUI.h"
#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <limits>

namespace ConsoleUI {

    /**
     * 윈도우 터미널 설정을 초기화합니다.
     */
    void Setup() {
        // 콘솔 출력 코드페이지를 UTF-8(65001)로 설정하여 한글 깨짐 방지
        SetConsoleOutputCP(65001);

        // ANSI Escape 시퀀스(색상 코드)를 인식하도록 콘솔 모드 변경
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
    }

    /**
     * 금액을 3자리마다 쉼표가 들어간 형식으로 변환합니다.
     */
    std::string Format(long long value) {
        std::string s = std::to_string(value);
        int insertPoint = (int)s.length() - 3;
        while (insertPoint > 0 && s[insertPoint - 1] != '-') {
            s.insert(insertPoint, ",");
            insertPoint -= 3;
        }
        return s;
    }

    /**
     * 사용자 입력을 정수로 안전하게 받아 반환합니다.
     */
    int Input() {
        int input;
        if (!(std::cin >> input)) {
            std::cin.clear(); // 에러 플래그 초기화
            std::cin.ignore(1000, '\n'); // 잘못된 입력 버퍼 비우기
            return -1;
        }
        return input;
    }

    /**
     * 문자열의 실제 화면 출력 너비를 계산합니다. 색상 코드는 길이에 포함하지 않습니다.
     */
    int GetWidth(const std::string& s) {
        int width = 0;
        for (int i = 0; i < (int)s.length(); ++i) {
            // ANSI 색상 코드 시퀀스(\033[...m) 건너뛰기
            if (s[i] == '\033') {
                while (i < (int)s.length() && s[i] != 'm') i++;
                continue;
            }
            // ASCII 문자는 1칸, 멀티바이트(한글 등)는 2칸 계산
            if ((unsigned char)s[i] < 0x80) {
                width += 1;
            } else {
                // 특정 터미널에서 화살표 기호가 1칸으로 나오는 현상 대응
                if (i + 2 < (int)s.length() && (unsigned char)s[i] == 0xE2 && (unsigned char)s[i + 1] == 0x96) {
                    width += 1;
                } else {
                    width += 2;
                }
                i += 2; // UTF-8 한글은 3바이트이므로 추가 인덱스 이동
            }
        }
        return width;
    }

    /**
     * 오른쪽/왼쪽에 공백을 채워 지정한 너비를 맞춘 문자열을 반환합니다.
     */
    std::string PadR(std::string s, int target) {
        int cur = GetWidth(s);
        int pad = target - cur;
        if (pad > 0) return s + std::string(pad, ' ');
        return s;
    }

    std::string PadL(std::string s, int target) {
        int cur = GetWidth(s);
        int pad = target - cur;
        if (pad > 0) return std::string(pad, ' ') + s;
        return s;
    }

    /**
     * 메뉴 문자열의 번호 부분에 색상을 입힙니다.
     */
    std::string ColorBtn(std::string c) {
        if (c.empty()) return c;
        size_t dot = c.find('.');
        if (dot != std::string::npos) {
            return YELLOW + c.substr(0, dot + 1) + RESET + c.substr(dot + 1);
        }
        return c;
    }

    void Clear() {
        system("cls");
    }

    void Pause() {
        // 입력 버퍼를 비우고 아무 키 입력을 기다림
        std::cin.clear();
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
        std::cin.get();
    }

    void SetCursorVisible(bool visible) {
        HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(out, &cursorInfo);
        cursorInfo.bVisible = visible;
        SetConsoleCursorInfo(out, &cursorInfo);
    }

    void CenterText(std::string text, int width, bool newLine) {
        int len = GetWidth(text);
        int pad = (width - len) / 2;
        if (pad > 0) std::cout << std::string(pad, ' ');
        std::cout << text;
        if (newLine) std::cout << std::endl;
        else std::cout << std::flush;
    }

    void DrawMain(int day, float cash, float totalEval, float totalInvested, 
                  const std::string& news, const std::string& l1, const std::string& l2, 
                  const std::string& c1, const std::string& c2, const std::string& c3, const std::string& c4,
                  const std::vector<std::pair<std::string, std::pair<float, float>>>& marketData,
                  int page, const std::string& centerMsg, 
                  const std::vector<std::vector<std::string>>& inventoryData) {

        Clear();

        float rate = (totalInvested > 0) ? ((totalEval - totalInvested) / totalInvested) * 100.0f : 0;
        std::string pColor = (rate > 0) ? RED : (rate < 0) ? BLUE : "";

        // 1. 상단 정보 헤더
        std::cout << "================================================================================\n";
        std::cout << " [ " << BOLD << GREEN << day << "일차" << RESET << " | " << CYAN << "현금: " << YELLOW << PadL(Format((long long)cash), 11) << RESET << "원 | " 
                  << CYAN << "주식자산: " << YELLOW << PadL(Format((long long)totalEval), 12) << RESET << "원 (" << pColor << (rate > 0 ? "+" : "") << (int)rate << "%" << RESET << ") ]\n";
        std::cout << "================================================================================\n";

        // 2. 메인 콘텐츠 영역
        if (!centerMsg.empty()) {
            std::cout << "\n\n";
            std::cout << "                " << YELLOW << BOLD << centerMsg << RESET << "\n";
            std::cout << "\n\n";
        } else if (page == -1) { // 시장 상황 모드
            int total = (int)marketData.size(), rows = 5;
            for (int i = 0; i < rows; ++i) {
                std::cout << "  ";
                for (int col = 0; col < 2; ++col) {
                    int idx = i + (col * rows);
                    if (idx < total) {
                        float p = marketData[idx].second.first;
                        float pp = marketData[idx].second.second;
                        float chg = (pp == 0) ? 0 : ((p - pp) / pp) * 100.0f;
                        std::string sign = (chg > 0) ? "▲" : (chg < 0) ? "▼" : "-";
                        std::string color = (chg > 0) ? RED : (chg < 0) ? BLUE : "";
                        std::string nPart = BOLD + "[" + std::to_string(idx + 1) + "] " + RESET + marketData[idx].first;
                        std::string pPart = Format((long long)p) + "원", pctPart = "(" + sign + std::to_string((int)std::abs(chg)) + "%)";
                        std::cout << PadR(nPart, 16) << PadL(pPart, 12) << color << PadL(pctPart, 8) << RESET;
                    } else { std::cout << std::string(36, ' '); }
                    if (col == 0) std::cout << " | ";
                }
                std::cout << std::endl;
            }
        } else { // 보유 종목 모드
            std::cout << "  " << YELLOW << "[보유 종목]" << RESET << std::endl;
            if (inventoryData.empty()) {
                std::cout << "  보유 중인 종목이 없습니다.\n\n\n\n";
            } else {
                for (const auto& row : inventoryData) {
                    // row: {namePart, qtyPart, valPart, profitPart}
                    std::cout << "  " << PadR(row[0], 14) << PadR(row[1], 8) << std::string(10, ' ') << "  " << PadL(row[2], 14) << "  " << PadL(row[3], 16) << std::endl;
                }
                for (int i = (int)inventoryData.size(); i < 4; ++i) std::cout << std::endl;
            }
        }

        // 3. 경제 뉴스 영역
        std::cout << std::endl;
        if (centerMsg.empty()) {
            std::cout << "  " << YELLOW << "[경제 뉴스]" << RESET << " " << (news.empty() ? "현재 중요한 소식이 없습니다." : news) << "\n";
        } else {
            std::cout << "\n";
        }

        // 4. 하단 인터페이스
        std::cout << "================================================================================\n";
        std::cout << " " << PadR(l1, 52) << " |  " << PadR(ColorBtn(c1), 10) << " " << ColorBtn(c2) << "\n";
        std::cout << " " << PadR(l2, 52) << " |  " << PadR(ColorBtn(c3), 10) << " " << ColorBtn(c4) << "\n";
        std::cout << "================================================================================\n";
    }
    }

