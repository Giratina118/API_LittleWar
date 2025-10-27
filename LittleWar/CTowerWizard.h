#ifndef __CTOWERWIZARD_H__
#define __CTOWERWIZARD_H__

#include "DefineData.h"
#include "Timer.h"
//#include "CUnitInfo.h"

class CUnitInfo;

class CFireBall
{
public:
	CFireBall();
	CFireBall(SURFACEINFO g_sfFireBall[47], int xPos, int yPos, float degree, float force);
	~CFireBall();

	SURFACEINFO* GetpSfFireBall() { return m_sfFireBall; }
	int GetFireBallState() { return m_sfFireBallState; }
	int GetFireBallAngle() { return m_nFireballAngle; }
	int GetXPos() { return (int)m_fPx; }
	int GetYPos() { return (int)m_fPy; }
	RECT GetRect();
	BOOL GetBoom() { return m_boom; }
	BOOL GetDie() { return m_die; }
	BOOL GetIsAttack() { return m_isAttack; }
	void SetIsAttack() { m_isAttack = TRUE; }

	void DrawSurface(HDC dcDst, int nBgX);
	void Update(Timer timer);

private:
	SURFACEINFO m_sfFireBall[47];	// 파이어볼 이팩트
	int			m_sfFireBallState;
	int			m_nFireballAngle;
	float		m_aniTimer;
	float		m_shootForce;		// 발사 위력(사거리에 영향)
	float		m_fPx;
	float		m_fPy;
	float		m_fVx;
	float		m_fVy;
	float		m_fAx;
	float		m_fAy;
	BOOL		m_boom;				// 폭발(지면 접촉)
	BOOL		m_isAttack;			// 공격했는지 여부
	BOOL		m_die;
};



class CTowerWizard
{
public:
	CTowerWizard();
	~CTowerWizard();

	void SetAttackDamage(int dmg) { m_attackDamage = dmg; }
	SURFACEINFO* GetpSfWizard() { return m_sfWizard; }
	SURFACEINFO* GetpSfFireBall() { return m_sfFireBall; }
	CFireBall* GetCFireBall() { return *m_fireBall; }
	int GetAttackDamage() { return m_attackDamage; }
	int GetAttackRange() { return (int)m_attackRange; }
	float GetAttackDelay() { return m_attackDelay; }
	float GetDegree() { return m_fireBallDegree; }
	void SetDegree(BOOL up, Timer timer);
	void SetLevelOne();

	void DrawSurface(HDC dcDst, int nBgX);
	void UpdateAttack(Timer timer, CUnitInfo* enemyUnit[MAX_UNIT_COUNT]);
	void DestroyFireBall();
	void DamageLevelUp() { m_attackDamage += 3; }
	void RangeLevelUp() { m_attackRange += 20; }
	void DelayLevelUp() { m_attackDelay -= 0.15f; }

private:
	SURFACEINFO m_sfWizard[2];			// 마법사
	SURFACEINFO m_sfFireBall[47];		// 파이어볼
	CFireBall*	m_fireBall[FIREBALL_MAX];
	int			m_nowfireBallCount;
	BOOL		m_wizardAttack;			// 공격 여부
	int			m_attackDamage;			// 타워 공격력
	float		m_attackRange;			// 공격 범위
	float		m_attackDelay;			// 공격 딜레이
	float		m_attackTimer;
	int			m_xPos;
	int			m_yPos;
	float		m_fireBallDegree;		// 발사 각도
};

#endif