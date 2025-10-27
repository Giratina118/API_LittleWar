#ifndef __ONCLICKMOUSE_H__
#define __ONCLICKMOUSE_H__

#include "CUnitInfo.h"

BOOL OnClickUnitIconInPlay(CUnitInfo* playerUnit[MAX_UNIT_COUNT], int mouseClickXPos, int* nowMana_InGame, int* maxMana_InGame, int* manaLevel_InGame, float* manaDelay_InGame, CUnitInfo* g_infoUnit, CTower* g_objTower);
void OnClickUpTitleScreen(HWND* hWnd, Screen* screenState, int mouseX, int mouseY, int mouseClickXPos, int mouseClickYPos,
	int* gold, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit, CTower* g_objTower, int* maxMana, float* manaDelay, int* upgradeLevel, int* upgradeGold, int* stageLevel);
void OnClickUpReadyScreen(Screen* screenState, int mouseX, int mouseY, int mouseClickXPos, int mouseClickYPos, int* nBgX, int* maxMana_InGame, int* maxMana, float* manaDelay_InGame, float* manaDelay,
	int stageLevel, int* gold, CUnitInfo* g_infoUnit, int* upgradeLevel, int* upgradeGold, CTower* g_objTower);
void OnClickUpPauseScreen(Screen* screenState, int mouseX, int mouseY, int mouseClickXPos, int mouseClickYPos, int* nBgX, CUnitInfo* playerUnit[MAX_UNIT_COUNT], CUnitInfo* enemyUnit[MAX_UNIT_COUNT],
	CTower* g_objTower, int* gold, int* getGold, int* nowMana_InGame, int* maxMana_InGame, int maxMana, float* manaDelay_InGame, float manaDelay, float* manaTimer, int* manaLevel_InGame);
void OnClickUpStageScreen(Screen* screenState, int mouseX, int mouseY, int mouseClickXPos, int mouseClickYPos, CUnitInfo* playerUnit[MAX_UNIT_COUNT], CUnitInfo* enemyUnit[MAX_UNIT_COUNT],
	CTower* g_objTower, int* gold, int* getGold, int* nowMana_InGame, int* maxMana_InGame, int maxMana, float* manaDelay_InGame, float manaDelay, float* manaTimer, int* manaLevel_InGame);

#endif