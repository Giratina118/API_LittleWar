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

SURFACEINFO g_sfBack; // �ĸ� ���� ǥ��
SURFACEINFO g_surfaceInfo_Array[Surface_Max]; // �̹��� ����
SURFACEINFO g_btnUnit[UNIT_COUNT / 2];	// ��ư�� ���� �׸�
CUnitInfo	g_infoUnit[UNIT_COUNT];		// ���� ����
CUnitInfo*	playerUnit[MAX_UNIT_COUNT];	// �÷��̾� ����
CUnitInfo*	enemyUnit[MAX_UNIT_COUNT];	// �� ����
CTower		g_objTower[2];				// Ÿ�� ����,  0: �÷��̾� Ÿ��,  1: �� Ÿ��

Screen		screenState = Screen_Title;	// Screen_Title, Screen_Ready, Screen_Game, Screen_StageEnd
int titleFlowDir = -1;
float titleFlowTimer = 0.0f;

static int stageLevel = 1;
static int nBgX = 0;					// ��� X��
static int gold = STARTGOLD;
int getGold = 0;
int maxMana = 50;
float manaDelay = 1.5f;
int manaLevel_InGame = 0;
int nowMana_InGame = 0, maxMana_InGame = 50;				// ���� ����, �ִ� ����
float manaDelay_InGame = 1.5f, manaTimer = 0.0f;	// ���� ȸ�� �ӵ� (������ 1 ȸ���Ǵµ� �ɸ��� �ð�(��))
int upgradeLevel[6] = { 1, 1, 1, 1, 1, 1 };				// ������(���ݷ�, ��Ÿ�, �����ֱ�), ����(�ִ� ����, ���� ȸ�� �ӵ�), Ÿ�� ü��
int upgradeGold[6] = { 500, 400, 500, 400, 500, 500 };	// ������(���ݷ�, ��Ÿ�, �����ֱ�), ����(�ִ� ����, ���� ȸ�� �ӵ�), Ÿ�� ü��
static int mouseClickXPos = 0, mouseClickYPos = 0;
BOOL buttonClick[10] = { 0, };	// Ÿ��Ʋ(�̾��ϱ�, ���ν���, ����), �غ�ȭ��(����, �ڷ�), ����ȭ��(�Ͻ�����), �Ͻ����� ȭ��(���, �ٽ�, �ݱ�), ���ȭ��(��������)

int playerArrowNum = 0, enemyArrowNum = 0;

Timer timer;
BOOL isWin = TRUE;

float enemyCreateUnitTimer = 2.5f;	// �� ���� �ð�

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
		if (PeekMessage(&Message, NULL, 0, 0, PM_REMOVE)) // �޽��� ť���� �޽��� ����
		{
			if (Message.message == WM_QUIT) break;
			DispatchMessage(&Message);
		}
		else // �޽����� ������
		{
			timer.Update();

			switch (screenState)
			{
			case Screen_Title:
			case Screen_Ready:
				// ���ȭ�� �帧
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
				manaTimer += timer.GetDeltaTime();			// �ð��� ���� ���� ȹ��
				if (manaTimer > manaDelay_InGame && nowMana_InGame < maxMana_InGame)
				{
					nowMana_InGame++;
					manaTimer = 0.0f;
				}

				for (int i = 0; i < UNIT_COUNT / 2; i++) g_infoUnit[i * 2].CreateTimerUpdate(timer); // �÷��̾� ���� ���� ������

				KeyProc(hWnd, &nBgX, &g_objTower[0]);												 // Ű �Է� (ȭ�� �¿� ��ũ��, ���� ���̾)

				UpdateUnitProc(playerUnit, enemyUnit, &nowMana_InGame, maxMana_InGame, &getGold);	 // �ʵ� �� ���� ������Ʈ
				g_objTower[0].UpdateTower(timer);													 // �÷��̾� Ÿ�� ������Ʈ
				g_objTower[1].UpdateTower(timer);													 // �� Ÿ�� ������Ʈ
				g_objTower[0].GetTowerWizard()->UpdateAttack(timer, enemyUnit);						 // Ÿ�� �� ������ ������Ʈ

				EnemyCreateUnit(&enemyCreateUnitTimer, enemyUnit, g_infoUnit, g_objTower);			 // �� ���� ����

				InvalidateRect(hWnd, NULL, TRUE);
				UpdateWindow(hWnd);

				GameResultSet(g_objTower, &isWin, &screenState, &getGold, &stageLevel, &enemyCreateUnitTimer, g_infoUnit);	// �¸�, �й� ó��
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

		// ���� ���� (���� ���� �� �� ������ ����� �̹����� �ֱ�, ���� ���� ������, ���� ����)
		g_infoUnit[0] = CWarrior();
		g_infoUnit[1] = CWarrior();
		g_infoUnit[2] = CArcher();
		g_infoUnit[3] = CArcher();
		g_infoUnit[4] = CGuardian();
		g_infoUnit[5] = CGuardian();

		// Ÿ��
		g_objTower[0] = CTower(TRUE, stageLevel);
		g_objTower[1] = CTower(FALSE, stageLevel);

		// ǥ�� ���� �� ��Ʈ�� ��������
		dcScreen = GetDC(hWnd);
		Init(dcScreen, g_surfaceInfo_Array, g_sfBack, g_btnUnit, g_infoUnit, g_objTower);
		ReleaseDC(hWnd, dcScreen);

		for (int i = 0; i < MAX_UNIT_COUNT; i++)	// �ʱ�ȭ
		{
			playerUnit[i] = NULL;
			enemyUnit[i] = NULL;
		}

		timer.Init();
		
		return 0;

	case WM_LBUTTONDOWN: {			// ���콺 Ŭ�� (���� ��)
		mouseClickXPos = LOWORD(lParam);
		mouseClickYPos = HIWORD(lParam);

		switch (screenState)
		{
		case Screen_Title:			// Ÿ��Ʋ ȭ�� Ŭ��
			if (mouseClickXPos > WINDOW_WIDTH / 2 - 150 && mouseClickXPos < WINDOW_WIDTH / 2 + 150)
			{
				if (mouseClickYPos > 250 && mouseClickYPos < 320)		buttonClick[0] = TRUE;	// �̾��ϱ�
				else if (mouseClickYPos > 325 && mouseClickYPos < 395)	buttonClick[1] = TRUE;	// ���ν���
				else if (mouseClickYPos > 400 && mouseClickYPos < 470)	buttonClick[2] = TRUE;	// ����
			}
			break;

		case Screen_Ready:			// �غ� ȭ�� Ŭ��
			if (mouseClickXPos > 100 && mouseClickXPos < 300)
			{
				if (mouseClickYPos > 230 && mouseClickYPos < 290)		buttonClick[3] = TRUE;	// ���ӽ���
				else if (mouseClickYPos > 330 && mouseClickYPos < 390)	buttonClick[4] = TRUE;	// Ÿ��Ʋ�� ���ư���
			}
			break;

		case Screen_Game:			// ���� ���� ȭ�� Ŭ��
			if (mouseClickYPos > 350 && mouseClickYPos < 450)	// ���콺 Ŭ�� (���� ����)
			{
				if (!OnClickUnitIconInPlay(playerUnit, mouseClickXPos, &nowMana_InGame, &maxMana_InGame, &manaLevel_InGame, &manaDelay_InGame, g_infoUnit, g_objTower))	break;
			}
			if (mouseClickXPos > 1180 && mouseClickXPos < 1220 && mouseClickYPos > 5 && mouseClickYPos < 55)	buttonClick[5] = TRUE;	// �Ͻ����� ��ư
			break;

		case Screen_Pause:			// �Ͻ����� ȭ��
			if (mouseClickYPos > MAP_HEIGHT / 2 - 50 && mouseClickYPos < MAP_HEIGHT / 2 + 50)
			{
				if (mouseClickXPos > WINDOW_WIDTH / 2 - 150 && mouseClickXPos < WINDOW_WIDTH / 2 - 70)		buttonClick[6] = TRUE;	// ����ϱ�
				else if (mouseClickXPos > WINDOW_WIDTH / 2 - 40 && mouseClickXPos < WINDOW_WIDTH / 2 + 40)	buttonClick[7] = TRUE;	// �ٽ��ϱ�
				else if (mouseClickXPos > WINDOW_WIDTH / 2 + 70 && mouseClickXPos < WINDOW_WIDTH / 2 + 150)	buttonClick[8] = TRUE;	// ������
			}
			break;

		case Screen_StageEnd:		// �������� ���� ȭ��
			if (mouseClickXPos > WINDOW_WIDTH / 2 - 85 && mouseClickXPos < WINDOW_WIDTH / 2 + 85 && mouseClickYPos > MAP_HEIGHT / 2 + 50 && mouseClickYPos < MAP_HEIGHT / 2 + 100)
				buttonClick[9] = TRUE;	// �������� ��ư
			break;
		}
		break;
	}
	case WM_LBUTTONUP:				// ���콺 Ŭ�� (�� ��)
	{
		int mouseX = LOWORD(lParam);
		int mouseY = HIWORD(lParam);

		for (int i = 0; i < 10; i++)	// ��ư ���� ���� �ʱ�ȭ
			buttonClick[i] = FALSE;

		switch (screenState)
		{
		case Screen_Title:			// Ÿ��Ʋ ȭ�� Ŭ��
			OnClickUpTitleScreen(&hWnd, &screenState, mouseX, mouseY, mouseClickXPos, mouseClickYPos, &gold, &enemyCreateUnitTimer, g_infoUnit, g_objTower, &maxMana, &manaDelay, upgradeLevel, upgradeGold, &stageLevel);
			break;

		case Screen_Ready:			// �غ� ȭ�� Ŭ��
			OnClickUpReadyScreen(&screenState, mouseX, mouseY, mouseClickXPos, mouseClickYPos, &nBgX, &maxMana_InGame, &maxMana, &manaDelay_InGame, &manaDelay, stageLevel, &gold, g_infoUnit, upgradeLevel, upgradeGold, g_objTower);
			break;

		case Screen_Game:			// ���� ���� ȭ�� Ŭ��
			if (mouseX > 1180 && mouseX < 1220 && mouseY > 5 && mouseY < 55)
			{
				isWin = FALSE;
				screenState = Screen_Pause;
			}
			break;

		case Screen_Pause:			// �Ͻ� ���� ȭ��
			OnClickUpPauseScreen(&screenState, mouseX, mouseY, mouseClickXPos, mouseClickYPos, &nBgX, playerUnit, enemyUnit, g_objTower, &gold, &getGold, &nowMana_InGame, &maxMana_InGame, maxMana, &manaDelay_InGame, manaDelay, &manaTimer, &manaLevel_InGame);
			break;

		case Screen_StageEnd:		// �������� ���� ȭ��, �������� ��ư
			OnClickUpStageScreen(&screenState, mouseX, mouseY, mouseClickXPos, mouseClickYPos, playerUnit, enemyUnit, g_objTower, &gold, &getGold, &nowMana_InGame, &maxMana_InGame, maxMana, &manaDelay_InGame, manaDelay, &manaTimer, &manaLevel_InGame);
			break;
		}
		break;
	}
	case WM_PAINT:					// ȭ�� �׸���
		HFONT newFont, oldFont;
		dcScreen = BeginPaint(hWnd, &ps);

		switch (screenState)
		{
		case Screen_Title:			// Ÿ��Ʋ ȭ�� �׸���
			PaintTitleScreen(buttonClick, g_surfaceInfo_Array, nBgX, &newFont, &oldFont);
			break;

		case Screen_Ready:			// �غ� ȭ�� �׸���
			PainrReadyScreen(buttonClick, g_surfaceInfo_Array, nBgX, &newFont, &oldFont, stageLevel, gold, g_infoUnit, upgradeLevel, upgradeGold, g_objTower, maxMana, manaDelay);
			break;

		case Screen_Game:			// ���� ��Ʋ ȭ�� �׸���
			PaintGameScreen(g_objTower, g_infoUnit, playerUnit, enemyUnit, g_surfaceInfo_Array, g_btnUnit, buttonClick, nBgX, maxMana_InGame, nowMana_InGame, getGold, &newFont, &oldFont);
			break;

		case Screen_Pause:			// �Ͻ� ���� ȭ��
			PaintPauseScreen(g_surfaceInfo_Array, buttonClick);
			break;

		case Screen_StageEnd:		// �������� ���� ȭ��
			PaintStageEndScreen(g_surfaceInfo_Array, isWin, buttonClick, getGold, &newFont, &oldFont);
			break;
		}

		CompleteBlt(dcScreen, &g_sfBack);		// ȭ�� ���
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