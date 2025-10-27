#ifndef __DEFINEDATA_H__
#define __DEFINEDATA_H__

#include <Windows.h>

#define UNIT_COUNT		6	// 유닛 종류(플레이어 3 + 적 3)
#define UNIT_SF_COUNT	5	// 유닛 별 이미지 종류(기본, 공격, 피격_빨강, 피격, 죽음)

#define MAP_SIZE_RATE	5	// 맵 이미지 배율
#define UNIT_SIZE_RATE	3	// 유닛 이미지 배율
#define MAP_WIDTH		500 * MAP_SIZE_RATE // 맵 가로 길이
#define MAP_HEIGHT		110 * MAP_SIZE_RATE // 맵 세로 길이 = 윈도우 세로 길이
#define WINDOW_WIDTH	500 * 2.5			// 윈도우 가로 길이

#define MAX_UNIT_COUNT	64	// 최대 유닛 수(플레이어 64, 적 64)
#define FIREBALL_MAX	128 // 최대 파이어볼 수

#define UNIT_MAX_LEVEL		20 // 유닛 업그레이드 최대 레벨
#define ABILITY_MAX_LEVEL	10 // 능력 업그레이드 최대 레벨

#define PI 3.141592		// 원주율

#define BAR_WIDTH 80	// 게이지 가로 길이 (유닛 생성 쿨타임 게이지)
#define BAR_HEIGHT 15   // 게이지 세로 길이 (유닛 생성 쿨타임 게이지)

#define STARTGOLD 100000	// 테스트용 금액


typedef struct SURFACEINFOtag // 이미지파일(비트맵) 정보 들어있는 구조체
{
	HDC			dcSurface;	// 비트맵이 적재되있는 표면 DC
	HBITMAP		hBmp;		// 비트맵 핸들
	HBITMAP		hOldBmp;	// 이전 비트맵 핸들 ( dc 해제시 참조 )
	int			nTop;		// 비트맵 크기
	int			nLeft;
	int			nWidth;
	int			nHeight;
	COLORREF	crColorKey;	// 컬러키
} SURFACEINFO;

enum Screen
{
	Screen_Title,	// 이어하기, 새로시작, 종료
	Screen_Ready,	// 현재 스테이지 보여주기, 게임 시작, 유닛 건물 능력 업그레이드
	Screen_Game,	// 게임 화면
	Screen_Pause,	// 일시정지 창
	Screen_StageEnd	// 게임 결과 창
};

enum SURFACETYPE
{
	Surface_Background,					// 배경 표면 
	Surface_Title,						// 타이틀 그림
	Surface_Result_Win,					// 게임 결과 승리
	Surface_Result_Lose,				// 게임 결과 패배
	Surface_Button_UnitCreate,			// 버튼(유닛 소환)
	Surface_Button_Screen,				// 버튼(게임 시작, 나가기 등)
	Surface_Button_Upgrade,				// 업그레이트 버튼
	Surface_Button_Round_Play,			// 둥근 버튼, 재생 버튼
	Surface_Button_Round_PlayPush,		// 둥근 버튼, 재생 버튼 눌림
	Surface_Button_Round_Pause,			// 둥근 버튼, 일시정지 버튼
	Surface_Button_Round_PausePush,		// 둥근 버튼, 일시정지 버튼 눌림
	Surface_Button_Round_RePlay,		// 둥근 버튼, 리플레이 버튼
	Surface_Button_Round_RePlayPush,	// 둥근 버튼, 리플레이 버튼 눌림
	Surface_Button_Round_Quit,			// 둥근 버튼, 종료 버튼
	Surface_Button_Round_QuitPush,		// 둥근 버튼, 종료 버튼 눌림
	Surface_Button_Rectangle_Blue,		// 사각 버튼, 파란 버튼
	Surface_Button_Rectangle_BluePush,	// 사각 버튼, 파랑 버튼 눌림
	Surface_Button_Rectangle_Red,		// 사각 버튼, 빨간 버튼
	Surface_Button_Rectangle_RedPush,	// 사각 버튼, 빨간 버튼 눌림
	Surface_Icon_Mana,					// 마나 아이콘
	Surface_Icon_Gold,					// 골드 아이콘
	Surface_Icon_Tower,					// 타워 아이콘
	Surface_Arrow_Right,				// 오른쪽 화살표
	Surface_Arrow_Up,					// 위쪽 화살표

	Surface_Max,
};

#endif