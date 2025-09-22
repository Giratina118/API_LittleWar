#include "CTowerWizard.h"
#include "ImageManager.h"
#include "CUnitInfo.h"
#include <math.h>
#include <stdio.h>

CTowerWizard::CTowerWizard()
{
	nowfireBallCount = 0;
	wizardAttack = FALSE;
	attackDamage = 10;
	attackRange = 250;
	attackDelay = attackTimer = 1.5f;
	xPos = 70;
	yPos = 155;
	for (int i = 0; i < FIREBALL_MAX; i++)
		m_fireBall[i] = NULL;
	fireBallDegree = 45;
}

CTowerWizard::~CTowerWizard()
{

}

void CTowerWizard::SetDegree(BOOL up, Timer timer)
{
	int dir = 1;
	if (!up)
		dir = -1;
	fireBallDegree += dir * 100 * timer.GetDeltaTime();
}

void CTowerWizard::SetLevelOne()
{
	attackDamage = 10;
	attackRange = 250;
	attackDelay = attackTimer = 1.5f;
}

void CTowerWizard::DrawSurface(HDC dcDst, int nBgX)
{
	int lineX = 100 * cos(fireBallDegree * (PI / 180.0f));
	int lineY = -100 * sin(fireBallDegree * (PI / 180.0f));
	MoveToEx(dcDst, xPos + nBgX + 30, yPos + 30, NULL);
	LineTo(dcDst, xPos + lineX + nBgX + 30, yPos + lineY + 30);
	__PutSprite(dcDst, xPos + nBgX, yPos, &g_sfWizard[wizardAttack]);

	for (int i = 0; i < FIREBALL_MAX; i++)
		if (m_fireBall[i] != NULL)
			m_fireBall[i]->DrawSurface(dcDst, nBgX);

}

void CTowerWizard::UpdateAttack(Timer timer, CUnitInfo* enemyUnit[MAX_UNIT_COUNT])
{
	// 일정 시간마다 파이어볼 발사
	attackTimer -= timer.GetDeltaTime();
	if (attackTimer < -0.2f && wizardAttack)
	{
		attackTimer = attackDelay - 0.2f;
		wizardAttack = FALSE;
	}
	else if (!wizardAttack && attackTimer < 0.0f && m_fireBall[nowfireBallCount] == NULL)
	{
		m_fireBall[nowfireBallCount] = new CFireBall(g_sfFireBall, xPos + 10, yPos, fireBallDegree, attackRange);

		attackTimer = 0.0f;
		wizardAttack = TRUE;

		nowfireBallCount++;
		if (nowfireBallCount >= FIREBALL_MAX)
			nowfireBallCount = 0;
	}

	// 파이어볼 업데이트
	for (int i = 0; i < FIREBALL_MAX; i++)
	{
		if (m_fireBall[i] == NULL)
			continue;

		if (m_fireBall[i]->GetDie())
		{
			//m_fireBall[i]->GetFireBallState() == 6 && m_fireBall[i]->GetFireBallAngle() > 5
			delete m_fireBall[i];
			m_fireBall[i] = NULL;
			continue;
		}
		else if (m_fireBall[i]->GetBoom() && !m_fireBall[i]->GetIsAttack())
		{
			m_fireBall[i]->SetIsAttack();
			for (int enemy = 0; enemy < MAX_UNIT_COUNT; enemy++)
			{
				//IntersectRect
				RECT interRect;
				if (enemyUnit[enemy] != NULL && !enemyUnit[enemy]->GetIsDie() && IntersectRect(&interRect, &m_fireBall[i]->GetRect(), &enemyUnit[enemy]->GetRect()))
					enemyUnit[enemy]->TakeDamage(attackDamage, timer);

				//if (enemyUnit[enemy] != NULL && !enemyUnit[enemy]->GetIsDie() && enemyUnit[enemy]->GetTargetLength(m_fireBall[i]->GetXPos() - 30) < 100.0f)
				//	enemyUnit[enemy]->TakeDamage(attackDamage, timer);
			}

		}
		m_fireBall[i]->Update(timer);
	}
		
}

void CTowerWizard::DestroyFireBall()
{
	for (int i = 0; i < FIREBALL_MAX; i++)
	{
		if (m_fireBall[i] != NULL)
		{
			delete m_fireBall[i];
			m_fireBall[i] = NULL;
		}
	}
}



CFireBall::CFireBall(SURFACEINFO g_sfFireBall[11], int xPos, int yPos, float degree, float force)
{
	boom = isAttack = FALSE;
	sfFireBallState = 0;
	aniTimer = 0.0f;
	fPx = xPos;
	fPy = yPos;
	fAx = -5.0f;
	fAy = 100.0f;
	die = FALSE;

	shootForce = force;
	fVx = shootForce * cos(degree * (PI / 180.0f));
	fVy = -1 * shootForce * sin(degree * (PI / 180.0f));

	float angle = atan(-fVy / fVx) * 180.0f / PI;
	if (angle > 55)
		nFireballAngle = 0;
	else if (angle > 30)
		nFireballAngle = 1;
	else if (angle > 10)
		nFireballAngle = 2;
	else if (angle > -10)
		nFireballAngle = 3;
	else if (angle > -30)
		nFireballAngle = 4;
	else if (angle > -55)
		nFireballAngle = 5;
	else if (angle < -55)
		nFireballAngle = 6;

	nFireballAngle = 0;

	for (int i = 0; i < 47; i++)
		this->g_sfFireBall[i] = g_sfFireBall[i];
}

CFireBall::~CFireBall()
{

}

RECT CFireBall::GetRect()
{
	RECT rect = { 0, };
	rect.left = fPx + g_sfFireBall[0].nWidth * UNIT_SIZE_RATE * 0.5f;
	rect.right = fPx + g_sfFireBall[0].nWidth * UNIT_SIZE_RATE * 1.5f;
	rect.top = fPy + g_sfFireBall[0].nHeight * UNIT_SIZE_RATE * 0.5f;
	rect.bottom = fPy + g_sfFireBall[0].nHeight * UNIT_SIZE_RATE * 1.5f;
	return rect;
}

void CFireBall::DrawSurface(HDC dcDst, int nBgX)
{
	__PutSprite(dcDst, fPx + nBgX, fPy, &g_sfFireBall[sfFireBallState + 6 * nFireballAngle]);

}

void CFireBall::Update(Timer timer)
{
	aniTimer += timer.GetDeltaTime();
	if (aniTimer > 0.05f)
	{
		aniTimer = 0.0f;

		if (nFireballAngle > 6 && sfFireBallState > 4)
			die = TRUE;
		else
		{
			if (sfFireBallState < 6)
				sfFireBallState++;

			if (sfFireBallState == 6)
				sfFireBallState = 0;
		}
	}

	if (!boom)
	{
		fVy += fAy * timer.GetDeltaTime();
		fPy += fVy * timer.GetDeltaTime();

		if (fVy > 0)
			fVx += fAx * timer.GetDeltaTime();
		fPx += fVx * timer.GetDeltaTime();

		
		float angle = atan(-fVy / fVx) * 180.0f / PI;
		if (angle > 55.0f)
		{
			if (nFireballAngle != 0)
				nFireballAngle = 0;
		}
		else if (angle > 30.0f)
		{
			if (nFireballAngle != 1)
				nFireballAngle = 1;
		}
		else if (angle > 10.0f)
		{
			if (nFireballAngle != 2)
				nFireballAngle = 2;
		}
		else if (angle > -10.0f)
		{

			if (nFireballAngle != 3)
				nFireballAngle = 3;
		}
		else if (angle > -30.0f)
		{
			if (nFireballAngle != 4)
				nFireballAngle = 4;
		}
		else if (angle > -55.0f)
		{
			if (nFireballAngle != 5)
				nFireballAngle = 5;
		}
		else if (angle < -55.0f)
		{
			if (nFireballAngle != 6)
				nFireballAngle = 6;
		}
			


	}

	if (!boom && fPy > 250)
	{
		sfFireBallState = 0;
		nFireballAngle = 7;
		boom = TRUE;
	}
}