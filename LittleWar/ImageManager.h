#ifndef __IMAGEMANAGER_H__
#define __IMAGEMANAGER_H__

#include "DefineData.h"
#include "CUnitInfo.h"
#include "CTower.h"

BOOL CreateBackBuffer(HDC dcScreen, int nWidth, int nHeight, int nBPP, SURFACEINFO* psInfo);	// dcScreen�� ȣȯ�Ǵ� �ĸ�ǥ��DC ����
void LoadSurface(HDC dcScreen, SURFACEINFO* psInfo, int nLeft, int nTop);				// ǥ�� ���� �� �ε�,  ���� ������Ʈ �Ǵ� ��Ʈ�� DC ���� �� ��Ʈ�� �ε�
void ReleaseSurface(SURFACEINFO* psInfo);							// ������ ǥ�� ����

BOOL PutImage(HDC dcDst, int x, int y, SURFACEINFO* psInfo);		// �̹��� ���
BOOL PutStretch(HDC dcDst, int x, int y, int xRate, int yRate, SURFACEINFO* psInfo);
BOOL PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo);		// ��������Ʈ
BOOL PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo, int width, int height);	// ��������Ʈ
BOOL CompleteBlt(HDC dcScreen, SURFACEINFO* psInfo);				// dcScreen�� ǥ���� ���� ( �ĸ�ǥ�� -> ȭ��DC ), Bit Block Transform
HBITMAP MakeDDBFromDIB(HDC hdc, char* Path);						// DIB -> DDB�� ��ȯ, Device Dependent Bitmap, Device Independent Bitmap

void SetImageSurface(SURFACEINFO* psfInfo);							// SURFACEINFO �̹��� �ʱ�ȭ
void SetSpriteSurface(SURFACEINFO* psfInfo, COLORREF crColorKey);	// SURFACEINFO ��������Ʈ �ʱ�ȭ

void Init(HDC dcScreen, SURFACEINFO g_surfaceInfo_Array[Surface_Max], SURFACEINFO& g_sfBack, SURFACEINFO g_btnUnit[UNIT_COUNT / 2], CUnitInfo g_infoUnit[UNIT_COUNT], CTower g_objTower[2]);	// �ϰ� ǥ�� ���� 
void SetSurface(HDC dcScreen, SURFACEINFO* surfaceInfo, char filePath[64], int left, int top, int width, int height);
void DestroyAll(SURFACEINFO& g_sfBack, SURFACEINFO& g_sfBG, CUnitInfo	g_infoUnit[UNIT_COUNT], CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit);			// �ϰ� ǥ�� �ı�
void DestroyUnit(CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit);
void DeleteUnit(CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit);

#endif