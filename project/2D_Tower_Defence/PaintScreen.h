#ifndef __PAINTSCREEN_H__
#define __PAINTSCREEN_H__

#include "DefineData.h"
#include "CUnitInfo.h"
#include "CTower.h"

void PaintTitleScreen(BOOL* buttonClick, SURFACEINFO* g_sfBG, SURFACEINFO* g_sfTitle, SURFACEINFO* g_sfRectBtn, int nBgX, HFONT* newFont, HFONT* oldFont);
void PainrReadyScreen(BOOL* buttonClick, SURFACEINFO* g_sfBG, SURFACEINFO* g_sfRectBtn, int nBgX, HFONT* newFont, HFONT* oldFont, int stageLevel, int gold, CUnitInfo* g_infoUnit, 
	SURFACEINFO* g_sfUpgradeBtn, int* upgradeLevel, int* upgradeGold, CTower* g_objTower, SURFACEINFO* g_sfMana, int maxMana, float manaDelay, SURFACEINFO* g_sfTowerIcon);
void PaintGameScreen(CTower* g_objTower, CUnitInfo* g_infoUnit, CUnitInfo* playerUnit[MAX_UNIT_COUNT], CUnitInfo* enemyUnit[MAX_UNIT_COUNT], SURFACEINFO* g_sfBG, SURFACEINFO* g_sfBtn,
	SURFACEINFO* g_btnUnit, SURFACEINFO* g_sfMana, SURFACEINFO* g_sfRoundBtn, BOOL* buttonClick, int nBgX, int maxMana_InGame, int nowMana_InGame, int getGold, HFONT* newFont, HFONT* oldFont);
void PaintPauseScreen(SURFACEINFO* g_sfUpgradeBtn, SURFACEINFO* g_sfRoundBtn, BOOL* buttonClick);
void PaintStageEndScreen(SURFACEINFO* g_sfUpgradeBtn, SURFACEINFO* g_sfGameResult, BOOL isWin, BOOL* buttonClick, SURFACEINFO* g_sfRectBtn, int getGold, HFONT* newFont, HFONT* oldFont);

void UpgradeUnitInfoPrint(CUnitInfo& unitInfo, int x, int y, HDC* backDcSurface, SURFACEINFO* g_sfUpgradeBtn);		// 유닛 업그레이드 정보
void UpgradeAbilityInfoPrint(int x, int y, char* upgradeInfoText, int abilityNum, HDC* backDcSurface, SURFACEINFO* g_sfUpgradeBtn, int* upgradeLevel, int* upgradeGold);	// 마법사, 마나, 타워 업그레이드 정보
void DrawGauge(HDC hdc, int x, int y, float currentValue, float maxValue);	// 자신 유닛 생성 딜레이 게이지

#endif