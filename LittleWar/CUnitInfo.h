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

	SURFACEINFO* GetpSfUnit() { return m_sfUnit; }		// SURFACEINFO ȣ��
	int GetXPos() { return (int)m_xPos; }					// x��ǥ
	int GetYPos() { return m_yPos; }						// y��ǥ
	int GetMaxHP() { return m_maxHP; }
	int GetAttackDamage() { return m_attackDamage; }
	float GetAttackDelay() { return m_attackDelay; }
	int GetAttackRange() { return m_attackRange; }
	int GetMoveSpeed() { return m_moveSpeed; }
	int GetCost() { return m_cost; }
	float GetCreateDelay() { return m_createDelay; }
	float GetCreateDelayTimer() { return m_createTimer; }
	BOOL GetIsDie() { return m_isDie; }					// ���� �������� ����
	float GetDieAnimTimer() { return m_dieAnimTimer; }	// ��� �ִϸ��̼� Ÿ�̸�
	int GetDieMana() { return m_dieMana; }
	int GetDieGold() { return m_dieGold; }
	int GetLevel() { return m_unitLevel; }
	int GetLevelUpGold() { return m_levelUpGold; }
	BOOL GetisGetDieGoldMana() { return m_isGetDieGoldMana; }
	RECT GetRect();

	void SetisGetDieGoldMana() { m_isGetDieGoldMana = TRUE; }
	void SetnAni(int n) { m_nowAnimation = n; }	// ���� �ִϸ��̼� ����
	void SetLevel(int level);
	void SetLevelOne();
	
	void DrawSurface(HDC dcDst, int nBgX);								// ���� �׸���
	void UpdateAction(Timer timer, CUnitInfo* target[MAX_UNIT_COUNT]);	// �ൿ ������Ʈ (����, �̵�, ���� ����, ���� �� �ִϸ��̼� Ÿ�̸�)
	void Move(Timer timer);						// �̵�
	void Attack(Timer timer);					// ����
	void TakeDamage(int damage, Timer timer);	// ������ �ޱ�
	void HalfHPAnimation(Timer timer);			// ü���� �� ���Ϸ� �������� �� �ִϸ��̼�

	BOOL ColliderCheck(CUnitInfo* enemy[MAX_UNIT_COUNT]);	// �浹 üũ (���� �����Ÿ� �ȿ� ���� ���Դ���)
	float GetTargetLength(float targetX);					// ���� Ÿ����� �Ÿ� ����

	BOOL CanCreateUnit(int mana);							// ���� ���� �����̰� �� �귶���� (������ �� �ִ���)
	void ResetCreateTimer() { m_createTimer = m_createDelay; } // ���� ���� Ÿ�̸� ����
	void CreateTimerUpdate(Timer timer) { if (m_createTimer > 0)	m_createTimer -= timer.GetDeltaTime(); }	// ���� ���� Ÿ�̸� ������Ʈ

	void UnitLevelUP();

protected:
	SURFACEINFO	m_sfUnit[5];	// 0: idle,  1: attack,  2~3: take damage(hp ���� ���� �϶� ��),  4: die
	int			m_nowAnimation;	// ���� �ִϸ��̼�
	float		m_xPos;			// ������ x��ǥ
	int			m_yPos;			// ������ y��ǥ

	int			m_maxHP;			// �ִ� ü��
	int			m_nowHP;			// ���� ü��
	int			m_attackDamage;		// ���ݷ�
	float		m_attackDelay;		// ���� ������
	float		m_attackTimer;	// ���� ������ Ÿ�̸�
	int			m_attackRange;		// ���� ����
	float		m_moveSpeed;		// �̵� �ӵ�

	int			m_cost;				// ���� ���
	float		m_createDelay;		// ���� ������
	float		m_createTimer;	// ���� ������ Ÿ�̸�

	BOOL		m_isPlayer;			// true: �Ʊ� ����,  false: �� ����
	BOOL		m_isDie;			// true: ����,  false: ���
	BOOL		m_isGetDieGoldMana;	// true: ��� ���, ���� ȹ�� �Ϸ�, false: �̿Ϸ�

	CUnitInfo*	m_pTargetUnit;		// ���� Ÿ�� ����

	float		m_halfHPAnimTimer;	// ü�� ���� �ִϸ��̼� Ÿ�̸�
	float		m_dieAnimTimer;		// ��� �ִϸ��̼� Ÿ�̸�
	int			m_enumUnitType;		// ���� ����

	BOOL		m_attackTower;		// Ÿ�� ���� ����
	CTower*		m_targetTower;		// ������ �� Ÿ��

	int			m_dieGold;			// �����߸� �� ȹ���� �� �ִ� ��
	int			m_dieMana;			// �����߸� �� ȹ���� �� �ִ� ����

	int			m_levelUpGold;		// ������ �� �ʿ��� ���
	int			m_unitLevel;
};

#endif