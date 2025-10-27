#include "CUnitInfo.h"
#include "ImageManager.h"

CUnitInfo::CUnitInfo()
{
	m_nowAnimation = 0;
	m_xPos = 100;
	m_yPos = 240;
	m_createTimer = 0.0f;
	m_attackTimer = 0.0f;
	m_isDie = FALSE;
	m_isGetDieGoldMana = FALSE;
	m_attackTower = FALSE;
	m_pTargetUnit = NULL;
	m_halfHPAnimTimer = 0.0f;
	m_dieAnimTimer = 1.0f;
	m_dieGold = 0;
	m_dieMana = 0;
}

CUnitInfo::~CUnitInfo()
{

}

RECT CUnitInfo::GetRect()
{
	RECT rect = { 0, };
	rect.left = m_xPos;
	rect.right = m_xPos + m_sfUnit[0].nWidth * UNIT_SIZE_RATE;
	rect.top = m_yPos;
	rect.bottom = m_yPos + m_sfUnit[0].nHeight * UNIT_SIZE_RATE;
	return rect;
}

void CUnitInfo::SetLevel(int level)
{
	if (level < 2)
	{
		SetLevelOne();
	}
	else
	{
		for (int lv = 1; lv < level; lv++)
			UnitLevelUP();
	}
}

void CUnitInfo::SetLevelOne()
{
	switch (m_enumUnitType)
	{
	case ENUM_UNIT_TYPE_WARRIOR:
		m_maxHP = m_nowHP = 60;
		m_attackDamage = 20;
		m_attackDelay = 2.0f;
		m_attackRange = 40;
		m_moveSpeed = 4.0f;
		m_cost = 3;
		m_createDelay = 2.0f;

		m_dieGold = 5;
		m_dieMana = 1;

		m_levelUpGold = 400;
		m_unitLevel = 1;
		break;

	case ENUM_UNIT_TYPE_ARCHER:
		m_maxHP = m_nowHP = 30;
		m_attackDamage = 10;
		m_attackDelay = 3.0f;
		m_attackRange = 250;
		m_moveSpeed = 3.0f;
		m_cost = 4;
		m_createDelay = 3.0f;

		m_dieGold = 6;
		m_dieMana = 1;

		m_levelUpGold = 450;
		m_unitLevel = 1;
		break;

	case ENUM_UNIT_TYPE_GUARDIAN:
		m_maxHP = m_nowHP = 100;
		m_attackDamage = 15;
		m_attackDelay = 2.5f;
		m_attackRange = 20;
		m_moveSpeed = 3.0f;
		m_cost = 4;
		m_createDelay = 2.0f;

		m_dieGold = 5;
		m_dieMana = 2;

		m_levelUpGold = 450;
		m_unitLevel = 1;
		break;
	}
}


// 유닛 그리기
void CUnitInfo::DrawSurface(HDC dcDst, int nBgX)
{
	if (m_sfUnit[m_nowAnimation].hBmp == NULL)
	{
		OutputDebugStringA("Error: m_sfUnit is NULL\n");
		return;
	}
	PutSprite(dcDst, m_xPos + nBgX, m_yPos, &m_sfUnit[m_nowAnimation]);
}

// 행동 업데이트 (공격, 이동, 반피 이하, 죽음 시 애니메이션 타이머)
void CUnitInfo::UpdateAction(Timer timer, CUnitInfo* target[MAX_UNIT_COUNT])
{
	m_attackTimer -= timer.GetDeltaTime();

	if (m_isDie)	// 죽은 유닛
	{
		m_dieAnimTimer -= timer.GetDeltaTime();	// 타이머 다 지나면 소멸
		return;
	}

	if (m_halfHPAnimTimer > 0.0f)		
	{
		HalfHPAnimation(timer);		// 체력이 반 이하로 떨어졌을 때 애니메이션 
		return;
	}

	// 공격 사정거리에 적이 들어오면 공격, 공격 모션이 끝나기 전까지 이동 불가
	if (ColliderCheck(target) || m_nowAnimation == 1)	
		Attack(timer);	// 공격
	else
		Move(timer);	// 이동
}

// 이동
void CUnitInfo::Move(Timer timer)
{
	if (m_isPlayer)	m_xPos += m_moveSpeed * timer.GetDeltaTime() * 50.0f;	// 아군 이동 방향 (->)
	else			m_xPos -= m_moveSpeed * timer.GetDeltaTime() * 50.0f;	// 적군 이동 방향 (<-)
}

// 공격
void CUnitInfo::Attack(Timer timer)
{
	if (m_attackTimer < 0.0f)		// 공격 쿨타임 다 돌았을 때(공격 가능 상태)
	{
		if (m_attackTimer < -0.5f && m_nowAnimation == 1)	// 공격 후 일정시간 경과 시 애니메이션 idle로 되돌리기
		{
			m_attackTimer = m_attackDelay - 0.5f;
			m_nowAnimation = 0;
		}
		else if (m_attackTower && m_nowAnimation == 0)
		{
			m_targetTower->TakeDamage(m_attackDamage, timer);
			m_attackTimer = 0.0f;
			m_nowAnimation = 1;
		}
		else if (m_pTargetUnit != NULL && !m_pTargetUnit->GetIsDie() && m_nowAnimation == 0)	// 공격
		{
			m_pTargetUnit->TakeDamage(m_attackDamage, timer);	// 적에게 데미지 입히기
			m_attackTimer = 0.0f;
			m_nowAnimation = 1;
		}
	}
}

// 데미지 받기
void CUnitInfo::TakeDamage(int damage, Timer timer)
{
	m_nowHP -= damage;
	if (m_nowHP <= 0)	// 체력 0 이하기 사망
	{
		m_isDie = TRUE;
		m_nowAnimation = 4;
	}
	else if (m_nowHP <= m_maxHP / 2 && m_nowHP + damage > m_maxHP / 2)	// 체력이 반 이하로 떨어지면 애니메이션
	{
		HalfHPAnimation(timer);
		m_nowAnimation = 2;
	}
}

// 체력이 반 이하로 떨어졌을 때 애니메이션
void CUnitInfo::HalfHPAnimation(Timer timer)
{
	m_halfHPAnimTimer += timer.GetDeltaTime();
	if (m_halfHPAnimTimer < 0.5f && m_halfHPAnimTimer > 0.0f)	// 넉백
	{
		if (m_enumUnitType == ENUM_UNIT_TYPE_GUARDIAN)		// 방패병은 넉백 면역
			return;

		if (m_isPlayer)	m_xPos -= timer.GetDeltaTime() * 200.0f;
		else			m_xPos += timer.GetDeltaTime() * 200.0f;
	}
	else
	{
		m_halfHPAnimTimer = 0.0f;
		m_nowAnimation = 0;
	}
}

// 충돌 체크
BOOL CUnitInfo::ColliderCheck(CUnitInfo* target[MAX_UNIT_COUNT])
{
	if (m_pTargetUnit != NULL && !m_pTargetUnit->GetIsDie() && GetTargetLength(m_pTargetUnit->GetXPos()) <= m_attackRange)	
		return TRUE; // 기존 타깃이 생존했고 공격범위가 유효할때 그대로 공격

	m_pTargetUnit = NULL;
	float shortestTargetLength = m_attackRange + 0.1f;

	for (int i = 0; i < MAX_UNIT_COUNT; i++) // 사정거리 안에 있는 상대 유닛 중 가장 가까이 있는 유닛을 타깃으로 설정
	{
		if (target[i] != NULL && !target[i]->GetIsDie() )
		{
			float targetLength = GetTargetLength(target[i]->GetXPos());
			if (targetLength <= m_attackRange && targetLength < shortestTargetLength)
			{
				shortestTargetLength = targetLength;
				m_pTargetUnit = target[i];
			}
		}
	}

	float lengthToTower = GetTargetLength((float)m_targetTower->GetDefenceLine());
	if (lengthToTower < shortestTargetLength && lengthToTower < m_attackRange)
	{
		m_attackTower = TRUE;
		return TRUE;
	}
	else
		m_attackTower = FALSE;

	if (m_pTargetUnit == NULL) return FALSE;
	else					   return TRUE;
}

// 거리 측정
float CUnitInfo::GetTargetLength(float targetX)
{
	float length = m_xPos - targetX;
	if (length < 0.0f)
		length *= -1.0f;
	return length;
}

BOOL CUnitInfo::CanCreateUnit(int mana)
{
	if (mana >= m_cost && m_createTimer <= 0)
		return TRUE;
	else
		return FALSE;
}

void CUnitInfo::UnitLevelUP()
{
	if (m_unitLevel >= UNIT_MAX_LEVEL)
		return;

	m_unitLevel++;
	m_maxHP *= 1.1f;
	m_nowHP = m_maxHP;
	m_attackDamage *= 1.1f;

	m_attackDelay *= 0.95f;
	m_createDelay *= 0.95f;
	m_levelUpGold *= 1.1f;

	if (m_unitLevel % 5 == 0)
	{
		m_maxHP *= 1.1f;
		m_nowHP = m_maxHP;
		m_attackDamage *= 1.1f;
		m_attackDelay *= 0.95f;
		m_createDelay *= 0.95f;

		m_moveSpeed += 1.0f;
		m_attackRange *= 1.2f;
		m_cost += 2;
	}
	else if (m_unitLevel % 5 == 4)
	{
		m_levelUpGold *= 1.1f;
	}
}