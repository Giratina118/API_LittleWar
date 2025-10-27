#include "OnClickMouse.h"
#include "CArcher.h"
#include "CGuardian.h"
#include "CWarrior.h"
#include "SaveLoadGame.h"
#include "ImageManager.h"

// ���� ���� ���� ��ȯ ��ư Ŭ��
BOOL OnClickUnitIconInPlay(CUnitInfo* playerUnit[MAX_UNIT_COUNT], int mouseClickXPos, int* nowMana_InGame, int* maxMana_InGame, int* manaLevel_InGame, float* manaDelay_InGame, CUnitInfo* g_infoUnit, CTower* g_objTower)
{
	// ���� ���� �ִ�ġ�� ���� �Ұ�
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

	// �Լ��� �����
	if (mouseClickXPos > 100 && mouseClickXPos < 200 && *nowMana_InGame >= *maxMana_InGame / 2 && *manaLevel_InGame < 10)		// ���� ���׷��̵�
	{
		*nowMana_InGame -= *maxMana_InGame / 2;
		*maxMana_InGame *= 1.5f;
		*manaDelay_InGame *= 0.8f;
		(*manaLevel_InGame)++;
	}
	if (mouseClickXPos > 210 && mouseClickXPos < 310 && g_infoUnit[0].CanCreateUnit(*nowMana_InGame))		// ����
	{
		playerUnit[nowPlayerUnitCount] = new CWarrior(g_infoUnit[0], TRUE, &g_objTower[1]);
		nowPlayerUnitCount++;
		g_infoUnit[0].ResetCreateTimer();
		*nowMana_InGame -= g_infoUnit[0].GetCost();
	}
	else if (mouseClickXPos > 320 && mouseClickXPos < 420 && g_infoUnit[2].CanCreateUnit(*nowMana_InGame))	// ���к�
	{
		playerUnit[nowPlayerUnitCount] = new CArcher(g_infoUnit[2], TRUE, &g_objTower[1]);
		nowPlayerUnitCount++;
		g_infoUnit[2].ResetCreateTimer();
		*nowMana_InGame -= g_infoUnit[2].GetCost();
	}
	else if (mouseClickXPos > 430 && mouseClickXPos < 530 && g_infoUnit[4].CanCreateUnit(*nowMana_InGame))	// �ü�
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
		if (mouseY > 250 && mouseY < 320 && mouseClickYPos > 250 && mouseClickYPos < 320)		// �̾��ϱ�
		{
			LoadFile(gold, enemyCreateUnitTimer, g_infoUnit, g_objTower, maxMana, manaDelay, upgradeLevel, upgradeGold, stageLevel);
			*screenState = Screen_Ready;
		}
		else if (mouseY > 325 && mouseY < 395 && mouseClickYPos > 325 && mouseClickYPos < 395)	// ���ν���
		{
			ResetNewGame(gold, enemyCreateUnitTimer, g_infoUnit, g_objTower, maxMana, manaDelay, upgradeLevel, upgradeGold, stageLevel);
			*screenState = Screen_Ready;
		}
		else if (mouseY > 400 && mouseY < 470 && mouseClickYPos > 400 && mouseClickYPos < 470)	// ����
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
		if (mouseY > 230 && mouseY < 290 && mouseClickYPos > 230 && mouseClickYPos < 290)		// ���ӽ���
		{
			*nBgX = 0;
			*maxMana_InGame = *maxMana;
			*manaDelay_InGame = *manaDelay;
			*screenState = Screen_Game;
		}
		else if (mouseY > 330 && mouseY < 390 && mouseClickYPos > 330 && mouseClickYPos < 390)	// Ÿ��Ʋ�� ���ư���
		{
			SaveFile(stageLevel, *gold, g_infoUnit, upgradeLevel);
			*screenState = Screen_Title;
		}
	}

	// ���׷��̵�
	if (mouseY > 40 && mouseY < 180 && mouseClickYPos > 40 && mouseClickYPos < 180)
	{
		if (mouseX > 400 && mouseX < 650 && mouseClickXPos > 400 && mouseClickXPos < 650 &&
			g_infoUnit[0].GetLevel() < UNIT_MAX_LEVEL && *gold >= g_infoUnit[0].GetLevelUpGold())	// ����
		{
			*gold -= g_infoUnit[0].GetLevelUpGold();
			g_infoUnit[0].UnitLevelUP();
		}
		else if (mouseX > 675 && mouseX < 925 && mouseClickXPos > 675 && mouseClickXPos < 925 &&
			g_infoUnit[2].GetLevel() < UNIT_MAX_LEVEL && *gold >= g_infoUnit[2].GetLevelUpGold())	// �ü�
		{
			*gold -= g_infoUnit[2].GetLevelUpGold();
			g_infoUnit[2].UnitLevelUP();
		}
		else if (mouseX > 950 && mouseX < 1200 && mouseClickXPos > 950 && mouseClickXPos < 1200 &&
			g_infoUnit[4].GetLevel() < UNIT_MAX_LEVEL && *gold >= g_infoUnit[4].GetLevelUpGold())	// ���к�
		{
			*gold -= g_infoUnit[4].GetLevelUpGold();
			g_infoUnit[4].UnitLevelUP();
		}
	}
	else if (mouseY > 200 && mouseY < 340 && mouseClickYPos > 200 && mouseClickYPos < 340)
	{
		if (mouseX > 400 && mouseX < 650 && mouseClickXPos > 400 && mouseClickXPos < 650 && upgradeLevel[0] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[0])			// ������ ���ݷ�
		{
			upgradeLevel[0]++;
			*gold -= upgradeGold[0];
			g_objTower[0].GetTowerWizard()->DamageLevelUp();
			upgradeGold[0] *= 1.2f;
		}
		else if (mouseX > 675 && mouseX < 925 && mouseClickXPos > 675 && mouseClickXPos < 925 && upgradeLevel[1] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[1])	// ������ ��Ÿ�
		{
			upgradeLevel[1]++;
			*gold -= upgradeGold[1];
			g_objTower[0].GetTowerWizard()->RangeLevelUp();
			upgradeGold[1] *= 1.2f;
		}
		else if (mouseX > 950 && mouseX < 1200 && mouseClickXPos > 950 && mouseClickXPos < 1200 && upgradeLevel[2] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[2])	// ������ ������
		{
			upgradeLevel[2]++;
			*gold -= upgradeGold[2];
			g_objTower[0].GetTowerWizard()->DelayLevelUp();
			upgradeGold[2] *= 1.2f;
		}
	}
	else if (mouseY > 360 && mouseY < 1500 && mouseClickYPos > 360 && mouseClickYPos < 1500)
	{
		if (mouseX > 400 && mouseX < 650 && mouseClickXPos > 400 && mouseClickXPos < 650 && upgradeLevel[3] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[3])			// �ִ� ����
		{
			upgradeLevel[3]++;
			*gold -= upgradeGold[3];
			*maxMana += 10;
			upgradeGold[3] *= 1.2f;
		}
		else if (mouseX > 675 && mouseX < 925 && mouseClickXPos > 675 && mouseClickXPos < 925 && upgradeLevel[4] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[4])	// ���� ȸ�� �ӵ�
		{
			upgradeLevel[4]++;
			*gold -= upgradeGold[4];
			*manaDelay -= 0.12f;
			upgradeGold[4] *= 1.2f;
		}
		else if (mouseX > 950 && mouseX < 1200 && mouseClickXPos > 950 && mouseClickXPos < 1200 && upgradeLevel[5] < ABILITY_MAX_LEVEL && *gold >= upgradeGold[5])	// Ÿ�� �ִ� ü��
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
		if (mouseX > WINDOW_WIDTH / 2 - 150 && mouseX < WINDOW_WIDTH / 2 - 70 && mouseClickXPos > WINDOW_WIDTH / 2 - 150 && mouseClickXPos < WINDOW_WIDTH / 2 - 70)		// ����ϱ�
		{
			*screenState = Screen_Game;
		}
		else if (mouseX > WINDOW_WIDTH / 2 - 40 && mouseX < WINDOW_WIDTH / 2 + 40 && mouseClickXPos > WINDOW_WIDTH / 2 - 40 && mouseClickXPos < WINDOW_WIDTH / 2 + 40)	// �ٽ��ϱ�
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
			*screenState = Screen_StageEnd;	// ������
	}
}

void OnClickUpStageScreen(Screen* screenState, int mouseX, int mouseY, int mouseClickXPos, int mouseClickYPos, CUnitInfo* playerUnit[MAX_UNIT_COUNT], CUnitInfo* enemyUnit[MAX_UNIT_COUNT],
	CTower* g_objTower, int* gold, int* getGold, int* nowMana_InGame, int* maxMana_InGame, int maxMana, float* manaDelay_InGame, float manaDelay, float* manaTimer, int* manaLevel_InGame)
{
	// �������� Ŭ���� �� next ��ư
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