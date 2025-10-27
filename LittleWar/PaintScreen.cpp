#include <stdio.h>
#include "PaintScreen.h"
#include "ImageManager.h"
#include "UpdateGameProc.h"

extern SURFACEINFO g_sfBack;

void PaintTitleScreen(BOOL* buttonClick, SURFACEINFO g_surfaceInfo_Array[Surface_Max], int nBgX, HFONT* newFont, HFONT* oldFont)
{
	PutStretch(g_sfBack.dcSurface, nBgX, 0, MAP_SIZE_RATE, MAP_SIZE_RATE, &g_surfaceInfo_Array[Surface_Background]);	// 배경
	PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 250, 20, &g_surfaceInfo_Array[Surface_Title], 500, 200);	// 타이틀 이미지

	if (buttonClick[0])	PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 150, 250, &g_surfaceInfo_Array[Surface_Button_Rectangle_BluePush], 300, 70);	// 이어하기 버튼
	else				PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 150, 250, &g_surfaceInfo_Array[Surface_Button_Rectangle_Blue], 300, 70);

	if (buttonClick[1]) PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 150, 325, &g_surfaceInfo_Array[Surface_Button_Rectangle_BluePush], 300, 70);	// 새로시작 버튼
	else				PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 150, 325, &g_surfaceInfo_Array[Surface_Button_Rectangle_Blue], 300, 70);

	if (buttonClick[2]) PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 150, 400, &g_surfaceInfo_Array[Surface_Button_Rectangle_RedPush], 300, 70);	// 종료 버튼
	else				PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 150, 400, &g_surfaceInfo_Array[Surface_Button_Rectangle_Red], 300, 70);

	// 문자 출력
	SetBkMode(g_sfBack.dcSurface, TRANSPARENT);
	SetTextColor(g_sfBack.dcSurface, RGB(0, 0, 0));

	*newFont = CreateFont(40, 0, 0, 0, FW_HEAVY, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "돋음체");
	*oldFont = (HFONT)SelectObject(g_sfBack.dcSurface, *newFont);

	TextOut(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 90, 265, "Load Game", strlen("Load Game"));
	TextOut(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 78, 340, "New Game", strlen("New Game"));
	TextOut(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 25, 415, "Exit", strlen("Exit"));

	SelectObject(g_sfBack.dcSurface, *oldFont);
	DeleteObject(*newFont);
}

void PainrReadyScreen(BOOL* buttonClick, SURFACEINFO g_surfaceInfo_Array[Surface_Max], int nBgX, HFONT* newFont, HFONT* oldFont, int stageLevel, int gold, CUnitInfo* g_infoUnit,
	int* upgradeLevel, int* upgradeGold, CTower* g_objTower, int maxMana, float manaDelay)
{
	PutStretch(g_sfBack.dcSurface, nBgX, 0, MAP_SIZE_RATE, MAP_SIZE_RATE, &g_surfaceInfo_Array[Surface_Background]);				 // 배경

	if (buttonClick[3]) PutSprite(g_sfBack.dcSurface, 100, 230, &g_surfaceInfo_Array[Surface_Button_Rectangle_BluePush], 200, 60); // 시작
	else				PutSprite(g_sfBack.dcSurface, 100, 230, &g_surfaceInfo_Array[Surface_Button_Rectangle_Blue], 200, 60);

	if (buttonClick[4]) PutSprite(g_sfBack.dcSurface, 100, 330, &g_surfaceInfo_Array[Surface_Button_Rectangle_RedPush], 200, 60);	 // 타이틀로 돌아가기
	else				PutSprite(g_sfBack.dcSurface, 100, 330, &g_surfaceInfo_Array[Surface_Button_Rectangle_Red], 200, 60);

	TextOut(g_sfBack.dcSurface, 126, 248, "Stage Start", strlen("Stage Start"));
	TextOut(g_sfBack.dcSurface, 177, 348, "Back", strlen("Back"));

	// 현재 스테이지 출력
	SetBkMode(g_sfBack.dcSurface, TRANSPARENT);
	SetTextColor(g_sfBack.dcSurface, RGB(0, 0, 0));

	*newFont = CreateFont(25, 0, 0, 0, FW_HEAVY, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "바탕체");
	*oldFont = (HFONT)SelectObject(g_sfBack.dcSurface, *newFont);
	char printStage[32];
	wsprintf(printStage, "Stage %d", stageLevel);
	TextOut(g_sfBack.dcSurface, 150, 150, printStage, strlen(printStage));

	// 상단 중앙 업그레이드 글자
	TextOut(g_sfBack.dcSurface, 470, 10, "Upgrade", strlen("Upgrade"));

	// 골드
	SetTextColor(g_sfBack.dcSurface, RGB(200, 200, 50));
	char printGold[32];
	wsprintf(printGold, "Gold: %d", gold);
	TextOut(g_sfBack.dcSurface, 1000, 10, printGold, strlen(printGold));

	// 업그레이드 내용 텍스트
	*newFont = CreateFont(18, 0, 0, 0, FW_HEAVY, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "돋음체");
	*oldFont = (HFONT)SelectObject(g_sfBack.dcSurface, *newFont);
	SetTextColor(g_sfBack.dcSurface, RGB(0, 0, 0));

	// 유닛 업그레이드
	UpgradeUnitInfoPrint(g_infoUnit[0], 400, 40, &g_sfBack.dcSurface, &g_surfaceInfo_Array[Surface_Button_Upgrade]);	// 전사
	UpgradeUnitInfoPrint(g_infoUnit[2], 675, 40, &g_sfBack.dcSurface, &g_surfaceInfo_Array[Surface_Button_Upgrade]);	// 궁수
	UpgradeUnitInfoPrint(g_infoUnit[4], 950, 40, &g_sfBack.dcSurface, &g_surfaceInfo_Array[Surface_Button_Upgrade]);	// 방패병


	// 업그레이드: 마법사(공격력, 사거리, 공격주기), 마나(최대 마나, 마나 회복 속도), 타워 체력
	char upgradeInfoText[32];

	// 마법사 공격력
	UpgradeAbilityInfoPrint(400, 200, "Attack Damage", 0, &g_sfBack.dcSurface, &g_surfaceInfo_Array[Surface_Button_Upgrade], upgradeLevel, upgradeGold, 
		&g_objTower[0].GetTowerWizard()->GetpSfWizard()[0], g_objTower[0].GetTowerWizard()->GetAttackDamage(), 4, 10);

	// 마법사 사거리
	UpgradeAbilityInfoPrint(675, 200, "Attack Range", 1, &g_sfBack.dcSurface, &g_surfaceInfo_Array[Surface_Button_Upgrade], upgradeLevel, upgradeGold,
		&g_objTower[0].GetTowerWizard()->GetpSfWizard()[0], g_objTower[0].GetTowerWizard()->GetAttackRange(), 20, 5);

	// 마법사 공격 속도
	UpgradeAbilityInfoPrint(950, 200, "Attack Delay", 2, &g_sfBack.dcSurface, &g_surfaceInfo_Array[Surface_Button_Upgrade], upgradeLevel, upgradeGold,
		&g_objTower[0].GetTowerWizard()->GetpSfWizard()[0], g_objTower[0].GetTowerWizard()->GetAttackDelay(), -0.15f, 0);

	// 최대 마나
	UpgradeAbilityInfoPrint(400, 360, "Max Mana", 3, &g_sfBack.dcSurface, &g_surfaceInfo_Array[Surface_Button_Upgrade], upgradeLevel, upgradeGold,
		&g_surfaceInfo_Array[Surface_Icon_Mana], maxMana, 10, 10);

	// 마나 회복 속도
	UpgradeAbilityInfoPrint(675, 360, "Mana Speed", 4, &g_sfBack.dcSurface, &g_surfaceInfo_Array[Surface_Button_Upgrade], upgradeLevel, upgradeGold,
		&g_surfaceInfo_Array[Surface_Icon_Mana], manaDelay, -0.12f, 0);

	// 타워 최대 체력
	UpgradeAbilityInfoPrint(950, 360, "Tower Max HP", 5, &g_sfBack.dcSurface, &g_surfaceInfo_Array[Surface_Button_Upgrade], upgradeLevel, upgradeGold,
		&g_surfaceInfo_Array[Surface_Icon_Tower], g_objTower[0].GetMaxHP(), 500, 0);

	SelectObject(g_sfBack.dcSurface, *oldFont);
	DeleteObject(*newFont);
}

void PaintGameScreen(CTower* g_objTower, CUnitInfo* g_infoUnit, CUnitInfo* playerUnit[MAX_UNIT_COUNT], CUnitInfo* enemyUnit[MAX_UNIT_COUNT], SURFACEINFO g_surfaceInfo_Array[Surface_Max], 
	SURFACEINFO g_btnUnit[UNIT_COUNT / 2], BOOL* buttonClick, int nBgX, int maxMana_InGame, int nowMana_InGame, int getGold, HFONT* newFont, HFONT* oldFont)
{
	PutStretch(g_sfBack.dcSurface, nBgX, 0, MAP_SIZE_RATE, MAP_SIZE_RATE, &g_surfaceInfo_Array[Surface_Background]);	// 배경

	// 타워
	g_objTower[0].GetTowerWizard()->DrawSurface(g_sfBack.dcSurface, nBgX);
	g_objTower[0].DrawSurface(g_sfBack.dcSurface, nBgX);
	g_objTower[1].DrawSurface(g_sfBack.dcSurface, nBgX);

	// 유닛 생성 버튼
	SetTextColor(g_sfBack.dcSurface, RGB(0, 255, 255));
	for (int i = 0; i < UNIT_COUNT / 2; i++)
	{
		PutSprite(g_sfBack.dcSurface, 100 + 110 * (i + 1), 350, &g_surfaceInfo_Array[Surface_Button_UnitCreate], 100, 100);	// 버튼
		PutSprite(g_sfBack.dcSurface, 110 + 110 * (i + 1), 310, &g_btnUnit[i]);		// 유닛 이미지

		PutSprite(g_sfBack.dcSurface, 125 + 110 * (i + 1), 390, &g_surfaceInfo_Array[Surface_Icon_Mana], 20, 20);	// 코스트
		char costPrint[16];
		wsprintf(costPrint, "%d", g_infoUnit[i * 2].GetCost());
		TextOut(g_sfBack.dcSurface, 150 + 110 * (i + 1), 390, costPrint, strlen(costPrint));
		DrawGauge(g_sfBack.dcSurface, 110 + 110 * (i + 1), 420, g_infoUnit[i * 2].GetCreateDelay() - g_infoUnit[i * 2].GetCreateDelayTimer(), g_infoUnit[i * 2].GetCreateDelay());		// 생성 딜레이 게이지
	}

	// 마나 업그레이드 버튼
	PutSprite(g_sfBack.dcSurface, 100, 350, &g_surfaceInfo_Array[Surface_Button_UnitCreate], 100, 100);		// 버튼
	PutSprite(g_sfBack.dcSurface, 125, 330, &g_surfaceInfo_Array[Surface_Icon_Mana], 50, 60);		// 유닛 이미지

	PutSprite(g_sfBack.dcSurface, 125, 400, &g_surfaceInfo_Array[Surface_Icon_Mana], 20, 20);		// 코스트
	char manaCostPrint[16];
	wsprintf(manaCostPrint, "%d", maxMana_InGame / 2);
	TextOut(g_sfBack.dcSurface, 150, 400, manaCostPrint, strlen(manaCostPrint));

	// 유닛
	for (int i = 0; i < MAX_UNIT_COUNT; i++)
	{
		if (playerUnit[i] != NULL)	playerUnit[i]->DrawSurface(g_sfBack.dcSurface, nBgX);
		if (enemyUnit[i] != NULL)	enemyUnit[i]->DrawSurface(g_sfBack.dcSurface, nBgX);
	}

	// 폰트 지정(마나, 골드 글자 크기)
	*newFont = CreateFont(25, 0, 0, 0, FW_HEAVY, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "굴림체");
	*oldFont = (HFONT)SelectObject(g_sfBack.dcSurface, *newFont);
	SetBkMode(g_sfBack.dcSurface, TRANSPARENT);

	// 마나
	SetTextColor(g_sfBack.dcSurface, RGB(0, 255, 255));
	PutSprite(g_sfBack.dcSurface, 10, 10, &g_surfaceInfo_Array[Surface_Icon_Mana]);
	char printMana[32];
	wsprintf(printMana, "%d/%d", nowMana_InGame, maxMana_InGame);
	TextOut(g_sfBack.dcSurface, 40, 12, printMana, strlen(printMana));

	// 획득 골드
	SetTextColor(g_sfBack.dcSurface, RGB(200, 200, 50));
	char printGetGold[32];
	wsprintf(printGetGold, "get Gold: %d", getGold);
	TextOut(g_sfBack.dcSurface, 940, 10, printGetGold, strlen(printGetGold));

	// 폰트 지정(타워 체력 표시)
	SelectObject(g_sfBack.dcSurface, *oldFont);
	*newFont = CreateFont(15, 0, 0, 0, FW_HEAVY, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "굴림체");
	*oldFont = (HFONT)SelectObject(g_sfBack.dcSurface, *newFont);
	SetTextColor(g_sfBack.dcSurface, RGB(0, 0, 0));

	// 타워 체력
	g_objTower[0].PrintTowerHP(g_sfBack.dcSurface, nBgX);
	g_objTower[1].PrintTowerHP(g_sfBack.dcSurface, nBgX);

	// 일시정지 버튼
	if (buttonClick[5]) PutSprite(g_sfBack.dcSurface, 1180, 5, &g_surfaceInfo_Array[Surface_Button_Round_PausePush], 50, 50);
	else				PutSprite(g_sfBack.dcSurface, 1180, 5, &g_surfaceInfo_Array[Surface_Button_Round_Pause], 50, 50);

	SelectObject(g_sfBack.dcSurface, *oldFont);
	DeleteObject(*newFont);
}

void PaintPauseScreen(SURFACEINFO g_surfaceInfo_Array[Surface_Max], BOOL* buttonClick)
{
	PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 200, MAP_HEIGHT / 2 - 100, &g_surfaceInfo_Array[Surface_Button_Upgrade], 400, 200); // 배경 창

	if (buttonClick[6]) PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 160, MAP_HEIGHT / 2 - 50, &g_surfaceInfo_Array[Surface_Button_Round_PlayPush], 100, 100); // 계속하기
	else				PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 160, MAP_HEIGHT / 2 - 50, &g_surfaceInfo_Array[Surface_Button_Round_Play], 100, 100);

	if (buttonClick[7]) PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 50, MAP_HEIGHT / 2 - 50, &g_surfaceInfo_Array[Surface_Button_Round_RePlayPush], 100, 100);// 다시하기
	else				PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 50, MAP_HEIGHT / 2 - 50, &g_surfaceInfo_Array[Surface_Button_Round_RePlay], 100, 100);

	if (buttonClick[8]) PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 + 60, MAP_HEIGHT / 2 - 50, &g_surfaceInfo_Array[Surface_Button_Round_QuitPush], 100, 100);	 // 나가기
	else				PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 + 60, MAP_HEIGHT / 2 - 50, &g_surfaceInfo_Array[Surface_Button_Round_Quit], 100, 100);
}

void PaintStageEndScreen(SURFACEINFO g_surfaceInfo_Array[Surface_Max], BOOL isWin, BOOL* buttonClick, int getGold, HFONT* newFont, HFONT* oldFont)
{
	SetBkMode(g_sfBack.dcSurface, TRANSPARENT);
	SetTextColor(g_sfBack.dcSurface, RGB(0, 0, 0));

	PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 350, MAP_HEIGHT / 2 - 200, &g_surfaceInfo_Array[Surface_Button_Upgrade], 700, 400); // 게임 결과 창
	PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 200, MAP_HEIGHT / 2 - 150, &g_surfaceInfo_Array[Surface_Result_Lose - isWin], 400, 100);	// 게임 결과 문자

	if (buttonClick[9]) PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 100, MAP_HEIGHT / 2 + 50, &g_surfaceInfo_Array[Surface_Button_Rectangle_BluePush], 200, 70); // 다음으로
	else				PutSprite(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 100, MAP_HEIGHT / 2 + 50, &g_surfaceInfo_Array[Surface_Button_Rectangle_Blue], 200, 70);

	*newFont = CreateFont(35, 0, 0, 0, FW_HEAVY, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "돋음체");
	*oldFont = (HFONT)SelectObject(g_sfBack.dcSurface, *newFont);
	TextOut(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - 30, MAP_HEIGHT / 2 + 70, "Next", strlen("Next"));

	// 획득 골드 출력
	SetTextColor(g_sfBack.dcSurface, RGB(200, 200, 50));
	*newFont = CreateFont(25, 0, 0, 0, FW_HEAVY, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "바탕체");
	*oldFont = (HFONT)SelectObject(g_sfBack.dcSurface, *newFont);
	char printGetGoldResult[32];
	wsprintf(printGetGoldResult, "Get Gold: %d", getGold);
	TextOut(g_sfBack.dcSurface, WINDOW_WIDTH / 2 - strlen(printGetGoldResult) * 7, 250, printGetGoldResult, strlen(printGetGoldResult));

	SelectObject(g_sfBack.dcSurface, *oldFont);
	DeleteObject(*newFont);
}

// 유닛 업그레이드 정보
void UpgradeUnitInfoPrint(CUnitInfo& unitInfo, int x, int y, HDC* backDcSurface, SURFACEINFO* g_sfUpgradeBtn)
{
	PutSprite(*backDcSurface, x, y, g_sfUpgradeBtn, 250, 140);
	PutSprite(*backDcSurface, x + 15, y + 10, &unitInfo.GetpSfUnit()[0]);

	char upgradeInfoText[32];
	SetTextColor(*backDcSurface, RGB(0, 0, 0));

	// 현재 레벨
	TextOut(*backDcSurface, x + 90, y + 10, "hp", strlen("hp"));					// 체력
	wsprintf(upgradeInfoText, "%d", unitInfo.GetMaxHP());
	TextOut(*backDcSurface, x + 145, y + 10, upgradeInfoText, strlen(upgradeInfoText));

	TextOut(*backDcSurface, x + 90, y + 25, "attack", strlen("attack"));			// 공격력
	wsprintf(upgradeInfoText, "%d", unitInfo.GetAttackDamage());
	TextOut(*backDcSurface, x + 145, y + 25, upgradeInfoText, strlen(upgradeInfoText));

	TextOut(*backDcSurface, x + 90, y + 40, "attSpe", strlen("attSpe"));			// 공격 딜레이
	sprintf_s(upgradeInfoText, "%.2f", unitInfo.GetAttackDelay());
	TextOut(*backDcSurface, x + 145, y + 40, upgradeInfoText, strlen(upgradeInfoText));

	TextOut(*backDcSurface, x + 90, y + 55, "create", strlen("create"));			// 생성 딜레이
	sprintf_s(upgradeInfoText, "%.2f", unitInfo.GetCreateDelay());
	TextOut(*backDcSurface, x + 145, y + 55, upgradeInfoText, strlen(upgradeInfoText));

	TextOut(*backDcSurface, x + 90, y + 70, "speed", strlen("speed"));				// 이동속도
	wsprintf(upgradeInfoText, "%d", unitInfo.GetMoveSpeed());
	TextOut(*backDcSurface, x + 145, y + 70, upgradeInfoText, strlen(upgradeInfoText));

	TextOut(*backDcSurface, x + 90, y + 85, "range", strlen("range"));				// 공격 사거리
	wsprintf(upgradeInfoText, "%d", unitInfo.GetAttackRange());
	TextOut(*backDcSurface, x + 145, y + 85, upgradeInfoText, strlen(upgradeInfoText));

	TextOut(*backDcSurface, x + 90, y + 100, "m_cost", strlen("m_cost"));					// 코스트
	wsprintf(upgradeInfoText, "%d", unitInfo.GetCost());
	TextOut(*backDcSurface, x + 145, y + 100, upgradeInfoText, strlen(upgradeInfoText));

	// 다음 레벨
	if (unitInfo.GetLevel() < 20)
	{
		if (unitInfo.GetLevel() % 5 == 4)
		{
			wsprintf(upgradeInfoText, "%d", (int)(unitInfo.GetMaxHP() * 1.1f * 1.1f));			// 체력
			TextOut(*backDcSurface, x + 200, y + 10, upgradeInfoText, strlen(upgradeInfoText));

			wsprintf(upgradeInfoText, "%d", (int)(unitInfo.GetAttackDamage() * 1.1f * 1.1f));	// 공격력
			TextOut(*backDcSurface, x + 200, y + 25, upgradeInfoText, strlen(upgradeInfoText));

			sprintf_s(upgradeInfoText, "%.2f", unitInfo.GetAttackDelay() * 0.95f * 0.95f);		// 공격 딜레이
			TextOut(*backDcSurface, x + 200, y + 40, upgradeInfoText, strlen(upgradeInfoText));

			sprintf_s(upgradeInfoText, "%.2f", unitInfo.GetCreateDelay() * 0.95f * 0.95f);		// 생성 딜레이
			TextOut(*backDcSurface, x + 200, y + 55, upgradeInfoText, strlen(upgradeInfoText));

			wsprintf(upgradeInfoText, "%d", (int)(unitInfo.GetMoveSpeed() + 1));				// 이동속도
			TextOut(*backDcSurface, x + 200, y + 70, upgradeInfoText, strlen(upgradeInfoText));

			wsprintf(upgradeInfoText, "%d", (int)(unitInfo.GetAttackRange() * 1.2f));			// 공격 사거리
			TextOut(*backDcSurface, x + 200, y + 85, upgradeInfoText, strlen(upgradeInfoText));

			wsprintf(upgradeInfoText, "%d", (int)(unitInfo.GetCost() + 2));						// 코스트
			TextOut(*backDcSurface, x + 200, y + 100, upgradeInfoText, strlen(upgradeInfoText));
		}
		else
		{
			wsprintf(upgradeInfoText, "%d", (int)(unitInfo.GetMaxHP() * 1.1f));					// 체력
			TextOut(*backDcSurface, x + 200, y + 10, upgradeInfoText, strlen(upgradeInfoText));

			wsprintf(upgradeInfoText, "%d", (int)(unitInfo.GetAttackDamage() * 1.1f));			// 공격력
			TextOut(*backDcSurface, x + 200, y + 25, upgradeInfoText, strlen(upgradeInfoText));

			sprintf_s(upgradeInfoText, "%.2f", unitInfo.GetAttackDelay() * 0.95f);				// 공격 딜레이
			TextOut(*backDcSurface, x + 200, y + 40, upgradeInfoText, strlen(upgradeInfoText));

			sprintf_s(upgradeInfoText, "%.2f", unitInfo.GetCreateDelay() * 0.95f);				// 생성 딜레이
			TextOut(*backDcSurface, x + 200, y + 55, upgradeInfoText, strlen(upgradeInfoText));

			wsprintf(upgradeInfoText, "%d", (int)(unitInfo.GetMoveSpeed()));					// 이동속도
			TextOut(*backDcSurface, x + 200, y + 70, upgradeInfoText, strlen(upgradeInfoText));

			wsprintf(upgradeInfoText, "%d", (int)(unitInfo.GetAttackRange()));					// 공격 사거리
			TextOut(*backDcSurface, x + 200, y + 85, upgradeInfoText, strlen(upgradeInfoText));

			wsprintf(upgradeInfoText, "%d", (int)(unitInfo.GetCost()));							// 코스트
			TextOut(*backDcSurface, x + 200, y + 100, upgradeInfoText, strlen(upgradeInfoText));
		}
	}

	TextOut(*backDcSurface, x + 180, y + 55, "->", strlen("->"));

	wsprintf(upgradeInfoText, "Lv.%d / 20", unitInfo.GetLevel());	// 레벨
	TextOut(*backDcSurface, x + 15, y + 80, upgradeInfoText, strlen(upgradeInfoText));

	SetTextColor(*backDcSurface, RGB(200, 200, 50));							// 업그레이드 비용
	wsprintf(upgradeInfoText, "%d Gold", unitInfo.GetLevelUpGold());
	TextOut(*backDcSurface, x + 15, y + 100, upgradeInfoText, strlen(upgradeInfoText));
}

// 마법사, 마나, 타워 업그레이드 정보
void UpgradeAbilityInfoPrint(int x, int y, char* upgradeName, int abilityNum, HDC* backDcSurface, SURFACEINFO* g_sfUpgradeBtn, int* upgradeLevel, int* upgradeGold,
	SURFACEINFO* psInfo, float nowAbil, float addAbil, int xPosAdjust)
{
	PutSprite(*backDcSurface, x, y, g_sfUpgradeBtn, 250, 140);

	SetTextColor(*backDcSurface, RGB(0, 0, 0));
	TextOut(*backDcSurface, x + 105, y + 35, upgradeName, strlen(upgradeName)); // 업그레이드 이름
	TextOut(*backDcSurface, x + 145, y + 65, "->", strlen("->"));

	char upgradeInfoText[32];
	wsprintf(upgradeInfoText, "Lv.%d / 10", upgradeLevel[abilityNum]); // 레벨
	TextOut(*backDcSurface, x + 15, y + 80, upgradeInfoText, strlen(upgradeInfoText));

	SetTextColor(*backDcSurface, RGB(200, 200, 50)); // 업그레이드 비용
	wsprintf(upgradeInfoText, "%d Gold", upgradeGold[abilityNum]);
	TextOut(*backDcSurface, x + 15, y + 100, upgradeInfoText, strlen(upgradeInfoText));

	if (abilityNum == 3 || abilityNum == 4)
		PutSprite(g_sfBack.dcSurface, x + 15, y + 10, psInfo, 50, 50);
	else
		PutSprite(g_sfBack.dcSurface, x + 15, y + 10, psInfo);

	if (abilityNum == 2 || abilityNum == 4)
	{
		SetTextColor(g_sfBack.dcSurface, RGB(0, 0, 0));
		sprintf_s(upgradeInfoText, "%.2f", nowAbil); // 현재 레벨 정보
		TextOut(g_sfBack.dcSurface, x + 100 + xPosAdjust, y + 65, upgradeInfoText, strlen(upgradeInfoText));
		if (upgradeLevel[abilityNum] < 10)
		{
			sprintf_s(upgradeInfoText, "%.2f", nowAbil + addAbil); // 다음 레벨 정보
			TextOut(g_sfBack.dcSurface, x + 175, y + 65, upgradeInfoText, strlen(upgradeInfoText));
		}
	}
	else
	{
		SetTextColor(g_sfBack.dcSurface, RGB(0, 0, 0));
		wsprintf(upgradeInfoText, "%d", (int)nowAbil); // 현재 레벨 정보
		TextOut(g_sfBack.dcSurface, x + 100 + xPosAdjust, y + 65, upgradeInfoText, strlen(upgradeInfoText));
		if (upgradeLevel[abilityNum] < 10)
		{
			wsprintf(upgradeInfoText, "%d", (int)nowAbil + (int)addAbil); // 다음 레벨 정보
			TextOut(g_sfBack.dcSurface, x + 175, y + 65, upgradeInfoText, strlen(upgradeInfoText));
		}
	}
}

// 자신 유닛 생성 딜레이 게이지
void DrawGauge(HDC hdc, int x, int y, float currentValue, float maxValue)
{
	if (maxValue <= 0)	return;			// maxValue가 0 이하인 경우 방지
	float fillRatio = currentValue / maxValue;
	if (fillRatio < 0)	fillRatio = 0;  // 최소 0
	if (fillRatio > 1)	fillRatio = 1;  // 최대 1

	Rectangle(hdc, x, y, x + BAR_WIDTH, y + BAR_HEIGHT);	// 게이지 배경 (회색)

	HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 255));		// 채워질 게이지 (파란색)
	RECT fillRect = { x, y, x + (BAR_WIDTH * fillRatio), y + BAR_HEIGHT };
	FillRect(hdc, &fillRect, hBrush);

	DeleteObject(hBrush);
}