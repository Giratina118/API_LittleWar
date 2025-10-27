#include "CArcher.h"

CArcher::CArcher()
{
	m_maxHP = m_nowHP = 30;
	m_attackDamage = 15;
	m_attackDelay = 3.0f;
	m_attackRange = 250;
	m_moveSpeed = 3.0f;
	m_cost = 4;
	m_createDelay = 3.0f;
	m_enumUnitType = ENUM_UNIT_TYPE_ARCHER;

	m_dieGold = 5;
	m_dieMana = 1;

	m_levelUpGold = 450;
	m_unitLevel = 1;
}

CArcher::CArcher(SURFACEINFO sfUnit[5], BOOL isPlayer, CTower* g_objTower)
{
	m_maxHP = m_nowHP = 30;
	m_attackDamage = 10;
	m_attackDelay = 3.0f;
	m_attackRange = 250;
	m_moveSpeed = 3.0f;
	m_cost = 4;
	m_createDelay = 3.0f;
	m_enumUnitType = ENUM_UNIT_TYPE_ARCHER;

	m_dieGold = 6;
	m_dieMana = 1;

	m_levelUpGold = 450;
	m_unitLevel = 1;

	m_targetTower = g_objTower;

	for (int i = 0; i < 5; i++)
		m_sfUnit[i] = sfUnit[i];

	m_isPlayer = isPlayer;
	if (!m_isPlayer)
		m_xPos = 2300;
		
}

CArcher::CArcher(CUnitInfo sfUnit, BOOL isPlayer, CTower* g_objTower)
{
	m_maxHP = m_nowHP = sfUnit.GetMaxHP();
	m_attackDamage = sfUnit.GetAttackDamage();
	m_attackDelay = sfUnit.GetAttackDelay();
	m_attackRange = sfUnit.GetAttackRange();
	m_moveSpeed = sfUnit.GetMoveSpeed();
	m_cost = sfUnit.GetCost();
	m_createDelay = sfUnit.GetCreateDelay();
	m_enumUnitType = ENUM_UNIT_TYPE_ARCHER;

	m_dieGold = 5;
	m_dieMana = 1;

	m_levelUpGold = 400;
	m_unitLevel = 0;

	m_targetTower = g_objTower;

	for (int i = 0; i < 5; i++)
		m_sfUnit[i] = sfUnit.GetpSfUnit()[i];

	m_isPlayer = isPlayer;
	if (!m_isPlayer)
		m_xPos = 2300;
}

CArcher::~CArcher()
{

}