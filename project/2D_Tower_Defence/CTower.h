#ifndef __CTOWER_H__
#define __CTOWER_H__

//#include <Windows.h>
#include "DefineData.h"
#include "Timer.h"
#include "CTowerWizard.h"

class CTower
{
public:
	CTower();
	CTower(BOOL player, int level);
	~CTower();

	SURFACEINFO* GetpSfUnit() { return g_sfTower; }
	CTowerWizard* GetTowerWizard() { return &towerWizard; }
	int GetMaxHP() { return maxHP; }
	int GetDefenceLine() { return defenceLine; }
	void SetMaxHP(int hp) { maxHP = hp;		nowHP = maxHP; }
	void SetTowerState(int state) { towerState = state; }
	void SetxPos(int x) { xPos = x; }
	void SetDefenceLine(BOOL leftTower);
	void SetLevelOne();

	void UpdateTower(Timer timer);
	void DrawSurface(HDC dcDst, int nBgX);				// 타워 그리기
	void PrintTowerHP(HDC dcDst, int nBgX);				// 타워 체력 표시
	void TakeDamage(int dmg, Timer timer);

	BOOL TowerHPZero();
	void ResetTower();
	void TowerMaxHPLevelUp() { maxHP += 500;	nowHP = maxHP; }

private:
	SURFACEINFO	g_sfTower[3];			// 0: 기본,  1: 공격받음,  2: 파괴
	int			towerState;				// 0: 기본,  1: 공격받음,  2: 파괴
	int			xPos;					// 타워의 x좌표
	int			yPos;					// 타워의 y좌표
	int			hpXPos;					// 타워 hp 표시 x좌표
	int			hpYPos;					// 타워 hp 표시 y좌표
	int			maxHP;					// 최대 체력
	int			nowHP;					// 현재 체력
	int			defenceLine;			// 타워의 앞, 상대 유닛이 공격하는 지점
	float		takeDamageStateDelay;	// 데미지를 받아 빨간 색으로 변했다가 원래대로 돌아오는 시간
	float		takeDamageStateTimer;	// takeDamageStateDelay 타이머
	CTowerWizard	towerWizard;		
};

#endif