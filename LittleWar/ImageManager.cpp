#include "ImageManager.h"

// dcScreen와 호환되는 후면표면DC 생성
BOOL CreateBackBuffer(HDC dcScreen, int nWidth, int nHeight, int nBPP, SURFACEINFO* psInfo)
{
	psInfo->dcSurface = CreateCompatibleDC(dcScreen); // 후면 표면 DC를 만든다.
	psInfo->hBmp = CreateCompatibleBitmap(dcScreen, nWidth, nHeight); // 후면 표면에 비트맵을 만들고
	psInfo->hOldBmp = (HBITMAP)SelectObject(psInfo->dcSurface, psInfo->hBmp); // 그 비트맵을 메모리DC에 연결한다.
	psInfo->nTop = 0;
	psInfo->nLeft = 0;
	psInfo->nWidth = nWidth;
	psInfo->nHeight = nHeight;
	PatBlt(psInfo->dcSurface, 0, 0, nWidth, nHeight, PATCOPY); // 배경 채우기

	return TRUE;
}

// 표면 생성 및 로드   각종 오브젝트 또는 비트맵 DC 생성 및 비트맵 로드
void LoadSurface(HDC dcScreen, SURFACEINFO* psInfo, int nLeft, int nTop)
{
	BITMAP  bit;

	psInfo->dcSurface = CreateCompatibleDC(dcScreen);
	psInfo->hOldBmp = (HBITMAP)SelectObject(psInfo->dcSurface, psInfo->hBmp);

	GetObject(psInfo->hBmp, sizeof(BITMAP), &bit);
	psInfo->nWidth = bit.bmWidth;
	psInfo->nHeight = bit.bmHeight;
	psInfo->nLeft = nLeft;
	psInfo->nTop = nTop;
}

// 생성된 표면 해제
void ReleaseSurface(SURFACEINFO* psInfo)
{
	SelectObject(psInfo->dcSurface, psInfo->hOldBmp);
	DeleteDC(psInfo->dcSurface);
	DeleteObject(psInfo->hBmp);
}

// 이미지 출력
BOOL PutImage(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	return BitBlt(dcDst, x, y, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCCOPY);
}

// 이미지 원하는 크기로 출력
BOOL PutStretch(HDC dcDst, int x, int y, int xRate, int yRate, SURFACEINFO* psInfo)
{
	return StretchBlt(dcDst, x, y, psInfo->nWidth * xRate, psInfo->nHeight * yRate, psInfo->dcSurface, 0, 0, psInfo->nWidth, psInfo->nHeight, SRCCOPY);
}

// 스프라이트
BOOL PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	return TransparentBlt(dcDst,
		x, y, psInfo->nWidth * UNIT_SIZE_RATE, psInfo->nHeight * UNIT_SIZE_RATE,
		psInfo->dcSurface,
		psInfo->nLeft, psInfo->nTop, psInfo->nWidth, psInfo->nHeight,
		psInfo->crColorKey);
}

// 스프라이트
BOOL PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo, int width, int height)
{
	return TransparentBlt(dcDst,
		x, y, width, height,
		psInfo->dcSurface,
		psInfo->nLeft, psInfo->nTop, psInfo->nWidth, psInfo->nHeight,
		psInfo->crColorKey);
}

// dcScreen에 표면을 복사 ( 후면표면 -> 화면DC )
BOOL CompleteBlt(HDC dcScreen, SURFACEINFO* psInfo)
{
	BitBlt(dcScreen, 0, 0, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCCOPY);
	return TRUE;
}

// DIB -> DDB로 변환
HBITMAP MakeDDBFromDIB(HDC hdc, char* Path)
{
	HANDLE  hFile;
	DWORD FileSize, dwRead;
	HBITMAP hBit;
	BITMAPFILEHEADER* fh;
	BITMAPINFO* ih;
	BITMAPINFOHEADER* iheader;
	LPBYTE lpMemBlock;

	// 비트맵 파일을 연다
	hFile = CreateFile(Path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)	return NULL;

	// 파일 크기만큼 메모리를 할당
	FileSize = GetFileSize(hFile, NULL);
	lpMemBlock = (LPBYTE)malloc(FileSize);
	fh = (BITMAPFILEHEADER*)lpMemBlock;

	// 파일 내용 메모리로 읽기
	ReadFile(hFile, lpMemBlock, FileSize, &dwRead, NULL);
	CloseHandle(hFile);

	ih = (BITMAPINFO*)(lpMemBlock + sizeof(BITMAPFILEHEADER));
	iheader = (BITMAPINFOHEADER*)(lpMemBlock + sizeof(BITMAPFILEHEADER));

	// DDB로 변환한다.
	hBit = CreateDIBitmap(hdc, iheader, CBM_INIT, lpMemBlock + fh->bfOffBits, ih, DIB_RGB_COLORS);

	free(lpMemBlock);

	return hBit;
}

// SURFACEINFO 이미지 초기화
void SetImageSurface(SURFACEINFO* psfInfo)
{
	::memset(psfInfo, 0, sizeof(SURFACEINFO));
}

// SURFACEINFO 스프라이트 초기화
void SetSpriteSurface(SURFACEINFO* psfInfo, COLORREF crColorKey)
{
	::memset(psfInfo, 0, sizeof(SURFACEINFO));
	psfInfo->crColorKey = crColorKey;
}

// 일괄 표면 생성
void Init(HDC dcScreen, SURFACEINFO g_surfaceInfo_Array[Surface_Max], SURFACEINFO& g_sfBack, SURFACEINFO g_btnUnit[UNIT_COUNT / 2], CUnitInfo g_infoUnit[UNIT_COUNT], CTower g_objTower[2])
{
	CreateBackBuffer(dcScreen, MAP_WIDTH, MAP_HEIGHT, 32, &g_sfBack);

	// 배경
	SetImageSurface(&g_surfaceInfo_Array[Surface_Background]);
	g_surfaceInfo_Array[Surface_Background].hBmp = MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\map.bmp");
	LoadSurface(dcScreen, &g_surfaceInfo_Array[Surface_Background], 0, 0);

	// 타이틀
	SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Title], "Images\\bmpImg\\Titles.bmp", 0, 0, 53, 21);

	// 게임 결과
	for (int i = 0; i < 2; i++)
		SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Result_Lose - i], "Images\\bmpImg\\Titles.bmp", 0, 34 - 12 * i, 61, 11);

	// 유닛
	for (int unitNum = 0; unitNum < UNIT_COUNT; unitNum++)
		for (int unitSFNum = 0; unitSFNum < UNIT_SF_COUNT; unitSFNum++)
			SetSurface(dcScreen, &g_infoUnit[unitNum].GetpSfUnit()[unitSFNum], "Images\\bmpImg\\Characters.bmp", unitNum % 2 * 31 * UNIT_SF_COUNT + 31 * unitSFNum, unitNum / 2 * 21, 31, 21);

	// 타워
	for (int towerNum = 0; towerNum < 2; towerNum++)
		for (int towerSFNum = 0; towerSFNum < 3; towerSFNum++)
			SetSurface(dcScreen, &g_objTower[towerNum].GetpSfUnit()[towerSFNum], "Images\\bmpImg\\Tower.bmp", towerSFNum * 41, towerNum * 40, 41, 40);
	g_objTower[0].SetDefenceLine(TRUE);
	g_objTower[1].SetDefenceLine(FALSE);

	// 마법사
	for (int wizardNum = 0; wizardNum < 2; wizardNum++)
		SetSurface(dcScreen, &g_objTower[0].GetTowerWizard()->GetpSfWizard()[wizardNum], "Images\\bmpImg\\Characters.bmp", wizardNum * 31, 21 * 3, 31, 21);

	// 파이어볼
	for (int fireBallNum = 0; fireBallNum < 42; fireBallNum++)
		SetSurface(dcScreen, &g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum], "Images\\bmpImg\\FireBall2.bmp", fireBallNum % 6 * 16, fireBallNum / 6 * 16, 16, 16);
	for (int fireBallNum = 42; fireBallNum < 47; fireBallNum++)
		SetSurface(dcScreen, &g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum], "Images\\bmpImg\\FireBall.bmp", (fireBallNum - 36) * 16, 0, 16, 16);


	// 버튼
	// 버튼의 유닛
	for (int i = 0; i < UNIT_COUNT / 2; i++)
		SetSurface(dcScreen, &g_btnUnit[i], "Images\\bmpImg\\UI.bmp", 25 * i, 0, 25, 25);

	// 유닛 생성 버튼
	SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Button_UnitCreate], "Images\\bmpImg\\UI.bmp", 25, 68, 20, 17);

	// 업그레이드 버튼
	SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Button_Upgrade], "Images\\bmpImg\\UI.bmp", 46, 68, 20, 17);
	
	// 버튼 (스크린 전환)
	SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Button_Screen], "Images\\bmpImg\\GUI.bmp", 130, 2, 120, 40);

	// 둥근 버튼
	for (int i = 0; i < 4; i++)
		SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Button_Round_Play + i], "Images\\bmpImg\\Buttom.bmp", 40 + 210 * i, 10, 190, 190);
	for (int i = 4; i < 8; i++)
		SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Button_Round_Play + i], "Images\\bmpImg\\Buttom.bmp", 40 + 210 * (i - 4), 220, 190, 190);

	// 사각형 버튼
	for (int i = 0; i < 2; i++)
		SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Button_Rectangle_Blue + i], "Images\\bmpImg\\Buttom.bmp", 40, 450 + i * 150, 385, 120);
	for (int i = 2; i < 4; i++)
		SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Button_Rectangle_Blue + i], "Images\\bmpImg\\Buttom.bmp", 460, 450 + (i - 2) * 150, 385, 120);


	// 아이콘, 화살표
	// 마나 아이콘
	SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Icon_Mana], "Images\\bmpImg\\UI.bmp", 1, 67, 8, 9);

	// 골드 아이콘
	SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Icon_Gold], "Images\\bmpImg\\UI.bmp", 1, 77, 9, 9);

	// 타워 아이콘
	SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Icon_Tower], "Images\\bmpImg\\UI.bmp", 1, 48, 22, 17);

	// 화살표
	SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Arrow_Right], "Images\\bmpImg\\UI.bmp", 12, 67, 9, 10);
	SetSurface(dcScreen, &g_surfaceInfo_Array[Surface_Arrow_Up], "Images\\bmpImg\\UI.bmp", 12, 77, 9, 9);
}

// 표면 생성
void SetSurface(HDC dcScreen, SURFACEINFO* surfaceInfo, char filePath[64], int left, int top, int width, int height)
{
	SetSpriteSurface(surfaceInfo, RGB(0, 255, 0));
	(*surfaceInfo).hBmp = MakeDDBFromDIB(dcScreen, filePath);

	LoadSurface(dcScreen, surfaceInfo, left, top);
	(*surfaceInfo).nWidth = width;
	(*surfaceInfo).nHeight = height;
}


// 일괄 표면 파괴
void DestroyAll(SURFACEINFO& g_sfBack, SURFACEINFO& g_sfBG, CUnitInfo	g_infoUnit[UNIT_COUNT], CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit)
{
	DestroyUnit(g_objPlayerUnit, g_objEnemyUnit);

	for (int unitCount = 0; unitCount < UNIT_COUNT; unitCount++)
		for (int unitSFCount = 0; unitSFCount < UNIT_SF_COUNT; unitSFCount++)
			ReleaseSurface(&g_infoUnit[unitCount].GetpSfUnit()[unitSFCount]);

	ReleaseSurface(&g_sfBG);
	ReleaseSurface(&g_sfBack);
}

void DestroyUnit(CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit)
{
	for (int unitCount = 0; unitCount < MAX_UNIT_COUNT; unitCount++)
	{
		if (g_objPlayerUnit[unitCount] != NULL)
		{
			for (int unitSFCount = 0; unitSFCount < UNIT_SF_COUNT; unitSFCount++)
				ReleaseSurface(g_objPlayerUnit[unitCount]->GetpSfUnit() + unitSFCount);
			delete g_objPlayerUnit[unitCount];
			g_objPlayerUnit[unitCount] = NULL;
		}
		if (g_objEnemyUnit[unitCount] != NULL)
		{
			for (int unitSFCount = 0; unitSFCount < UNIT_SF_COUNT; unitSFCount++)
				ReleaseSurface(g_objEnemyUnit[unitCount]->GetpSfUnit() + unitSFCount);
			delete g_objEnemyUnit[unitCount];
			g_objEnemyUnit[unitCount] = NULL;
		}
	}
}

void DeleteUnit(CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit)
{
	for (int unitCount = 0; unitCount < MAX_UNIT_COUNT; unitCount++)
	{
		if (g_objPlayerUnit[unitCount] != NULL)
		{
			delete g_objPlayerUnit[unitCount];
			g_objPlayerUnit[unitCount] = NULL;
		}
		if (g_objEnemyUnit[unitCount] != NULL)
		{
			delete g_objEnemyUnit[unitCount];
			g_objEnemyUnit[unitCount] = NULL;
		}
	}
}