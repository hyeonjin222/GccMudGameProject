#include "GameManager.h"
#include "ConsoleUI.h"


int main() {
    // 1. 콘솔 환경 설정: 한글 유니코드(UTF-8) 출력 설정 및 ANSI 색상 코드 활성화
    ConsoleUI::Setup();
 
    // 2. 게임 관리자 객체 생성
    GameManager game;

    // 3. 게임 루프 시작
    game.Start();

    return 0;
}
