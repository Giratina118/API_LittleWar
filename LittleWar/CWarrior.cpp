#include "CWarrior.h"

CWarrior::CWarrior()
{
	m_maxHP = m_nowHP = 60;
	m_attackDamage = 20;
	m_attackDelay = 2.0f;
	m_attackRange = 40;
	m_moveSpeed = 4.0f;
	m_cost = 3;
	m_createDelay = 2.0f;
	m_enumUnitType = ENUM_UNIT_TYPE_WARRIOR;

	m_dieGold = 5;
	m_dieMana = 1;

	m_levelUpGold = 400;
	m_unitLevel = 1;
}

CWarrior::CWarrior(SURFACEINFO sfUnit[5], BOOL isPlayer, CTower* g_objTower)
{
	m_maxHP = m_nowHP = 60;
	m_attackDamage = 20;
	m_attackDelay = 2.0f;
	m_attackRange = 40;
	m_moveSpeed = 4.0f;
	m_cost = 3;
	m_createDelay = 2.0f;
	m_enumUnitType = ENUM_UNIT_TYPE_WARRIOR;

	m_dieGold = 5;
	m_dieMana = 1;

	m_levelUpGold = 400;
	m_unitLevel = 1;

	m_targetTower = g_objTower;

	for (int i = 0; i < 5; i++)
		m_sfUnit[i] = sfUnit[i];

	m_isPlayer = isPlayer;
	if (!m_isPlayer)
		m_xPos = 2300;
}

CWarrior::CWarrior(CUnitInfo sfUnit, BOOL isPlayer, CTower* g_objTower)
{
	m_maxHP = m_nowHP = sfUnit.GetMaxHP();
	m_attackDamage = sfUnit.GetAttackDamage();
	m_attackDelay = sfUnit.GetAttackDelay();
	m_attackRange = sfUnit.GetAttackRange();
	m_moveSpeed = sfUnit.GetMoveSpeed();
	m_cost = sfUnit.GetCost();
	m_createDelay = sfUnit.GetCreateDelay();
	m_enumUnitType = ENUM_UNIT_TYPE_WARRIOR;

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

CWarrior::~CWarrior()
{

}