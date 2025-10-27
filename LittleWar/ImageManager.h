#ifndef __IMAGEMANAGER_H__
#define __IMAGEMANAGER_H__

#include "DefineData.h"
#include "CUnitInfo.h"
#include "CTower.h"

BOOL CreateBackBuffer(HDC dcScreen, int nWidth, int nHeight, int nBPP, SURFACEINFO* psInfo);	// dcScreen와 호환되는 후면표면DC 생성
void LoadSurface(HDC dcScreen, SURFACEINFO* psInfo, int nLeft, int nTop);				// 표면 생성 및 로드,  각종 오브젝트 또는 비트맵 DC 생성 및 비트맵 로드
void ReleaseSurface(SURFACEINFO* psInfo);							// 생성된 표면 해제

BOOL PutImage(HDC dcDst, int x, int y, SURFACEINFO* psInfo);		// 이미지 출력
BOOL PutStretch(HDC dcDst, int x, int y, int xRate, int yRate, SURFACEINFO* psInfo);
BOOL PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo);		// 스프라이트
BOOL PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo, int width, int height);	// 스프라이트
BOOL CompleteBlt(HDC dcScreen, SURFACEINFO* psInfo);				// dcScreen에 표면을 복사 ( 후면표면 -> 화면DC ), Bit Block Transform
HBITMAP MakeDDBFromDIB(HDC hdc, char* Path);						// DIB -> DDB로 변환, Device Dependent Bitmap, Device Independent Bitmap

void SetImageSurface(SURFACEINFO* psfInfo);							// SURFACEINFO 이미지 초기화
void SetSpriteSurface(SURFACEINFO* psfInfo, COLORREF crColorKey);	// SURFACEINFO 스프라이트 초기화

void Init(HDC dcScreen, SURFACEINFO g_surfaceInfo_Array[Surface_Max], SURFACEINFO& g_sfBack, SURFACEINFO g_btnUnit[UNIT_COUNT / 2], CUnitInfo g_infoUnit[UNIT_COUNT], CTower g_objTower[2]);	// 일괄 표면 생성 
void SetSurface(HDC dcScreen, SURFACEINFO* surfaceInfo, char filePath[64], int left, int top, int width, int height);
void DestroyAll(SURFACEINFO& g_sfBack, SURFACEINFO& g_sfBG, CUnitInfo	g_infoUnit[UNIT_COUNT], CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit);			// 일괄 표면 파괴
void DestroyUnit(CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit);
void DeleteUnit(CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit);

#endif