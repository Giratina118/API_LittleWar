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

	SURFACEINFO* GetpSfUnit() { return m_sfTower; }
	CTowerWizard* GetTowerWizard() { return &m_towerWizard; }
	int GetMaxHP() { return m_maxHP; }
	int GetDefenceLine() { return m_defenceLine; }
	void SetMaxHP(int hp) { m_maxHP = hp;		m_nowHP = m_maxHP; }
	void SetTowerState(int state) { m_towerState = state; }
	void SetxPos(int x) { m_xPos = x; }
	void SetDefenceLine(BOOL leftTower);
	void SetLevelOne();

	void UpdateTower(Timer timer);
	void DrawSurface(HDC dcDst, int nBgX);				// 타워 그리기
	void PrintTowerHP(HDC dcDst, int nBgX);				// 타워 체력 표시
	void TakeDamage(int dmg, Timer timer);

	BOOL TowerHPZero();
	void ResetTower();
	void TowerMaxHPLevelUp() { m_maxHP += 500;	m_nowHP = m_maxHP; }

private:
	SURFACEINFO	m_sfTower[3];			// 0: 기본,  1: 공격받음,  2: 파괴
	int			m_towerState;			// 0: 기본,  1: 공격받음,  2: 파괴
	int			m_xPos;					// 타워의 x좌표
	int			m_yPos;					// 타워의 y좌표
	int			m_hpXPos;				// 타워 hp 표시 x좌표
	int			m_hpYPos;				// 타워 hp 표시 y좌표
	int			m_maxHP;				// 최대 체력
	int			m_nowHP;				// 현재 체력
	int			m_defenceLine;			// 타워의 앞, 상대 유닛이 공격하는 지점
	float		m_takeDamageStateDelay;	// 데미지를 받아 빨간 색으로 변했다가 원래대로 돌아오는 시간
	float		m_takeDamageStateTimer;	// takeDamageStateDelay 타이머
	CTowerWizard	m_towerWizard;		
};

#endif