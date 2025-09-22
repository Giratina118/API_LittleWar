#include "ImageManager.h"

// dcScreen와 호환되는 후면표면DC 생성
BOOL __CreateBackBuffer(HDC dcScreen, int nWidth, int nHeight, int nBPP, SURFACEINFO* psInfo)
{
	psInfo->dcSurface = CreateCompatibleDC(dcScreen);
	psInfo->hBmp = CreateCompatibleBitmap(dcScreen, nWidth, nHeight);
	psInfo->hOldBmp = (HBITMAP)SelectObject(psInfo->dcSurface, psInfo->hBmp);
	psInfo->nTop = 0;
	psInfo->nLeft = 0;
	psInfo->nWidth = nWidth;
	psInfo->nHeight = nHeight;
	PatBlt(psInfo->dcSurface, 0, 0, nWidth, nHeight, PATCOPY);

	return TRUE;
}

// 표면 생성 및 로드   각종 오브젝트 또는 비트맵 DC 생성 및 비트맵 로드
void __LoadSurface(HDC dcScreen, SURFACEINFO* psInfo, int nLeft, int nTop)
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
void __ReleaseSurface(SURFACEINFO* psInfo)
{
	SelectObject(psInfo->dcSurface, psInfo->hOldBmp);
	DeleteDC(psInfo->dcSurface);
	DeleteObject(psInfo->hBmp);
}

// 이미지 출력
BOOL __PutImage(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	return BitBlt(dcDst, x, y, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCCOPY);
}

// 이미지 원하는 크기로 출력
BOOL __PutStretch(HDC dcDst, int x, int y, int xRate, int yRate, SURFACEINFO* psInfo)
{
	return StretchBlt(dcDst, x, y, psInfo->nWidth * xRate, psInfo->nHeight * yRate, psInfo->dcSurface, 0, 0, psInfo->nWidth, psInfo->nHeight, SRCCOPY);
}

// 스프라이트
BOOL __PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo)
{
	return TransparentBlt(dcDst,
		x, y, psInfo->nWidth * UNIT_SIZE_RATE, psInfo->nHeight * UNIT_SIZE_RATE,
		psInfo->dcSurface,
		psInfo->nLeft, psInfo->nTop, psInfo->nWidth, psInfo->nHeight,
		psInfo->crColorKey);
}

// 스프라이트
BOOL __PutSprite(HDC dcDst, int x, int y, SURFACEINFO* psInfo, int width, int height)
{
	return TransparentBlt(dcDst,
		x, y, width, height,
		psInfo->dcSurface,
		psInfo->nLeft, psInfo->nTop, psInfo->nWidth, psInfo->nHeight,
		psInfo->crColorKey);
}

// dcScreen에 표면을 복사 ( 후면표면 -> 화면DC )
BOOL __CompleteBlt(HDC dcScreen, SURFACEINFO* psInfo)
{
	BitBlt(dcScreen, 0, 0, psInfo->nWidth, psInfo->nHeight, psInfo->dcSurface, 0, 0, SRCCOPY);
	return TRUE;
}

// DIB -> DDB로 변환
HBITMAP __MakeDDBFromDIB(HDC hdc, char* Path)
{
	HANDLE  hFile;
	DWORD   FileSize, dwRead;
	HBITMAP hBit;
	BITMAPFILEHEADER* fh;
	BITMAPINFO* ih;
	BITMAPINFOHEADER* iheader;
	LPBYTE			 lpMemBlock;

	// 파일을 연다
	hFile = CreateFile(Path, GENERIC_READ, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)	return NULL;

	FileSize = GetFileSize(hFile, NULL);

	lpMemBlock = (LPBYTE)malloc(FileSize);
	fh = (BITMAPFILEHEADER*)lpMemBlock;
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
void __SetImgSurface(SURFACEINFO* psfInfo)
{
	::memset(psfInfo, 0, sizeof(SURFACEINFO));
}

// SURFACEINFO 스프라이트 초기화
void __SetSprSurface(SURFACEINFO* psfInfo, COLORREF crColorKey)
{
	::memset(psfInfo, 0, sizeof(SURFACEINFO));
	psfInfo->crColorKey = crColorKey;
}

// 일괄 표면 생성, 내부의 생성 반복되는 부분 묶어서 함수로 만들기
void __Init(HDC dcScreen, SURFACEINFO& g_sfBack, SURFACEINFO& g_sfBG, CUnitInfo g_infoUnit[UNIT_COUNT], SURFACEINFO g_btnUnit[UNIT_COUNT / 2],
	SURFACEINFO& g_btn, CTower g_objTower[2], SURFACEINFO& g_sfTitle, SURFACEINFO& g_sfUpgradeBtn, SURFACEINFO g_sfGameResult[2],
	SURFACEINFO& g_sfMana, SURFACEINFO& g_sfGold, SURFACEINFO& g_sfTowerIcon, SURFACEINFO g_sfUpgradeArrow[2], SURFACEINFO& g_sfBtnScreen,
	SURFACEINFO g_sfRoundBtn[8], SURFACEINFO g_sfRectBtn[4], SURFACEINFO g_sfArrow[2])
{
	__CreateBackBuffer(dcScreen, MAP_WIDTH, MAP_HEIGHT, 32, &g_sfBack);

	// 배경
	__SetImgSurface(&g_sfBG);
	g_sfBG.hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\map.bmp");
	__LoadSurface(dcScreen, &g_sfBG, 0, 0);

	// 유닛
	for (int unitNum = 0; unitNum < UNIT_COUNT; unitNum++)
	{
		for (int unitSFNum = 0; unitSFNum < UNIT_SF_COUNT; unitSFNum++)
		{
			__SetSprSurface(&g_infoUnit[unitNum].GetpSfUnit()[unitSFNum], RGB(0, 255, 0));
			g_infoUnit[unitNum].GetpSfUnit()[unitSFNum].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\Characters.bmp");
			
			__LoadSurface(dcScreen, &g_infoUnit[unitNum].GetpSfUnit()[unitSFNum], unitNum % 2 * 31 * UNIT_SF_COUNT + 31 * unitSFNum, unitNum / 2 * 21);
			g_infoUnit[unitNum].GetpSfUnit()[unitSFNum].nWidth = 31;
			g_infoUnit[unitNum].GetpSfUnit()[unitSFNum].nHeight = 21;
		}
	}

	// 버튼의 유닛
	for (int i = 0; i < UNIT_COUNT / 2; i++)
	{
		__SetSprSurface(&g_btnUnit[i], RGB(0, 255, 0));
		g_btnUnit[i].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\UI.bmp");

		__LoadSurface(dcScreen, &g_btnUnit[i], 25 * i, 0);
		g_btnUnit[i].nWidth = 25;
		g_btnUnit[i].nHeight = 25;
	}

	// 버튼
	__SetSprSurface(&g_btn, RGB(0, 255, 0));
	g_btn.hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\UI.bmp");

	__LoadSurface(dcScreen, &g_btn, 25, 68);
	g_btn.nWidth = 20;
	g_btn.nHeight = 17;

	// 업그레이드 버튼
	__SetSprSurface(&g_sfUpgradeBtn, RGB(0, 255, 0));
	g_sfUpgradeBtn.hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\UI.bmp");

	__LoadSurface(dcScreen, &g_sfUpgradeBtn, 46, 68);
	g_sfUpgradeBtn.nWidth = 20;
	g_sfUpgradeBtn.nHeight = 17;

	// 타워
	for (int towerNum = 0; towerNum < 2; towerNum++)
	{
		for (int towerSFNum = 0; towerSFNum < 3; towerSFNum++)
		{
			__SetSprSurface(&g_objTower[towerNum].GetpSfUnit()[towerSFNum], RGB(0, 255, 0));
			g_objTower[towerNum].GetpSfUnit()[towerSFNum].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\Tower.bmp");

			__LoadSurface(dcScreen, &g_objTower[towerNum].GetpSfUnit()[towerSFNum], towerSFNum * 41, towerNum * 40);
			g_objTower[towerNum].GetpSfUnit()[towerSFNum].nWidth = 41;
			g_objTower[towerNum].GetpSfUnit()[towerSFNum].nHeight = 40;
		}
	}
	g_objTower[0].SetDefenceLine(TRUE);
	g_objTower[1].SetDefenceLine(FALSE);

	// 마법사
	for (int wizardNum = 0; wizardNum < 2; wizardNum++)
	{
		__SetSprSurface(&g_objTower[0].GetTowerWizard()->GetpSfWizard()[wizardNum], RGB(0, 255, 0));
		g_objTower[0].GetTowerWizard()->GetpSfWizard()[wizardNum].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\Characters.bmp");

		__LoadSurface(dcScreen, &g_objTower[0].GetTowerWizard()->GetpSfWizard()[wizardNum], wizardNum * 31, 21 * 3);
		g_objTower[0].GetTowerWizard()->GetpSfWizard()[wizardNum].nWidth = 31;
		g_objTower[0].GetTowerWizard()->GetpSfWizard()[wizardNum].nHeight = 21;
	}

	// 파이어볼
	for (int fireBallNum = 0; fireBallNum < 42; fireBallNum++)
	{
		__SetSprSurface(&g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum], RGB(0, 255, 0));
		g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\FireBall2.bmp");

		__LoadSurface(dcScreen, &g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum], fireBallNum % 6 * 16, fireBallNum / 6 * 16);
		g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum].nWidth = 16;
		g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum].nHeight = 16;
	}
	for (int fireBallNum = 42; fireBallNum < 47; fireBallNum++)
	{
		__SetSprSurface(&g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum], RGB(0, 255, 0));
		g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\FireBall.bmp");

		__LoadSurface(dcScreen, &g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum], (fireBallNum - 36) * 16, 0);
		g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum].nWidth = 16;
		g_objTower[0].GetTowerWizard()->GetpSfFireBall()[fireBallNum].nHeight = 16;
	}


	// 타이틀
	__SetSprSurface(&g_sfTitle, RGB(0, 255, 0));
	g_sfTitle.hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\Titles.bmp");

	__LoadSurface(dcScreen, &g_sfTitle, 0, 0);
	g_sfTitle.nWidth = 53;
	g_sfTitle.nHeight = 21;

	// 게임 결과
	for (int i = 0; i < 2; i++)
	{
		__SetSprSurface(&g_sfGameResult[i], RGB(0, 255, 0));
		g_sfGameResult[i].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\Titles.bmp");

		__LoadSurface(dcScreen, &g_sfGameResult[i], 0, 34 - 12 * i);
		g_sfGameResult[i].nWidth = 61;
		g_sfGameResult[i].nHeight = 11;
	}

	// 마나 아이콘
	__SetSprSurface(&g_sfMana, RGB(0, 255, 0));
	g_sfMana.hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\UI.bmp");

	__LoadSurface(dcScreen, &g_sfMana, 1, 67);
	g_sfMana.nWidth = 8;
	g_sfMana.nHeight = 9;

	// 골드 아이콘
	__SetSprSurface(&g_sfGold, RGB(0, 255, 0));
	g_sfGold.hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\UI.bmp");

	__LoadSurface(dcScreen, &g_sfGold, 1, 77);
	g_sfGold.nWidth = 9;
	g_sfGold.nHeight = 9;

	// 타워 아이콘
	__SetSprSurface(&g_sfTowerIcon, RGB(0, 255, 0));
	g_sfTowerIcon.hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\UI.bmp");

	__LoadSurface(dcScreen, &g_sfTowerIcon, 1, 48);
	g_sfTowerIcon.nWidth = 22;
	g_sfTowerIcon.nHeight = 17;

	// 화살표
	__SetSprSurface(&g_sfUpgradeArrow[0], RGB(0, 255, 0));
	g_sfUpgradeArrow[0].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\UI.bmp");

	__LoadSurface(dcScreen, &g_sfUpgradeArrow[0], 12, 67);
	g_sfUpgradeArrow[0].nWidth = 9;
	g_sfUpgradeArrow[0].nHeight = 10;

	__SetSprSurface(&g_sfUpgradeArrow[1], RGB(0, 255, 0));
	g_sfUpgradeArrow[1].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\UI.bmp");

	__LoadSurface(dcScreen, &g_sfUpgradeArrow[1], 12, 77);
	g_sfUpgradeArrow[1].nWidth = 9;
	g_sfUpgradeArrow[1].nHeight = 9;

	// 버튼 (스크린 전환)
	__SetSprSurface(&g_sfBtnScreen, RGB(0, 0, 255));
	g_sfBtnScreen.hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\GUI.bmp");

	__LoadSurface(dcScreen, &g_sfBtnScreen, 130, 2);
	g_sfBtnScreen.nWidth = 120;
	g_sfBtnScreen.nHeight = 40;

	// 둥근 버튼
	for (int i = 0; i < 4; i++)
	{
		__SetSprSurface(&g_sfRoundBtn[i], RGB(0, 255, 0));
		g_sfRoundBtn[i].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\Buttom.bmp");

		__LoadSurface(dcScreen, &g_sfRoundBtn[i], 40 + 210 * i, 10);
		g_sfRoundBtn[i].nWidth = 190;
		g_sfRoundBtn[i].nHeight = 190;
	}
	for (int i = 4; i < 8; i++)
	{
		__SetSprSurface(&g_sfRoundBtn[i], RGB(0, 255, 0));
		g_sfRoundBtn[i].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\Buttom.bmp");

		__LoadSurface(dcScreen, &g_sfRoundBtn[i], 40 + 210 * (i - 4), 220);
		g_sfRoundBtn[i].nWidth = 190;
		g_sfRoundBtn[i].nHeight = 190;
	}

	// 사각형 버튼
	for (int i = 0; i < 2; i++)
	{
		__SetSprSurface(&g_sfRectBtn[i], RGB(0, 255, 0));
		g_sfRectBtn[i].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\Buttom.bmp");

		__LoadSurface(dcScreen, &g_sfRectBtn[i], 40, 450 + i * 150);
		g_sfRectBtn[i].nWidth = 385;
		g_sfRectBtn[i].nHeight = 120;
	}
	for (int i = 2; i < 4; i++)
	{
		__SetSprSurface(&g_sfRectBtn[i], RGB(0, 255, 0));
		g_sfRectBtn[i].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\Buttom.bmp");

		__LoadSurface(dcScreen, &g_sfRectBtn[i], 460, 450 + (i - 2) * 150);
		g_sfRectBtn[i].nWidth = 385;
		g_sfRectBtn[i].nHeight = 120;
	}

	// 화살
	__SetSprSurface(&g_sfArrow[0], RGB(0, 255, 0));
	g_sfArrow[0].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\Characters.bmp");
	__LoadSurface(dcScreen, &g_sfArrow[0], 31 * 4 + 3, 21 * 7 + 13);
	g_sfArrow[0].nWidth = 16;
	g_sfArrow[0].nHeight = 7;

	__SetSprSurface(&g_sfArrow[1], RGB(0, 255, 0));
	g_sfArrow[1].hBmp = __MakeDDBFromDIB(dcScreen, "Images\\bmpImg\\Characters.bmp");
	__LoadSurface(dcScreen, &g_sfArrow[1], 31 * 9 + 1, 21 * 7 + 13);
	g_sfArrow[1].nWidth = 16;
	g_sfArrow[1].nHeight = 7;

}

// 일괄 표면 파괴
void __DestroyAll(SURFACEINFO& g_sfBack, SURFACEINFO& g_sfBG, CUnitInfo	g_infoUnit[UNIT_COUNT], CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit)
{
	__DestroyUnit(g_objPlayerUnit, g_objEnemyUnit);

	for (int unitCount = 0; unitCount < UNIT_COUNT; unitCount++)
		for (int unitSFCount = 0; unitSFCount < UNIT_SF_COUNT; unitSFCount++)
			__ReleaseSurface(&g_infoUnit[unitCount].GetpSfUnit()[unitSFCount]);

	__ReleaseSurface(&g_sfBG);
	__ReleaseSurface(&g_sfBack);
}

void __DestroyUnit(CUnitInfo** g_objPlayerUnit, CUnitInfo** g_objEnemyUnit)
{
	for (int unitCount = 0; unitCount < MAX_UNIT_COUNT; unitCount++)
	{
		if (g_objPlayerUnit[unitCount] != NULL)
		{
			for (int unitSFCount = 0; unitSFCount < UNIT_SF_COUNT; unitSFCount++)
				__ReleaseSurface(g_objPlayerUnit[unitCount]->GetpSfUnit() + unitSFCount);
			delete g_objPlayerUnit[unitCount];
			g_objPlayerUnit[unitCount] = NULL;
		}
		if (g_objEnemyUnit[unitCount] != NULL)
		{
			for (int unitSFCount = 0; unitSFCount < UNIT_SF_COUNT; unitSFCount++)
				__ReleaseSurface(g_objEnemyUnit[unitCount]->GetpSfUnit() + unitSFCount);
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