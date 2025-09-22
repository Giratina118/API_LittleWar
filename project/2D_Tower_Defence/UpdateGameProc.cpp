#include "UpdateGameProc.h"
#include <stdio.h>
#include "CArcher.h"
#include "CGuardian.h"
#include "CWarrior.h"
#include "ImageManager.h"

extern Timer timer;

// 키보드 입력 (화면 좌우 스크롤, 화살 상하 각도)
void __KeyProc(HWND hWnd, int* nBgX, CTower* myTower)
{
	if (GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80)	// 오른쪽으로 이동
	{
		if (*nBgX <= WINDOW_WIDTH - MAP_WIDTH)
			return;
		*nBgX -= 20;
	}
	if (GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80)		// 왼쪽으로 이동
	{
		if (*nBgX >= 0)
			return;
		*nBgX += 20;
	}
	if (GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80)		// 파이어볼 위로
	{
		if (myTower->GetTowerWizard()->GetDegree() > 70)
			return;
		myTower->GetTowerWizard()->SetDegree(TRUE, timer);
	}
	if (GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80)		// 파이어볼 아래로
	{
		if (myTower->GetTowerWizard()->GetDegree() < -50)
			return;
		myTower->GetTowerWizard()->SetDegree(FALSE, timer);
	}
}

// 필드 위 유닛 업데이트
void UpdateUnitProc(CUnitInfo* playerUnit[MAX_UNIT_COUNT], CUnitInfo* enemyUnit[MAX_UNIT_COUNT], int* nowMana_InGame, int maxMana_InGame, int* getGold)
{
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
	{
		if (playerUnit[i] != NULL)
		{
			if (playerUnit[i]->GetDieAnimTimer() < 0.0f)		// 아군 유닛 사망 처리(소멸)
			{
				delete playerUnit[i];
				playerUnit[i] = NULL;
			}
			else
				playerUnit[i]->UpdateAction(timer, enemyUnit);	// 아군 유닛 업데이트
		}

		if (enemyUnit[i] != NULL)
		{
			if (enemyUnit[i]->GetIsDie() && !enemyUnit[i]->GetisGetDieGoldMana())	// 적 유닛 사망 처리(골드, 마나 획득)
			{
				enemyUnit[i]->SetisGetDieGoldMana();
				*nowMana_InGame += enemyUnit[i]->GetDieMana();
				if (*nowMana_InGame > maxMana_InGame)
					*nowMana_InGame = maxMana_InGame;
				*getGold += enemyUnit[i]->GetDieGold();
			}

			if (enemyUnit[i]->GetDieAnimTimer() < 0.0f)			// 적 유닛 사망 처리(소멸)
			{
				delete enemyUnit[i];
				enemyUnit[i] = NULL;
			}
			else
				enemyUnit[i]->UpdateAction(timer, playerUnit);	// 적 유닛 업데이트
		}
	}
}

// 적 유닛 생성
void EnemyCreateUnit(float* enemyCreateUnitTimer, CUnitInfo* enemyUnit[MAX_UNIT_COUNT], CUnitInfo g_infoUnit[UNIT_COUNT], CTower* g_objTower)
{
	if (*enemyCreateUnitTimer > 0.0f)
		*enemyCreateUnitTimer -= timer.GetDeltaTime();
	else
	{
		int enemy = rand() % 3;
		int nowEnemyUnitCount = -1;
		for (int i = 0; i < MAX_UNIT_COUNT; i++)
		{
			if (enemyUnit[i] == NULL)
			{
				nowEnemyUnitCount = i;
				break;
			}
		}

		if (nowEnemyUnitCount == -1)
			return;

		switch (rand() % 3)
		{
		case 0:
			enemyUnit[nowEnemyUnitCount] = new CWarrior(g_infoUnit[1].GetpSfUnit(), FALSE, &g_objTower[0]);
			break;

		case 1:
			enemyUnit[nowEnemyUnitCount] = new CArcher(g_infoUnit[3].GetpSfUnit(), FALSE, &g_objTower[0]);
			break;

		case 2:
			enemyUnit[nowEnemyUnitCount] = new CGuardian(g_infoUnit[5].GetpSfUnit(), FALSE, &g_objTower[0]);
			break;

		default:
			break;
		}

		*enemyCreateUnitTimer = rand() % 4 + 1;
		nowEnemyUnitCount++;
	}
}

// 스테이지 클리어에 따른 난이도 상승
void StageClear(int stage, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit, CTower* g_objTower)
{
	*enemyCreateUnitTimer *= 0.9f;
	if (stage % 5 == 1)
		*enemyCreateUnitTimer = 2.0f;

	g_infoUnit[1].SetLevel(stage / 2);
	g_infoUnit[3].SetLevel(stage / 2);
	g_infoUnit[5].SetLevel(stage / 2);

	g_objTower[1].SetMaxHP(500 * stage);
}

// 게임 승리, 패배 결과 반영
void GameResultSet(CTower* g_objTower, BOOL* isWin, Screen* screenState, int* getGold, int* stageLevel, float* enemyCreateUnitTimer, CUnitInfo* g_infoUnit)
{
	if (g_objTower[0].TowerHPZero())			// 패배 처리
	{
		*isWin = FALSE;
		*screenState = stageEndScreen;
	}
	else if (g_objTower[1].TowerHPZero())		// 승리 처리
	{
		*isWin = TRUE;
		*getGold += *getGold * 0.5f + *stageLevel * 500;
		(*stageLevel)++;
		StageClear(*stageLevel, enemyCreateUnitTimer, g_infoUnit, g_objTower);
		*screenState = stageEndScreen;
	}
}