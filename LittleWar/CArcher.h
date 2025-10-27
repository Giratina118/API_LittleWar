#ifndef __CARCHER_H__
#define __CARCHER_H__

#include "CUnitInfo.h"

class CArcher : public CUnitInfo
{
public:
	CArcher();
	CArcher(SURFACEINFO sfUnit[5], BOOL isPlayer, CTower* g_objTower);
	CArcher(CUnitInfo sfUnit, BOOL isPlayer, CTower* g_objTower);
	~CArcher();
};

#endif