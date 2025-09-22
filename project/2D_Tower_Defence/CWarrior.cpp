#include "CWarrior.h"

CWarrior::CWarrior()
{
	maxHP = nowHP = 60;
	attackDamage = 20;
	attackDelay = 2.0f;
	attackRange = 40;
	moveSpeed = 4.0f;
	cost = 3;
	createDelay = 2.0f;
	enumUnitType = ENUM_UNIT_TYPE_WARRIOR;

	dieGold = 5;
	dieMana = 1;

	levelUpGold = 400;
	unitLevel = 1;
}

CWarrior::CWarrior(SURFACEINFO sfUnit[5], BOOL isPlayer, CTower* g_objTower)
{
	maxHP = nowHP = 60;
	attackDamage = 20;
	attackDelay = 2.0f;
	attackRange = 40;
	moveSpeed = 4.0f;
	cost = 3;
	createDelay = 2.0f;
	enumUnitType = ENUM_UNIT_TYPE_WARRIOR;

	dieGold = 5;
	dieMana = 1;

	levelUpGold = 400;
	unitLevel = 1;

	targetTower = g_objTower;

	for (int i = 0; i < 5; i++)
		g_sfUnit[i] = sfUnit[i];

	m_isPlayer = isPlayer;
	if (!m_isPlayer)
		xPos = 2300;
}

CWarrior::CWarrior(CUnitInfo sfUnit, BOOL isPlayer, CTower* g_objTower)
{
	maxHP = nowHP = sfUnit.GetMaxHP();
	attackDamage = sfUnit.GetAttackDamage();
	attackDelay = sfUnit.GetAttackDelay();
	attackRange = sfUnit.GetAttackRange();
	moveSpeed = sfUnit.GetMoveSpeed();
	cost = sfUnit.GetCost();
	createDelay = sfUnit.GetCreateDelay();
	enumUnitType = ENUM_UNIT_TYPE_WARRIOR;

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

CWarrior::~CWarrior()
{

}