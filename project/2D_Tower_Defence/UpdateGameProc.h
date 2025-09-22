#ifndef __UPDATEGAMEPROC_H__
#define __UPDATEGAMEPROC_H__

#include "CUnitInfo.h"
	
void __KeyProc(HWND hWnd, int* nBgX, CTower* myTower);		// 키보드 입력 (화면 좌우 스크롤, 화살 상하 각도)
void UpdateUnitProc(CUnitInfo* playerUnit[MAX_UNIT_COUNT], CUnitInfo* enemyUnit[MAX_UNIT_COUNT], int* nowMana_InGame, int maxMana_InGame, int* getGold);	 // 필드 위 유닛 업데이트
void EnemyCreateUnit(float* enemyCreateUnitTimer, CUnitInfo* enemyUnit[MAX_UNIT_COUNT], CUnitInfo g_infoUnit[UNIT_COUNT], CTower* g_objTower);				 // 적 유닛 생성
void StageClear(int stage, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit, CTower* g_objTower);															 // 스테이지 클리어에 따른 난이도 상승
void GameResultSet(CTower* g_objTower, BOOL* isWin, Screen* screenState, int* getGold, int* stageLevel, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit); // 게임 승리, 패배 결과 반영

#endif