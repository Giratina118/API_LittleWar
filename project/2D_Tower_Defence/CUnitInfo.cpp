#include "CUnitInfo.h"
#include "ImageManager.h"

CUnitInfo::CUnitInfo()
{
	nAni = 0;
	xPos = 100;
	yPos = 240;
	createDelayTimer = 0.0f;
	attackDelayTimer = 0.0f;
	m_isDie = FALSE;
	m_isGetDieGoldMana = FALSE;
	attackTower = FALSE;
	pTargetUnit = NULL;
	halfHPAnimTimer = 0.0f;
	dieAnimTimer = 1.0f;
	dieGold = 0;
	dieMana = 0;
}

CUnitInfo::~CUnitInfo()
{

}

RECT CUnitInfo::GetRect()
{
	RECT rect = { 0, };
	rect.left = xPos;
	rect.right = xPos + g_sfUnit[0].nWidth * UNIT_SIZE_RATE;
	rect.top = yPos;
	rect.bottom = yPos + g_sfUnit[0].nHeight * UNIT_SIZE_RATE;
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
	switch (enumUnitType)
	{
	case ENUM_UNIT_TYPE_WARRIOR:
		maxHP = nowHP = 60;
		attackDamage = 20;
		attackDelay = 2.0f;
		attackRange = 40;
		moveSpeed = 4.0f;
		cost = 3;
		createDelay = 2.0f;

		dieGold = 5;
		dieMana = 1;

		levelUpGold = 400;
		unitLevel = 1;
		break;

	case ENUM_UNIT_TYPE_ARCHER:
		maxHP = nowHP = 30;
		attackDamage = 10;
		attackDelay = 3.0f;
		attackRange = 250;
		moveSpeed = 3.0f;
		cost = 4;
		createDelay = 3.0f;

		dieGold = 6;
		dieMana = 1;

		levelUpGold = 450;
		unitLevel = 1;
		break;

	case ENUM_UNIT_TYPE_GUARDIAN:
		maxHP = nowHP = 100;
		attackDamage = 15;
		attackDelay = 2.5f;
		attackRange = 20;
		moveSpeed = 3.0f;
		cost = 4;
		createDelay = 2.0f;

		dieGold = 5;
		dieMana = 2;

		levelUpGold = 450;
		unitLevel = 1;
		break;
	}
}


// 유닛 그리기
void CUnitInfo::DrawSurface(HDC dcDst, int nBgX)
{
	if (g_sfUnit[nAni].hBmp == NULL)
	{
		OutputDebugStringA("Error: g_sfUnit is NULL\n");
		return;
	}
	__PutSprite(dcDst, xPos + nBgX, yPos, &g_sfUnit[nAni]);
}

// 행동 업데이트 (공격, 이동, 반피 이하, 죽음 시 애니메이션 타이머)
void CUnitInfo::UpdateAction(Timer timer, CUnitInfo* target[MAX_UNIT_COUNT])
{
	attackDelayTimer -= timer.GetDeltaTime();

	if (m_isDie)	// 죽은 유닛
	{
		dieAnimTimer -= timer.GetDeltaTime();	// 타이머 다 지나면 소멸
		return;
	}

	if (halfHPAnimTimer > 0.0f)		
	{
		HalfHPAnimation(timer);		// 체력이 반 이하로 떨어졌을 때 애니메이션 
		return;
	}

	if (ColliderCheck(target) || nAni == 1)	// 공격 사정거리에 적이 들어오면 공격, 공격 모션이 끝나기 전까지 이동 불가
	{
		Attack(timer);	// 공격
	}
	else
	{
		Move(timer);	// 이동
	}
}

// 이동
void CUnitInfo::Move(Timer timer)
{
	if (m_isPlayer)	xPos += moveSpeed * timer.GetDeltaTime() * 50.0f;	// 아군 이동 방향 (->)
	else			xPos -= moveSpeed * timer.GetDeltaTime() * 50.0f;	// 적군 이동 방향 (<-)
}

// 공격
void CUnitInfo::Attack(Timer timer)
{
	if (attackDelayTimer < 0.0f)		// 공격 쿨타임 다 돌았을 때(공격 가능 상태)
	{
		if (attackDelayTimer < -0.5f && nAni == 1)	// 공격 후 일정시간 경과 시 애니메이션 idle로 되돌리기
		{
			attackDelayTimer = attackDelay - 0.5f;
			nAni = 0;
		}
		else if (attackTower && nAni == 0)
		{
			targetTower->TakeDamage(attackDamage, timer);
			attackDelayTimer = 0.0f;
			nAni = 1;
		}
		else if (pTargetUnit != NULL && !pTargetUnit->GetIsDie() && nAni == 0)	// 공격
		{
			pTargetUnit->TakeDamage(attackDamage, timer);	// 적에게 데미지 입히기
			attackDelayTimer = 0.0f;
			nAni = 1;

			if (enumUnitType == ENUM_UNIT_TYPE_ARCHER)
			{

			}
		}
	}
}

// 데미지 받기
void CUnitInfo::TakeDamage(int damage, Timer timer)
{
	nowHP -= damage;
	if (nowHP <= 0)	// 체력 0 이하기 사망
	{
		m_isDie = TRUE;
		nAni = 4;
	}
	else if (nowHP <= maxHP / 2 && nowHP + damage > maxHP / 2)	// 체력이 반 이하로 떨어지면 애니메이션
	{
		HalfHPAnimation(timer);
		nAni = 2;
	}
}

// 체력이 반 이하로 떨어졌을 때 애니메이션
void CUnitInfo::HalfHPAnimation(Timer timer)
{
	halfHPAnimTimer += timer.GetDeltaTime();
	if (halfHPAnimTimer < 0.5f && halfHPAnimTimer > 0.0f)	// 넉백
	{
		if (enumUnitType == ENUM_UNIT_TYPE_GUARDIAN)		// 방패병은 넉백 면역
			return;

		if (m_isPlayer)	xPos -= timer.GetDeltaTime() * 200.0f;
		else			xPos += timer.GetDeltaTime() * 200.0f;
	}
	else
	{
		halfHPAnimTimer = 0.0f;
		nAni = 0;
	}
}

// 충돌 체크
BOOL CUnitInfo::ColliderCheck(CUnitInfo* target[MAX_UNIT_COUNT])
{
	if (pTargetUnit != NULL && !pTargetUnit->GetIsDie() && GetTargetLength(pTargetUnit->GetXPos()) <= attackRange)	// 기존 타깃이 생존했고 공격범위가 유효할때 그대로 공격
		return TRUE;

	pTargetUnit = NULL;
	float shortestTargetLength = attackRange + 0.1f;

	// 사정거리 안에 있는 상대 유닛 중 가장 가까이 있는 유닛을 타깃으로 설정
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
	{
		if (target[i] != NULL && !target[i]->GetIsDie() )
		{
			float targetLength = GetTargetLength(target[i]->GetXPos());
			if (targetLength <= attackRange && targetLength < shortestTargetLength)
			{
				shortestTargetLength = targetLength;
				pTargetUnit = target[i];
			}
		}
	}

	float lengthToTower = GetTargetLength((float)targetTower->GetDefenceLine());
	if (lengthToTower < shortestTargetLength && lengthToTower < attackRange)
	{
		attackTower = TRUE;
		return TRUE;
	}
	else
		attackTower = FALSE;

	if (pTargetUnit == NULL)
		return FALSE;
	else
		return TRUE;
}

// 거리 측정
float CUnitInfo::GetTargetLength(float targetX)
{
	float length = xPos - targetX;
	if (length < 0.0f)
		length *= -1.0f;
	return length;
}

BOOL CUnitInfo::CanCreateUnit(int mana)
{
	if (mana >= cost && createDelayTimer <= 0)
		return TRUE;
	else
		return FALSE;
}

void CUnitInfo::UnitLevelUP()
{
	if (unitLevel >= UNIT_MAX_LEVEL)
		return;

	unitLevel++;
	maxHP *= 1.1f;
	nowHP = maxHP;
	attackDamage *= 1.1f;

	attackDelay *= 0.95f;
	createDelay *= 0.95f;
	levelUpGold *= 1.1f;

	if (unitLevel % 5 == 0)
	{
		maxHP *= 1.1f;
		nowHP = maxHP;
		attackDamage *= 1.1f;
		attackDelay *= 0.95f;
		createDelay *= 0.95f;

		moveSpeed += 1.0f;
		attackRange *= 1.2f;
		cost += 2;
	}
	else if (unitLevel % 5 == 4)
	{
		levelUpGold *= 1.1f;
	}
}