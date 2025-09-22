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

	SURFACEINFO* GetpSfFireBall() { return g_sfFireBall; }
	int GetFireBallState() { return sfFireBallState; }
	int GetFireBallAngle() { return nFireballAngle; }
	int GetXPos() { return (int)fPx; }
	int GetYPos() { return (int)fPy; }
	RECT GetRect();
	BOOL GetBoom() { return boom; }
	BOOL GetDie() { return die; }
	BOOL GetIsAttack() { return isAttack; }
	void SetIsAttack() { isAttack = TRUE; }

	void DrawSurface(HDC dcDst, int nBgX);
	void Update(Timer timer);

private:
	SURFACEINFO g_sfFireBall[47];		// 파이어볼 이팩트
	int			sfFireBallState;
	int			nFireballAngle;
	float		aniTimer;
	float		shootForce;				// 발사 위력(사거리에 영향)
	float		fPx;
	float		fPy;
	float		fVx;
	float		fVy;
	float		fAx;
	float		fAy;
	BOOL		boom;				// 폭발(지면 접촉)
	BOOL		isAttack;			// 공격했는지 여부
	BOOL		die;
};



class CTowerWizard
{
public:
	CTowerWizard();
	~CTowerWizard();

	void SetAttackDamage(int dmg) { attackDamage = dmg; }
	SURFACEINFO* GetpSfWizard() { return g_sfWizard; }
	SURFACEINFO* GetpSfFireBall() { return g_sfFireBall; }
	CFireBall* GetCFireBall() { return *m_fireBall; }
	int GetAttackDamage() { return attackDamage; }
	int GetAttackRange() { return (int)attackRange; }
	float GetAttackDelay() { return attackDelay; }
	float GetDegree() { return fireBallDegree; }
	void SetDegree(BOOL up, Timer timer);
	void SetLevelOne();

	void DrawSurface(HDC dcDst, int nBgX);
	void UpdateAttack(Timer timer, CUnitInfo* enemyUnit[MAX_UNIT_COUNT]);
	void DestroyFireBall();
	void DamageLevelUp() { attackDamage += 3; }
	void RangeLevelUp() { attackRange += 20; }
	void DelayLevelUp() { attackDelay -= 0.15f; }

private:
	SURFACEINFO g_sfWizard[2];			// 마법사
	SURFACEINFO g_sfFireBall[47];		// 파이어볼
	CFireBall*	m_fireBall[FIREBALL_MAX];
	int			nowfireBallCount;
	BOOL		wizardAttack;			// 공격 여부
	int			attackDamage;			// 타워 공격력
	float		attackRange;			// 공격 범위
	float		attackDelay;			// 공격 딜레이
	float		attackTimer;
	int			xPos;
	int			yPos;
	float		fireBallDegree;			// 발사 각도
};

#endif