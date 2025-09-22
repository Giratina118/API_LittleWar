#ifndef __IMAGEMANAGER_H__
#define __IMAGEMANAGER_H__

#include "DefineData.h"
#include "CUnitInfo.h"
#include "CTower.h"

BOOL __CreateBackBuffer(HDC dcScreen, int nWidth, int nHeight, int nBPP, SURFACEINFO* psInfo);	// dcScreen와 호환되는 후면표면DC 생성
void __LoadSurface(HDC dcScreen, SURFACEINFO* psInfo, int nLeft, int nTop);				// 표면 생성 및 로드,  각종 오브젝트 또는 비트맵 DC 생성 및 비트맵 로드
void __ReleaseSurface(SURFACEINFO* psInfo);							// 생성된 표면 해제

BOOL __PutImage(HDC dcDst, int x, int y, SURFACEINFO* psInfo);		// 이미지 출력
BOOL __PutStretch(HDC dcDst, int x, int y, int xRate, int yRate, SURFACEINFO* psInfo);
BOOL __PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo);		// 스프라이트
BOOL __PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo, int width, int height);	// 스프라이트
BOOL __CompleteBlt(HDC dcScreen, SURFACEINFO* psInfo);				// dcScreen에 표면을 복사 ( 후면표면 -> 화면DC )
HBITMAP __MakeDDBFromDIB(HDC hdc, char* Path);						// DIB -> DDB로 변환

void __SetImgSurface(SURFACEINFO* psfInfo);							// SURFACEINFO 이미지 초기화
void __SetSprSurface(SURFACEINFO* psfInfo, COLORREF crColorKey);	// SURFACEINFO 스프라이트 초기화

void __Init(HDC dcScreen, SURFACEINFO& g_sfBack, SURFACEINFO& g_sfBG, CUnitInfo g_infoUnit[UNIT_COUNT], SURFACEINFO g_btnUnit[UNIT_COUNT / 2], 
	SURFACEINFO& g_btn, CTower g_objTower[2], SURFACEINFO& g_sfTitle, SURFACEINFO& g_sfUpgradeBtn, SURFACEINFO g_sfGameResult[2], 
	SURFACEINFO& g_sfMana, SURFACEINFO& g_sfGold, SURFACEINFO& g_sfTowerIcon, SURFACEINFO g_sfUpgradeArrow[2], SURFACEINFO& g_sfBtnScreen,
	SURFACEINFO g_sfRoundBtn[8], SURFACEINFO g_sfRectBtn[4], SURFACEINFO g_sfArrow[2]);	// 일괄 표면 생성 
void __DestroyAll(SURFACEINFO& g_sfBack, SURFACEINFO& g_sfBG, CUnitInfo	g_infoUnit[UNIT_COUNT], CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit);			// 일괄 표면 파괴
void __DestroyUnit(CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit);
void DeleteUnit(CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit);

#endif