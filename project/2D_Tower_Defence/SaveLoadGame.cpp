#include "SaveLoadGame.h"
#include "DefineData.h"
#include "UpdateGameProc.h"
#include <stdio.h>

#pragma warning(disable : 4996)

// 새로 시작
void ResetNewGame(int* gold, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit, CTower* g_objTower, int* maxMana, float* manaDelay, int* upgradeLevel, int* upgradeGold)
{
	*gold = STARTGOLD;
	StageClear(1, enemyCreateUnitTimer, g_infoUnit, g_objTower);

	g_infoUnit[0].SetLevel(1);
	g_infoUnit[2].SetLevel(1);
	g_infoUnit[4].SetLevel(1);

	g_objTower[0].GetTowerWizard()->SetLevelOne();

	*maxMana = 50;
	*manaDelay = 1.5f;
	g_objTower[0].SetLevelOne();

	for (int i = 0; i < 6; i++)
		upgradeLevel[i] = 1;

	upgradeGold[0] = 500;
	upgradeGold[1] = 400;
	upgradeGold[2] = 500;
	upgradeGold[3] = 400;
	upgradeGold[4] = 500;
	upgradeGold[5] = 500;
}

// 파일 불러오기
void LoadFile(int* gold, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit, CTower* g_objTower, int* maxMana, float* manaDelay, int* upgradeLevel, int* upgradeGold, int* stageLevel)
{
	ResetNewGame(gold, enemyCreateUnitTimer, g_infoUnit, g_objTower, maxMana, manaDelay, upgradeLevel, upgradeGold);

	FILE* fin;
	fin = fopen("saveData.bin", "rb");
	if (fin != NULL)	// 저장된 파일이 있을 경우
	{
		int warriorLevel, archerLever, guardianLevel;
		fscanf(fin, "%d %d %d %d %d %d %d %d %d %d %d", stageLevel, gold, &warriorLevel, &archerLever, &guardianLevel,
			&upgradeLevel[0], &upgradeLevel[1], &upgradeLevel[2], &upgradeLevel[3], &upgradeLevel[4], &upgradeLevel[5]);

		StageClear(*stageLevel, enemyCreateUnitTimer, g_infoUnit, g_objTower);

		g_infoUnit[0].SetLevel(warriorLevel);
		g_infoUnit[2].SetLevel(archerLever);
		g_infoUnit[4].SetLevel(guardianLevel);

		for (int lv = 1; lv < upgradeLevel[0]; lv++)
		{
			g_objTower[0].GetTowerWizard()->DamageLevelUp();
			upgradeGold[0] *= 1.2f;
		}

		for (int lv = 1; lv < upgradeLevel[1]; lv++)
		{
			g_objTower[0].GetTowerWizard()->RangeLevelUp();
			upgradeGold[1] *= 1.2f;
		}

		for (int lv = 1; lv < upgradeLevel[2]; lv++)
		{
			g_objTower[0].GetTowerWizard()->DelayLevelUp();
			upgradeGold[2] *= 1.2f;
		}

		for (int lv = 1; lv < upgradeLevel[3]; lv++)
		{
			*maxMana += 10;
			upgradeGold[3] *= 1.2f;
		}

		for (int lv = 1; lv < upgradeLevel[4]; lv++)
		{
			*manaDelay -= 0.12f;
			upgradeGold[4] *= 1.2f;
		}

		for (int lv = 1; lv < upgradeLevel[5]; lv++)
		{
			g_objTower[0].TowerMaxHPLevelUp();
			upgradeGold[5] *= 1.2f;
		}

		fclose(fin);
	}
}

// 파일 저장하기
void SaveFile(int stageLevel, int gold, CUnitInfo* g_infoUnit, int* upgradeLevel)
{
	FILE* fout;
	fout = fopen("saveData.bin", "wb");
	fprintf(fout, "%d %d %d %d %d %d %d %d %d %d %d",
		stageLevel, gold, g_infoUnit[0].GetLevel(), g_infoUnit[2].GetLevel(), g_infoUnit[4].GetLevel(),
		upgradeLevel[0], upgradeLevel[1], upgradeLevel[2], upgradeLevel[3], upgradeLevel[4], upgradeLevel[5]);
	fclose(fout);
}