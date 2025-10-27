#ifndef __CUNITINFO_H__
#define __CUNITINFO_H__

//#include <Windows.h>
#include "Timer.h"
#include "DefineData.h"
#include "CTower.h"

enum UnitType
{
	ENUM_UNIT_TYPE_NONE = -1,
	ENUM_UNIT_TYPE_WARRIOR = 0,
	ENUM_UNIT_TYPE_ARCHER,
	ENUM_UNIT_TYPE_GUARDIAN
};

class CUnitInfo
{
public:
	CUnitInfo();
	~CUnitInfo();

	SURFACEINFO* GetpSfUnit() { return m_sfUnit; }		// SURFACEINFO 호출
	int GetXPos() { return (int)m_xPos; }					// x좌표
	int GetYPos() { return m_yPos; }						// y좌표
	int GetMaxHP() { return m_maxHP; }
	int GetAttackDamage() { return m_attackDamage; }
	float GetAttackDelay() { return m_attackDelay; }
	int GetAttackRange() { return m_attackRange; }
	int GetMoveSpeed() { return m_moveSpeed; }
	int GetCost() { return m_cost; }
	float GetCreateDelay() { return m_createDelay; }
	float GetCreateDelayTimer() { return m_createTimer; }
	BOOL GetIsDie() { return m_isDie; }					// 죽은 유닛인지 여부
	float GetDieAnimTimer() { return m_dieAnimTimer; }	// 사망 애니메이션 타이머
	int GetDieMana() { return m_dieMana; }
	int GetDieGold() { return m_dieGold; }
	int GetLevel() { return m_unitLevel; }
	int GetLevelUpGold() { return m_levelUpGold; }
	BOOL GetisGetDieGoldMana() { return m_isGetDieGoldMana; }
	RECT GetRect();

	void SetisGetDieGoldMana() { m_isGetDieGoldMana = TRUE; }
	void SetnAni(int n) { m_nowAnimation = n; }	// 현재 애니메이션 설정
	void SetLevel(int level);
	void SetLevelOne();
	
	void DrawSurface(HDC dcDst, int nBgX);								// 유닛 그리기
	void UpdateAction(Timer timer, CUnitInfo* target[MAX_UNIT_COUNT]);	// 행동 업데이트 (공격, 이동, 반피 이하, 죽음 시 애니메이션 타이머)
	void Move(Timer timer);						// 이동
	void Attack(Timer timer);					// 공격
	void TakeDamage(int damage, Timer timer);	// 데미지 받기
	void HalfHPAnimation(Timer timer);			// 체력이 반 이하로 떨어졌을 때 애니메이션

	BOOL ColliderCheck(CUnitInfo* enemy[MAX_UNIT_COUNT]);	// 충돌 체크 (공격 사정거리 안에 적이 들어왔는지)
	float GetTargetLength(float targetX);					// 공격 타깃과의 거리 측정

	BOOL CanCreateUnit(int mana);							// 유닛 생산 딜레이가 다 흘렀는지 (생산할 수 있는지)
	void ResetCreateTimer() { m_createTimer = m_createDelay; } // 유닛 생산 타이머 리셋
	void CreateTimerUpdate(Timer timer) { if (m_createTimer > 0)	m_createTimer -= timer.GetDeltaTime(); }	// 유닛 생산 타이머 업데이트

	void UnitLevelUP();

protected:
	SURFACEINFO	m_sfUnit[5];	// 0: idle,  1: attack,  2~3: take damage(hp 일정 구간 하락 시),  4: die
	int			m_nowAnimation;	// 현재 애니메이션
	float		m_xPos;			// 유닛의 x좌표
	int			m_yPos;			// 유닛의 y좌표

	int			m_maxHP;			// 최대 체력
	int			m_nowHP;			// 현재 체력
	int			m_attackDamage;		// 공격력
	float		m_attackDelay;		// 공격 딜레이
	float		m_attackTimer;	// 공격 딜레이 타이머
	int			m_attackRange;		// 공격 범위
	float		m_moveSpeed;		// 이동 속도

	int			m_cost;				// 생산 비용
	float		m_createDelay;		// 생산 딜레이
	float		m_createTimer;	// 생산 딜레이 타이머

	BOOL		m_isPlayer;			// true: 아군 유닛,  false: 적 유닛
	BOOL		m_isDie;			// true: 생존,  false: 사망
	BOOL		m_isGetDieGoldMana;	// true: 사망 골드, 마나 획득 완료, false: 미완료

	CUnitInfo*	m_pTargetUnit;		// 공격 타깃 유닛

	float		m_halfHPAnimTimer;	// 체력 반피 애니메이션 타이머
	float		m_dieAnimTimer;		// 사망 애니메이션 타이머
	int			m_enumUnitType;		// 유닛 유형

	BOOL		m_attackTower;		// 타워 공격 여부
	CTower*		m_targetTower;		// 공격할 적 타워

	int			m_dieGold;			// 쓰러뜨릴 시 획득할 수 있는 돈
	int			m_dieMana;			// 쓰러뜨릴 시 획득할 수 있는 마나

	int			m_levelUpGold;		// 레벨업 시 필요한 골드
	int			m_unitLevel;
};

#endif