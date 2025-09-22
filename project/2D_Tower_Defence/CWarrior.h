#ifndef __CWARRIOR_H__
#define __CWARRIOR_H__

#include "CUnitInfo.h"

class CWarrior : public CUnitInfo
{
public:
	CWarrior();
	CWarrior(SURFACEINFO sfUnit[5], BOOL isPlayer, CTower* g_objTower);
	CWarrior(CUnitInfo sfUnit, BOOL isPlayer, CTower* g_objTower);
	~CWarrior();

};

#endif