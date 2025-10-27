#ifndef __UPDATEGAMEPROC_H__
#define __UPDATEGAMEPROC_H__

#include "CUnitInfo.h"
	
void KeyProc(HWND hWnd, int* nBgX, CTower* myTower);		// Ű���� �Է� (ȭ�� �¿� ��ũ��, ȭ�� ���� ����)
void UpdateUnitProc(CUnitInfo* playerUnit[MAX_UNIT_COUNT], CUnitInfo* enemyUnit[MAX_UNIT_COUNT], int* nowMana_InGame, int maxMana_InGame, int* getGold);	 // �ʵ� �� ���� ������Ʈ
void EnemyCreateUnit(float* enemyCreateUnitTimer, CUnitInfo* enemyUnit[MAX_UNIT_COUNT], CUnitInfo g_infoUnit[UNIT_COUNT], CTower* g_objTower);				 // �� ���� ����
void StageClear(int stage, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit, CTower* g_objTower);															 // �������� Ŭ��� ���� ���̵� ���
void GameResultSet(CTower* g_objTower, BOOL* isWin, Screen* screenState, int* getGold, int* stageLevel, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit); // ���� �¸�, �й� ��� �ݿ�

#endif