#include "OnClickMouse.h"
#include "CArcher.h"
#include "CGuardian.h"
#include "CWarrior.h"
#include "SaveLoadGame.h"
#include "ImageManager.h"

// 게임 내의 유닛 소환 버튼 클릭
BOOL OnClickUnitIconInPlay(CUnitInfo* playerUnit[MAX_UNIT_COUNT], int mouseClickXPos, int* nowMana_InGame, int* maxMana_InGame, int* manaLevel_InGame, float* manaDelay_InGame, CUnitInfo* g_infoUnit, CTower* g_objTower)
{
	// 유닛 수가 최대치면 생성 불가
	int nowPlayerUnitCount = -1;
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
	{
		if (playerUnit[i] == NULL)
		{
			nowPlayerUnitCount = i;
			break;
		}
	}
	if (nowPlayerUnitCount == -1)
		return FALSE;

	// 함수로 만들기
	if (mouseClickXPos > 100 && mouseClickXPos < 200 && *nowMana_InGame >= *maxMana_InGame / 2 && *manaLevel_InGame < 10)		// 마나 업그레이드
	{
		*nowMana_InGame -= *maxMana_InGame / 2;
		*maxMana_InGame *= 1.5f;
		*manaDelay_InGame *= 0.8f;
		(*manaLevel_InGame)++;
	}
	if (mouseClickXPos > 210 && mouseClickXPos < 310 && g_infoUnit[0].CanCreateUnit(*nowMana_InGame))		// 전사
	{
		playerUnit[nowPlayerUnitCount] = new CWarrior(g_infoUnit[0], TRUE, &g_objTower[1]);
		nowPlayerUnitCount++;
		g_infoUnit[0].ResetCreateTimer();
		*nowMana_InGame -= g_infoUnit[0].GetCost();
	}
	else if (mouseClickXPos > 320 && mouseClickXPos < 420 && g_infoUnit[2].CanCreateUnit(*nowMana_InGame))	// 방패병
	{
		playerUnit[nowPlayerUnitCount] = new CArcher(g_infoUnit[2], TRUE, &g_objTower[1]);
		nowPlayerUnitCount++;
		g_infoUnit[2].ResetCreateTimer();
		*nowMana_InGame -= g_infoUnit[2].GetCost();
	}
	else if (mouseClickXPos > 430 && mouseClickXPos < 530 && g_infoUnit[4].CanCreateUnit(*nowMana_InGame))	// 궁수
	{
		playerUnit[nowPlayerUnitCount] = new CGuardian(g_infoUnit[4], TRUE, &g_objTower[1]);
		nowPlayerUnitCount++;
		g_infoUnit[4].ResetCreateTimer();
		*nowMana_InGame -= g_infoUnit[4].GetCost();
	}

	return TRUE;
}

void OnClickUpTitleScreen(HWND* hWnd, Screen* screenState, int mouseX, int mouseY, int mouseClickXPos, int mouseClickYPos,
	int* gold, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit, CTower* g_objTower, int* maxMana, float* manaDelay, int* upgradeLevel, int* upgradeGold, int* stageLevel)
{
	if (mouseX > WINDOW_WIDTH / 2 - 150 && mouseX < WINDOW_WIDTH / 2 + 150 && mouseClickXPos > WINDOW_WIDTH / 2 - 150 && mouseClickXPos < WINDOW_WIDTH / 2 + 150)
	{
		if (mouseY > 250 && mouseY < 320 && mouseClickYPos > 250 && mouseClickYPos < 320)		// 이어하기
		{
			LoadFile(gold, enemyCreateUnitTimer, g_infoUnit, g_objTower, maxMana, manaDelay, upgradeLevel, upgradeGold, stageLevel);
			*screenState = Screen_Ready;
		}
		else if (mouseY > 325 && mouseY < 395 && mouseClickYPos > 325 && mouseClickYPos < 395)	// 새로시작
		{
			ResetNewGame(gold, enemyCreateUnitTimer, g_infoUnit, g_objTower, maxMana, manaDelay, upgradeLevel, upgradeGold, stageLevel);
			*screenState = Screen_Ready;
		}
		else if (mouseY > 400 && mouseY < 470 && mouseClickYPos > 400 && mouseClickYPos < 470)	// 종료
		{
			DestroyWindow(*hWnd);
		}
	}
}

void OnClickUpReadyScreen(Screen* screenState, int mouseX, int mouseY, int mouseClickXPos, int mouseClickYPos, int* nBgX, int* maxMana_InGame, int* maxMana, float* manaDelay_InGame, float* manaDelay,
	int stageLevel, int* gold, CUnitInfo* g_infoUnit, int* upgradeLevel, int* upgradeGold, CTower* g_objTower)
{
	if (mouseX > 100 && mouseX < 300 && mouseClickXPos > 100 && mouseClickXPos < 300)
	{
		if (mouseY > 230 && mouseY < 290 && mouseClickYPos > 230 && mouseClickYPos < 290)		// 게임시작
		{
			*nBgX = 0;
			*maxMana_InGame = *maxMana;
			*manaDelay_InGame = *manaDelay;
			*screenState = Screen_Game;
		}
		else if (mouseY > 330 && mouseY < 390 && mouseClickYPos > 330 && mouseClickYPos < 390)	// 타이틀로 돌아가기
		{
			SaveFile(stageLevel, *gold, g_infoUnit, upgradeLevel);
			*screenState = Screen_Title;
		}
	}

	// 업그레이드
	if (mouseY > 40 && mouseY < 180 && mouseClickYPos > 40 && mouseClickYPos < 180)
	{
		if (mouseX > 400 && mouseX < 650 && mouseClickXPos > 400 && mouseClickXPos < 650 &&
			g_infoUnit[0].GetLevel() < UNIT_MAX_LEVEL && *gold >= g_infoUnit[0].GetLevelUpGold())	// 전사
		{
			*gold -= g_infoUnit[0].GetLevelUpGold();
			g_infoUnit[0].UnitLevelUP();
		}
		else if (mouseX > 675 && mouseX < 925 && mouseClickXPos > 675 && mouseClickXPos < 925 &&
			g_infoUnit[2].GetLevel() < UNIT_MAX_LEVEL && *gold >= g_infoUnit[2].GetLevelUpGold())	// 궁수
		{
			*gold -= g_infoUnit[2].GetLevelUpGold();
			g_infoUnit[2].UnitLevelUP();
		}
		else if (mouseX > 950 && mouseX < 1200 && mouseClickXPos > 950 && mouseClickXPos < 1200 &&
			g_infoUnit[4].GetLevel() < UNIT_MAX_LEVEL && *gold >= g_infoUnit[4].GetLevelUpGold())	// 방패병
		{
			*gold -= g_infoUnit[4].GetLevelUpGold();
			g_infoUnit[4].UnitLevelUP();
		}
	}
	else if (mouseY > 200 && mouseY < 340 && mouseClickYPos > 200 && mouseClickYPos < 340)
	{
		if (mouseX > 400 && mouseX < 650 && mouseClickXPos > 400 && mouseClickXPos < 650 && upgradeLevel[0] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[0])			// 마법사 공격력
		{
			upgradeLevel[0]++;
			*gold -= upgradeGold[0];
			g_objTower[0].GetTowerWizard()->DamageLevelUp();
			upgradeGold[0] *= 1.2f;
		}
		else if (mouseX > 675 && mouseX < 925 && mouseClickXPos > 675 && mouseClickXPos < 925 && upgradeLevel[1] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[1])	// 마법사 사거리
		{
			upgradeLevel[1]++;
			*gold -= upgradeGold[1];
			g_objTower[0].GetTowerWizard()->RangeLevelUp();
			upgradeGold[1] *= 1.2f;
		}
		else if (mouseX > 950 && mouseX < 1200 && mouseClickXPos > 950 && mouseClickXPos < 1200 && upgradeLevel[2] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[2])	// 마법사 딜레이
		{
			upgradeLevel[2]++;
			*gold -= upgradeGold[2];
			g_objTower[0].GetTowerWizard()->DelayLevelUp();
			upgradeGold[2] *= 1.2f;
		}
	}
	else if (mouseY > 360 && mouseY < 1500 && mouseClickYPos > 360 && mouseClickYPos < 1500)
	{
		if (mouseX > 400 && mouseX < 650 && mouseClickXPos > 400 && mouseClickXPos < 650 && upgradeLevel[3] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[3])			// 최대 마나
		{
			upgradeLevel[3]++;
			*gold -= upgradeGold[3];
			*maxMana += 10;
			upgradeGold[3] *= 1.2f;
		}
		else if (mouseX > 675 && mouseX < 925 && mouseClickXPos > 675 && mouseClickXPos < 925 && upgradeLevel[4] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[4])	// 마나 회복 속도
		{
			upgradeLevel[4]++;
			*gold -= upgradeGold[4];
			*manaDelay -= 0.12f;
			upgradeGold[4] *= 1.2f;
		}
		else if (mouseX > 950 && mouseX < 1200 && mouseClickXPos > 950 && mouseClickXPos < 1200 && upgradeLevel[5] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[5])	// 타워 최대 체력
		{
			upgradeLevel[5]++;
			*gold -= upgradeGold[5];
			g_objTower[0].TowerMaxHPLevelUp();
			upgradeGold[5] *= 1.2f;
		}
	}
}

void OnClickUpPauseScreen(Screen* screenState, int mouseX, int mouseY, int mouseClickXPos, int mouseClickYPos, int* nBgX, CUnitInfo* playerUnit[MAX_UNIT_COUNT], CUnitInfo* enemyUnit[MAX_UNIT_COUNT],
	CTower* g_objTower, int* gold, int* getGold, int* nowMana_InGame, int* maxMana_InGame, int maxMana, float* manaDelay_InGame, float manaDelay, float* manaTimer, int* manaLevel_InGame)
{
	if (mouseY > MAP_HEIGHT / 2 - 50 && mouseY < MAP_HEIGHT / 2 + 50 && mouseClickYPos > MAP_HEIGHT / 2 - 50 && mouseClickYPos < MAP_HEIGHT / 2 + 50)
	{
		if (mouseX > WINDOW_WIDTH / 2 - 150 && mouseX < WINDOW_WIDTH / 2 - 70 && mouseClickXPos > WINDOW_WIDTH / 2 - 150 && mouseClickXPos < WINDOW_WIDTH / 2 - 70)		// 계속하기
		{
			*screenState = Screen_Game;
		}
		else if (mouseX > WINDOW_WIDTH / 2 - 40 && mouseX < WINDOW_WIDTH / 2 + 40 && mouseClickXPos > WINDOW_WIDTH / 2 - 40 && mouseClickXPos < WINDOW_WIDTH / 2 + 40)	// 다시하기
		{
			*gold += *getGold;
			*getGold = 0;
			*nowMana_InGame = 0;
			*maxMana_InGame = maxMana;
			*manaDelay_InGame = manaDelay;
			*manaTimer = 0.0f;
			*manaLevel_InGame = 0;
			DeleteUnit(playerUnit, enemyUnit);

			g_objTower[0].GetTowerWizard()->DestroyFireBall();
			g_objTower[0].ResetTower();
			g_objTower[1].ResetTower();
			*nBgX = 0;

			*screenState = Screen_Game;
		}
		else if (mouseX > WINDOW_WIDTH / 2 + 70 && mouseX < WINDOW_WIDTH / 2 + 150 && mouseClickXPos > WINDOW_WIDTH / 2 + 70 && mouseClickXPos < WINDOW_WIDTH / 2 + 150)
			*screenState = Screen_StageEnd;	// 나가기
	}
}

void OnClickUpStageScreen(Screen* screenState, int mouseX, int mouseY, int mouseClickXPos, int mouseClickYPos, CUnitInfo* playerUnit[MAX_UNIT_COUNT], CUnitInfo* enemyUnit[MAX_UNIT_COUNT],
	CTower* g_objTower, int* gold, int* getGold, int* nowMana_InGame, int* maxMana_InGame, int maxMana, float* manaDelay_InGame, float manaDelay, float* manaTimer, int* manaLevel_InGame)
{
	// 스테이지 클리어 후 next 버튼
	if (mouseX > WINDOW_WIDTH / 2 - 85 && mouseX < WINDOW_WIDTH / 2 + 85 && mouseY > MAP_HEIGHT / 2 + 50 && mouseY < MAP_HEIGHT / 2 + 100 &&
		mouseClickXPos > WINDOW_WIDTH / 2 - 85 && mouseClickXPos < WINDOW_WIDTH / 2 + 85 && mouseClickYPos > MAP_HEIGHT / 2 + 50 && mouseClickYPos < MAP_HEIGHT / 2 + 100)
	{
		*screenState = Screen_Ready;
		*gold += *getGold;
		*getGold = 0;
		*nowMana_InGame = 0;
		*maxMana_InGame = maxMana;
		*manaDelay_InGame = manaDelay;
		*manaTimer = 0.0f;
		*manaLevel_InGame = 0;
		DeleteUnit(playerUnit, enemyUnit);

		g_objTower[0].GetTowerWizard()->DestroyFireBall();
		g_objTower[0].ResetTower();
		g_objTower[1].ResetTower();
	}
}