#include "CTower.h"
#include "ImageManager.h"

CTower::CTower()
{
	m_towerState = 0;
	m_nowHP = m_maxHP = 1000;
	m_xPos = 50;
	m_yPos = 180;
	
	m_takeDamageStateDelay = m_takeDamageStateTimer = 0.5f;
}

CTower::CTower(BOOL player, int level)
{
	if (player)
	{
		m_nowHP = m_maxHP = 1000;
		m_towerWizard.SetAttackDamage(10);
		m_xPos = 50;
		m_hpXPos = m_xPos + 100;
	}
	else
	{
		m_xPos = 2310;
		m_nowHP = m_maxHP = 1000 + 250 * (level - 1);
		m_towerWizard.SetAttackDamage(0);
		m_hpXPos = m_xPos - 50;
	}
	m_towerState = 0;
	m_yPos = 180;
	m_hpYPos = m_yPos - 20;
	m_takeDamageStateDelay = m_takeDamageStateTimer = 0.5f;
}

CTower::~CTower()
{

}

void CTower::SetDefenceLine(BOOL leftTower)
{
	if (leftTower)
	{
		m_defenceLine = m_xPos + m_sfTower[0].nWidth + 10;
	}
	else
	{
		m_defenceLine = m_xPos - 10;
	}
}

void CTower::SetLevelOne()
{
	m_nowHP = m_maxHP = 1000;
}

void CTower::UpdateTower(Timer timer)
{
	if (m_towerState == 1)	// 공격받으면 일정시간 타워가 빨간 색으로 나오도록
	{
		m_takeDamageStateTimer -= timer.GetDeltaTime();

		if (m_takeDamageStateTimer < 0.0f)
		{
			m_towerState = 0;
			m_takeDamageStateTimer = m_takeDamageStateDelay;
		}
	}
}

void CTower::DrawSurface(HDC dcDst, int nBgX)
{
	PutSprite(dcDst, m_xPos + nBgX, m_yPos, &m_sfTower[m_towerState]);
}

void CTower::PrintTowerHP(HDC dcDst, int nBgX)
{
	char printTowerHP[255];
	wsprintf(printTowerHP, "%d/%d", m_nowHP, m_maxHP);
	TextOut(dcDst, m_hpXPos + nBgX, m_hpYPos, printTowerHP, strlen(printTowerHP));
}

void CTower::TakeDamage(int dmg, Timer timer)
{
	m_nowHP -= dmg;
	if (m_nowHP <= 0)
	{
		// 게임 끝
		m_nowHP = 0;
		m_towerState = 2;
		return;
	}

	m_towerState = 1;
	m_takeDamageStateTimer = m_takeDamageStateDelay;
}

BOOL CTower::TowerHPZero()
{
	if (m_nowHP > 0)
		return FALSE;
	else
		return TRUE;
}

void CTower::ResetTower()
{
	m_nowHP = m_maxHP;
	m_towerState = 0;
	m_takeDamageStateTimer = 0.5f;
}