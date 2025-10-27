#ifndef __CGUARDIAN_H__
#define __CGUARDIAN_H__

#include "CUnitInfo.h"

class CGuardian : public CUnitInfo
{
public:
	CGuardian();
	CGuardian(SURFACEINFO sfUnit[5], BOOL isPlayer, CTower* g_objTower);
	CGuardian(CUnitInfo sfUnit, BOOL isPlayer, CTower* g_objTower);
	~CGuardian();
};

#endif