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
	void DrawSurface(HDC dcDst, int nBgX);				// Ÿ�� �׸���
	void PrintTowerHP(HDC dcDst, int nBgX);				// Ÿ�� ü�� ǥ��
	void TakeDamage(int dmg, Timer timer);

	BOOL TowerHPZero();
	void ResetTower();
	void TowerMaxHPLevelUp() { m_maxHP += 500;	m_nowHP = m_maxHP; }

private:
	SURFACEINFO	m_sfTower[3];			// 0: �⺻,  1: ���ݹ���,  2: �ı�
	int			m_towerState;			// 0: �⺻,  1: ���ݹ���,  2: �ı�
	int			m_xPos;					// Ÿ���� x��ǥ
	int			m_yPos;					// Ÿ���� y��ǥ
	int			m_hpXPos;				// Ÿ�� hp ǥ�� x��ǥ
	int			m_hpYPos;				// Ÿ�� hp ǥ�� y��ǥ
	int			m_maxHP;				// �ִ� ü��
	int			m_nowHP;				// ���� ü��
	int			m_defenceLine;			// Ÿ���� ��, ��� ������ �����ϴ� ����
	float		m_takeDamageStateDelay;	// �������� �޾� ���� ������ ���ߴٰ� ������� ���ƿ��� �ð�
	float		m_takeDamageStateTimer;	// takeDamageStateDelay Ÿ�̸�
	CTowerWizard	m_towerWizard;		
};

#endif