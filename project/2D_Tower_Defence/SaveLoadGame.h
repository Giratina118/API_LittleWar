#ifndef __SAVELOADGAME_H__
#define __SAVELOADGAME_H__

#include "CUnitInfo.h"

void ResetNewGame(int* gold, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit, CTower* g_objTower, int* maxMana, float* manaDelay, int* upgradeLevel, int* upgradeGold);
void LoadFile(int* gold, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit, CTower* g_objTower, int* maxMana, float* manaDelay, int* upgradeLevel, int* upgradeGold, int* stageLevel);
void SaveFile(int stageLevel, int gold, CUnitInfo* g_infoUnit, int* upgradeLevel);

#endif