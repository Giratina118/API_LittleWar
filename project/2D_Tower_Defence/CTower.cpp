#include "CTower.h"
#include "ImageManager.h"

CTower::CTower()
{
	towerState = 0;
	nowHP = maxHP = 1000;
	xPos = 50;
	yPos = 180;
	
	takeDamageStateDelay = takeDamageStateTimer = 0.5f;
}

CTower::CTower(BOOL player, int level)
{
	if (player)
	{
		nowHP = maxHP = 1000;
		towerWizard.SetAttackDamage(10);
		xPos = 50;
		hpXPos = xPos + 100;
	}
	else
	{
		xPos = 2310;
		nowHP = maxHP = 1000 + 250 * (level - 1);
		towerWizard.SetAttackDamage(0);
		hpXPos = xPos - 50;
	}
	towerState = 0;
	yPos = 180;
	hpYPos = yPos - 20;
	takeDamageStateDelay = takeDamageStateTimer = 0.5f;
}

CTower::~CTower()
{

}

void CTower::SetDefenceLine(BOOL leftTower)
{
	if (leftTower)
	{
		defenceLine = xPos + g_sfTower[0].nWidth + 10;
	}
	else
	{
		defenceLine = xPos - 10;
	}
}

void CTower::SetLevelOne()
{
	nowHP = maxHP = 1000;
}

void CTower::UpdateTower(Timer timer)
{
	if (towerState == 1)	// 공격받으면 일정시간 타워가 빨간 색으로 나오도록
	{
		takeDamageStateTimer -= timer.GetDeltaTime();

		if (takeDamageStateTimer < 0.0f)
		{
			towerState = 0;
			takeDamageStateTimer = takeDamageStateDelay;
		}
	}
}

void CTower::DrawSurface(HDC dcDst, int nBgX)
{
	__PutSprite(dcDst, xPos + nBgX, yPos, &g_sfTower[towerState]);
}

void CTower::PrintTowerHP(HDC dcDst, int nBgX)
{
	char printTowerHP[255];
	wsprintf(printTowerHP, "%d/%d", nowHP, maxHP);
	TextOut(dcDst, hpXPos + nBgX, hpYPos, printTowerHP, strlen(printTowerHP));
}

void CTower::TakeDamage(int dmg, Timer timer)
{
	nowHP -= dmg;
	if (nowHP <= 0)
	{
		// 게임 끝
		nowHP = 0;
		towerState = 2;
		return;
	}

	towerState = 1;
	takeDamageStateTimer = takeDamageStateDelay;
}

BOOL CTower::TowerHPZero()
{
	if (nowHP > 0)
		return FALSE;
	else
		return TRUE;
}

void CTower::ResetTower()
{
	nowHP = maxHP;
	towerState = 0;
	takeDamageStateTimer = 0.5f;
}