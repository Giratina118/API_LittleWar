#include "CGuardian.h"

CGuardian::CGuardian()
{
	maxHP = nowHP = 110;
	attackDamage = 10;
	attackDelay = 2.5f;
	attackRange = 20;
	moveSpeed = 3.0f;
	cost = 4;
	createDelay = 2.0f;
	enumUnitType = ENUM_UNIT_TYPE_GUARDIAN;

	dieGold = 5;
	dieMana = 1;

	levelUpGold = 450;
	unitLevel = 1;
}

CGuardian::CGuardian(SURFACEINFO sfUnit[5], BOOL isPlayer, CTower* g_objTower)
{
	maxHP = nowHP = 100;
	attackDamage = 15;
	attackDelay = 2.5f;
	attackRange = 20;
	moveSpeed = 3.0f;
	cost = 4;
	createDelay = 2.0f;
	enumUnitType = ENUM_UNIT_TYPE_GUARDIAN;

	dieGold = 5;
	dieMana = 2;

	levelUpGold = 450;
	unitLevel = 1;
	
	targetTower = g_objTower;

	for (int i = 0; i < 5; i++)
		g_sfUnit[i] = sfUnit[i];

	m_isPlayer = isPlayer;
	if (!m_isPlayer)
		xPos = 2300;
}

CGuardian::CGuardian(CUnitInfo sfUnit, BOOL isPlayer, CTower* g_objTower)
{
	maxHP = nowHP = sfUnit.GetMaxHP();
	attackDamage = sfUnit.GetAttackDamage();
	attackDelay = sfUnit.GetAttackDelay();
	attackRange = sfUnit.GetAttackRange();
	moveSpeed = sfUnit.GetMoveSpeed();
	cost = sfUnit.GetCost();
	createDelay = sfUnit.GetCreateDelay();
	enumUnitType = ENUM_UNIT_TYPE_GUARDIAN;

	dieGold = 5;
	dieMana = 1;

	levelUpGold = 400;
	unitLevel = 0;

	targetTower = g_objTower;

	for (int i = 0; i < 5; i++)
		g_sfUnit[i] = sfUnit.GetpSfUnit()[i];

	m_isPlayer = isPlayer;
	if (!m_isPlayer)
		xPos = 2300;
}

CGuardian::~CGuardian()
{

}