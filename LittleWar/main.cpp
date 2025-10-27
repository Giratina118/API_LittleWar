#include <iostream>
#include <time.h>
#include <stdio.h>
#include "Timer.h"
#include "ImageManager.h"
#include "CArcher.h"
#include "CGuardian.h"
#include "CWarrior.h"
#include "CTower.h"
#include "UpdateGameProc.h"
#include "SaveLoadGame.h"
#include "PaintScreen.h"
#include "OnClickMouse.h"

#pragma warning(disable : 4996)
//#pragma comment(lib, "msimg32.lib")


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HINSTANCE g_hInst;
HWND hWndMain;
LPSTR lpszClass = "api_2d_tower_defence_MinHyeok";

SURFACEINFO g_sfBack; // 후면 버퍼 표면
SURFACEINFO g_surfaceInfo_Array[Surface_Max]; // 이미지 모음
SURFACEINFO g_btnUnit[UNIT_COUNT / 2];	// 버튼의 유닛 그림
CUnitInfo	g_infoUnit[UNIT_COUNT];		// 유닛 정보
CUnitInfo*	playerUnit[MAX_UNIT_COUNT];	// 플레이어 유닛
CUnitInfo*	enemyUnit[MAX_UNIT_COUNT];	// 적 유닛
CTower		g_objTower[2];				// 타워 정보,  0: 플레이어 타워,  1: 적 타워

Screen		screenState = Screen_Title;	// Screen_Title, Screen_Ready, Screen_Game, Screen_StageEnd
int titleFlowDir = -1;
float titleFlowTimer = 0.0f;

static int stageLevel = 1;
static int nBgX = 0;					// 배경 X값
static int gold = STARTGOLD;
int getGold = 0;
int maxMana = 50;
float manaDelay = 1.5f;
int manaLevel_InGame = 0;
int nowMana_InGame = 0, maxMana_InGame = 50;				// 현재 마나, 최대 마나
float manaDelay_InGame = 1.5f, manaTimer = 0.0f;	// 마나 회복 속도 (마나가 1 회복되는데 걸리는 시간(초))
int upgradeLevel[6] = { 1, 1, 1, 1, 1, 1 };				// 마법사(공격력, 사거리, 공격주기), 마나(최대 마나, 마나 회복 속도), 타워 체력
int upgradeGold[6] = { 500, 400, 500, 400, 500, 500 };	// 마법사(공격력, 사거리, 공격주기), 마나(최대 마나, 마나 회복 속도), 타워 체력
static int mouseClickXPos = 0, mouseClickYPos = 0;
BOOL buttonClick[10] = { 0, };	// 타이틀(이어하기, 새로시작, 종료), 준비화면(시작, 뒤로), 게임화면(일시정지), 일시정지 화면(계속, 다시, 닫기), 결과화면(다음으로)

int playerArrowNum = 0, enemyArrowNum = 0;

Timer timer;
BOOL isWin = TRUE;

float enemyCreateUnitTimer = 2.5f;	// 적 생성 시간

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASS WndClass;
	g_hInst = hInstance;

	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.lpszClassName = lpszClass;
	WndClass.lpszMenuName = NULL;
	WndClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClass(&WndClass);

	hWnd = CreateWindow(lpszClass, lpszClass, WS_SYSMENU | WS_CAPTION,
		CW_USEDEFAULT, CW_USEDEFAULT, MAP_WIDTH / 2, MAP_HEIGHT,
		NULL, (HMENU)NULL, hInstance, NULL);
	ShowWindow(hWnd, nCmdShow);
	hWndMain = hWnd;

	while (TRUE)
	{
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) // 메시지 큐에서 메시지 제거
		{
			if (Message.message == WM_QUIT) break;
			DispatchMessage(&Message);
		}
		else // 메시지가 없으면
		{
			timer.Update();

			switch (screenState)
			{
			case Screen_Title:
			case Screen_Ready:
				// 배경화면 흐름
				titleFlowTimer += timer.GetDeltaTime();
				if (titleFlowTimer > 0.01f)
				{
					titleFlowTimer = 0.0f;
					if (titleFlowDir == -1 && nBgX <= WINDOW_WIDTH - MAP_WIDTH) titleFlowDir = 1;
					else if (titleFlowDir == 1 && nBgX >= 0)					titleFlowDir = -1;
					else														nBgX += titleFlowDir * 2;
				}

				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
				break;

			case Screen_Game:
				manaTimer += timer.GetDeltaTime();			// 시간에 따른 마나 획득
				if (manaTimer > manaDelay_InGame && nowMana_InGame < maxMana_InGame)
				{
					nowMana_InGame++;
					manaTimer = 0.0f;
				}

				for (int i = 0; i < UNIT_COUNT / 2; i++) g_infoUnit[i * 2].CreateTimerUpdate(timer); // 플레이어 유닛 생성 딜레이

				KeyProc(hWnd, &nBgX, &g_objTower[0]);												 // 키 입력 (화면 좌우 스크롤, 상하 파이어볼)

				UpdateUnitProc(playerUnit, enemyUnit, &nowMana_InGame, maxMana_InGame, &getGold);	 // 필드 위 유닛 업데이트
				g_objTower[0].UpdateTower(timer);													 // 플레이어 타워 업데이트
				g_objTower[1].UpdateTower(timer);													 // 적 타워 업데이트
				g_objTower[0].GetTowerWizard()->UpdateAttack(timer, enemyUnit);						 // 타워 위 마법사 업데이트

				EnemyCreateUnit(&enemyCreateUnitTimer, enemyUnit, g_infoUnit, g_objTower);			 // 적 유닛 생성

				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);

				GameResultSet(g_objTower, &isWin, &screenState, &getGold, &stageLevel, &enemyCreateUnitTimer, g_infoUnit);	// 승리, 패배 처리
				break;

			case Screen_Pause:
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
				break;

			case Screen_StageEnd:
				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);
				break;
			}
			
			Sleep(16);
		}
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	HDC dcScreen;
	PAINTSTRUCT ps;

	switch (iMessage)
	{
	case WM_CREATE:
		srand((unsigned int)time(NULL));

		// 원본 생성 (유닛 생성 시 이 원본에 저장된 이미지를 넣기, 유닛 생성 딜레이, 마나 관리)
		g_infoUnit[0] = CWarrior();
		g_infoUnit[1] = CWarrior();
		g_infoUnit[2] = CArcher();
		g_infoUnit[3] = CArcher();
		g_infoUnit[4] = CGuardian();
		g_infoUnit[5] = CGuardian();

		// 타워
		g_objTower[0] = CTower(TRUE, stageLevel);
		g_objTower[1] = CTower(FALSE, stageLevel);

		// 표면 생성 및 비트맵 가져오기
		dcScreen = GetDC(hWnd);
		Init(dcScreen, g_surfaceInfo_Array, g_sfBack, g_btnUnit, g_infoUnit, g_objTower);
		ReleaseDC(hWnd, dcScreen);

		for (int i = 0; i < MAX_UNIT_COUNT; i++)	// 초기화
		{
			playerUnit[i] = NULL;
			enemyUnit[i] = NULL;
		}

		timer.Init();
		
		return 0;

	case WM_LBUTTONDOWN: {			// 마우스 클릭 (누를 때)
		mouseClickXPos = LOWORD(lParam);
		mouseClickYPos = HIWORD(lParam);

		switch (screenState)
		{
		case Screen_Title:			// 타이틀 화면 클릭
			if (mouseClickXPos > WINDOW_WIDTH / 2 - 150 && mouseClickXPos < WINDOW_WIDTH / 2 + 150)
			{
				if (mouseClickYPos > 250 && mouseClickYPos < 320)		buttonClick[0] = TRUE;	// 이어하기
				else if (mouseClickYPos > 325 && mouseClickYPos < 395)	buttonClick[1] = TRUE;	// 새로시작
				else if (mouseClickYPos > 400 && mouseClickYPos < 470)	buttonClick[2] = TRUE;	// 종료
			}
			break;

		case Screen_Ready:			// 준비 화면 클릭
			if (mouseClickXPos > 100 && mouseClickXPos < 300)
			{
				if (mouseClickYPos > 230 && mouseClickYPos < 290)		buttonClick[3] = TRUE;	// 게임시작
				else if (mouseClickYPos > 330 && mouseClickYPos < 390)	buttonClick[4] = TRUE;	// 타이틀로 돌아가기
			}
			break;

		case Screen_Game:			// 게임 전투 화면 클릭
			if (mouseClickYPos > 350 && mouseClickYPos < 450)	// 마우스 클릭 (유닛 생성)
			{
				if (!OnClickUnitIconInPlay(playerUnit, mouseClickXPos, &nowMana_InGame, &maxMana_InGame, &manaLevel_InGame, &manaDelay_InGame, g_infoUnit, g_objTower))	break;
			}
			if (mouseClickXPos > 1180 && mouseClickXPos < 1220 && mouseClickYPos > 5 && mouseClickYPos < 55)	buttonClick[5] = TRUE;	// 일시정지 버튼
			break;

		case Screen_Pause:			// 일시정지 화면
			if (mouseClickYPos > MAP_HEIGHT / 2 - 50 && mouseClickYPos < MAP_HEIGHT / 2 + 50)
			{
				if (mouseClickXPos > WINDOW_WIDTH / 2 - 150 && mouseClickXPos < WINDOW_WIDTH / 2 - 70)		buttonClick[6] = TRUE;	// 계속하기
				else if (mouseClickXPos > WINDOW_WIDTH / 2 - 40 && mouseClickXPos < WINDOW_WIDTH / 2 + 40)	buttonClick[7] = TRUE;	// 다시하기
				else if (mouseClickXPos > WINDOW_WIDTH / 2 + 70 && mouseClickXPos < WINDOW_WIDTH / 2 + 150)	buttonClick[8] = TRUE;	// 나가기
			}
			break;

		case Screen_StageEnd:		// 스테이지 종료 화면
			if (mouseClickXPos > WINDOW_WIDTH / 2 - 85 && mouseClickXPos < WINDOW_WIDTH / 2 + 85 && mouseClickYPos > MAP_HEIGHT / 2 + 50 && mouseClickYPos < MAP_HEIGHT / 2 + 100)
				buttonClick[9] = TRUE;	// 다음으로 버튼
			break;
		}
		break;
	}
	case WM_LBUTTONUP:				// 마우스 클릭 (뗄 때)
	{
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		for (int i = 0; i < 10; i++)	// 버튼 눌림 상태 초기화
			buttonClick[i] = FALSE;

		switch (screenState)
		{
		case Screen_Title:			// 타이틀 화면 클릭
			OnClickUpTitleScreen(&hWnd, &screenState, mouseX, mouseY, mouseClickXPos, mouseClickYPos, &gold, &enemyCreateUnitTimer, g_infoUnit, g_objTower, &maxMana, &manaDelay, upgradeLevel, upgradeGold, &stageLevel);
			break;

		case Screen_Ready:			// 준비 화면 클릭
			OnClickUpReadyScreen(&screenState, mouseX, mouseY, mouseClickXPos, mouseClickYPos, &nBgX, &maxMana_InGame, &maxMana, &manaDelay_InGame, &manaDelay, stageLevel, &gold, g_infoUnit, upgradeLevel, upgradeGold, g_objTower);
			break;

		case Screen_Game:			// 게임 전투 화면 클릭
			if (mouseX > 1180 && mouseX < 1220 && mouseY > 5 && mouseY < 55)
			{
				isWin = FALSE;
				screenState = Screen_Pause;
			}
			break;

		case Screen_Pause:			// 일시 정지 화면
			OnClickUpPauseScreen(&screenState, mouseX, mouseY, mouseClickXPos, mouseClickYPos, &nBgX, playerUnit, enemyUnit, g_objTower, &gold, &getGold, &nowMana_InGame, &maxMana_InGame, maxMana, &manaDelay_InGame, manaDelay, &manaTimer, &manaLevel_InGame);
			break;

		case Screen_StageEnd:		// 스테이지 종료 화면, 다음으로 버튼
			OnClickUpStageScreen(&screenState, mouseX, mouseY, mouseClickXPos, mouseClickYPos, playerUnit, enemyUnit, g_objTower, &gold, &getGold, &nowMana_InGame, &maxMana_InGame, maxMana, &manaDelay_InGame, manaDelay, &manaTimer, &manaLevel_InGame);
			break;
		}
		break;
	}
	case WM_PAINT:					// 화면 그리기
		HFONT newFont, oldFont;
		dcScreen = BeginPaint(hWnd, &ps);

		switch (screenState)
		{
		case Screen_Title:			// 타이틀 화면 그리기
			PaintTitleScreen(buttonClick, g_surfaceInfo_Array, nBgX, &newFont, &oldFont);
			break;

		case Screen_Ready:			// 준비 화면 그리기
			PainrReadyScreen(buttonClick, g_surfaceInfo_Array, nBgX, &newFont, &oldFont, stageLevel, gold, g_infoUnit, upgradeLevel, upgradeGold, g_objTower, maxMana, manaDelay);
			break;

		case Screen_Game:			// 게임 배틀 화면 그리기
			PaintGameScreen(g_objTower, g_infoUnit, playerUnit, enemyUnit, g_surfaceInfo_Array, g_btnUnit, buttonClick, nBgX, maxMana_InGame, nowMana_InGame, getGold, &newFont, &oldFont);
			break;

		case Screen_Pause:			// 일시 정지 화면
			PaintPauseScreen(g_surfaceInfo_Array, buttonClick);
			break;

		case Screen_StageEnd:		// 스테이지 종료 화면
			PaintStageEndScreen(g_surfaceInfo_Array, isWin, buttonClick, getGold, &newFont, &oldFont);
			break;
		}

		CompleteBlt(dcScreen, &g_sfBack);		// 화면 출력
		EndPaint(hWnd, &ps);

		return 0;

	case WM_TIMER:
		return 0;

	case WM_DESTROY:	
		g_objTower[0].GetTowerWizard()->DestroyFireBall();
		SaveFile(stageLevel, gold, g_infoUnit, upgradeLevel);
		DestroyAll(g_sfBack, g_surfaceInfo_Array[Surface_Background], g_infoUnit, playerUnit, enemyUnit);
		PostQuitMessage(0);
		return 0;
	}

	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}