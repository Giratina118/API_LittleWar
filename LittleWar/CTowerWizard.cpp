#include "CTowerWizard.h"
#include "ImageManager.h"
#include "CUnitInfo.h"
#include <math.h>
#include <stdio.h>

CTowerWizard::CTowerWizard()
{
	m_nowfireBallCount = 0;
	m_wizardAttack = FALSE;
	m_attackDamage = 10;
	m_attackRange = 250;
	m_attackDelay = m_attackTimer = 1.5f;
	m_xPos = 70;
	m_yPos = 155;
	for (int i = 0; i < FIREBALL_MAX; i++)
		m_fireBall[i] = NULL;
	m_fireBallDegree = 45;
}

CTowerWizard::~CTowerWizard()
{

}

void CTowerWizard::SetDegree(BOOL up, Timer timer)
{
	int dir = 1;
	if (!up)
		dir = -1;
	m_fireBallDegree += dir * 100 * timer.GetDeltaTime();
}

void CTowerWizard::SetLevelOne()
{
	m_attackDamage = 10;
	m_attackRange = 250;
	m_attackDelay = m_attackTimer = 1.5f;
}

void CTowerWizard::DrawSurface(HDC dcDst, int nBgX)
{
	int lineX = 100 * cos(m_fireBallDegree * (PI / 180.0f));
	int lineY = -100 * sin(m_fireBallDegree * (PI / 180.0f));
	MoveToEx(dcDst, m_xPos + nBgX + 30, m_yPos + 30, NULL);
	LineTo(dcDst, m_xPos + lineX + nBgX + 30, m_yPos + lineY + 30);
	PutSprite(dcDst, m_xPos + nBgX, m_yPos, &m_sfWizard[m_wizardAttack]);

	for (int i = 0; i < FIREBALL_MAX; i++)
		if (m_fireBall[i] != NULL)
			m_fireBall[i]->DrawSurface(dcDst, nBgX);

}

void CTowerWizard::UpdateAttack(Timer timer, CUnitInfo* enemyUnit[MAX_UNIT_COUNT])
{
	// 일정 시간마다 파이어볼 발사
	m_attackTimer -= timer.GetDeltaTime();
	if (m_attackTimer < -0.2f && m_wizardAttack)
	{
		m_attackTimer = m_attackDelay - 0.2f;
		m_wizardAttack = FALSE;
	}
	else if (!m_wizardAttack && m_attackTimer < 0.0f && m_fireBall[m_nowfireBallCount] == NULL)
	{
		m_fireBall[m_nowfireBallCount] = new CFireBall(m_sfFireBall, m_xPos + 10, m_yPos, m_fireBallDegree, m_attackRange);

		m_attackTimer = 0.0f;
		m_wizardAttack = TRUE;

		m_nowfireBallCount++;
		if (m_nowfireBallCount >= FIREBALL_MAX)
			m_nowfireBallCount = 0;
	}

	// 파이어볼 업데이트
	for (int i = 0; i < FIREBALL_MAX; i++)
	{
		if (m_fireBall[i] == NULL)
			continue;

		if (m_fireBall[i]->GetDie())
		{
			delete m_fireBall[i];
			m_fireBall[i] = NULL;
			continue;
		}
		else if (m_fireBall[i]->GetBoom() && !m_fireBall[i]->GetIsAttack())
		{
			m_fireBall[i]->SetIsAttack();
			for (int enemy = 0; enemy < MAX_UNIT_COUNT; enemy++)
			{
				RECT interRect;
				if (enemyUnit[enemy] != NULL && !enemyUnit[enemy]->GetIsDie() && IntersectRect(&interRect, &m_fireBall[i]->GetRect(), &enemyUnit[enemy]->GetRect()))
					enemyUnit[enemy]->TakeDamage(m_attackDamage, timer);
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
	m_boom = m_isAttack = FALSE;
	m_sfFireBallState = 0;
	m_aniTimer = 0.0f;
	m_fPx = xPos;
	m_fPy = yPos;
	m_fAx = -5.0f;
	m_fAy = 100.0f;
	m_die = FALSE;

	m_shootForce = force;
	m_fVx = m_shootForce * cos(degree * (PI / 180.0f));
	m_fVy = -1 * m_shootForce * sin(degree * (PI / 180.0f));

	float angle = atan(-m_fVy / m_fVx) * 180.0f / PI;
	if (angle > 55)
		m_nFireballAngle = 0;
	else if (angle > 30)
		m_nFireballAngle = 1;
	else if (angle > 10)
		m_nFireballAngle = 2;
	else if (angle > -10)
		m_nFireballAngle = 3;
	else if (angle > -30)
		m_nFireballAngle = 4;
	else if (angle > -55)
		m_nFireballAngle = 5;
	else if (angle < -55)
		m_nFireballAngle = 6;

	m_nFireballAngle = 0;

	for (int i = 0; i < 47; i++)
		this->m_sfFireBall[i] = g_sfFireBall[i];
}

CFireBall::~CFireBall()
{

}

RECT CFireBall::GetRect()
{
	RECT rect = { 0, };
	rect.left = m_fPx + m_sfFireBall[0].nWidth * UNIT_SIZE_RATE * 0.5f;
	rect.right = m_fPx + m_sfFireBall[0].nWidth * UNIT_SIZE_RATE * 1.5f;
	rect.top = m_fPy + m_sfFireBall[0].nHeight * UNIT_SIZE_RATE * 0.5f;
	rect.bottom = m_fPy + m_sfFireBall[0].nHeight * UNIT_SIZE_RATE * 1.5f;
	return rect;
}

void CFireBall::DrawSurface(HDC dcDst, int nBgX)
{
	PutSprite(dcDst, m_fPx + nBgX, m_fPy, &m_sfFireBall[m_sfFireBallState + 6 * m_nFireballAngle]);
}

void CFireBall::Update(Timer timer)
{
	m_aniTimer += timer.GetDeltaTime();
	if (m_aniTimer > 0.05f)
	{
		m_aniTimer = 0.0f;
		// 파이어볼의 폭발 애니메이션이 끝났다면 제거
		if (m_nFireballAngle > 6 && m_sfFireBallState > 4)
			m_die = TRUE; 
		else
		{ // 파이어볼 애니메이션 업데이트
			if (m_sfFireBallState < 6)  m_sfFireBallState++;
			if (m_sfFireBallState == 6) m_sfFireBallState = 0;
		}
	}

	if (!m_boom) 
	{	// 파이어볼이 땅에 닿아 터지기 전이면 이동 방향에 따라 애니메이션 각도 변경
		m_fVy += m_fAy * timer.GetDeltaTime();
		m_fPy += m_fVy * timer.GetDeltaTime();

		if (m_fVy > 0)
			m_fVx += m_fAx * timer.GetDeltaTime();
		m_fPx += m_fVx * timer.GetDeltaTime();

		float angle = atan(-m_fVy / m_fVx) * 180.0f / PI;
		if (angle > 55.0f)		  if (m_nFireballAngle != 0)  m_nFireballAngle = 0;
		else if (angle > 30.0f)   if (m_nFireballAngle != 1)  m_nFireballAngle = 1;
		else if (angle > 10.0f)   if (m_nFireballAngle != 2)  m_nFireballAngle = 2;
		else if (angle > -10.0f)  if (m_nFireballAngle != 3)  m_nFireballAngle = 3;
		else if (angle > -30.0f)  if (m_nFireballAngle != 4)  m_nFireballAngle = 4;
		else if (angle > -55.0f)  if (m_nFireballAngle != 5)  m_nFireballAngle = 5;
		else if (angle < -55.0f)  if (m_nFireballAngle != 6)  m_nFireballAngle = 6;
	}

	if (!m_boom && m_fPy > 250)
	{	// 파이어볼이 땅에 닿아았다면 폭발 애니메이션으로 변경
		m_sfFireBallState = 0;
		m_nFireballAngle = 7;
		m_boom = TRUE;
	}
}