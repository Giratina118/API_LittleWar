#ifndef __DEFINEDATA_H__
#define __DEFINEDATA_H__

#include <Windows.h>

#define UNIT_COUNT		6
#define UNIT_SF_COUNT	5

#define XSPEED_JUMP 7		// 이동 속도
#define XSPEED_WALK 10
#define YSPEED_JUMP -20
#define GRAVITY		1.0

#define MAX_YPOS 300		// 한계 이동 반경
#define MAX_XPOS 500
#define MIN_XPOS 100

#define MAP_SIZE_RATE	5
#define UNIT_SIZE_RATE	3
#define MAP_WIDTH		500*MAP_SIZE_RATE
#define MAP_HEIGHT		110*MAP_SIZE_RATE
#define WINDOW_WIDTH	500*2.5

#define MAX_UNIT_COUNT	50
#define FIREBALL_MAX	64

#define UNIT_MAX_LEVEL		20
#define ABILITY_MAX_LEVEL	10

#define PI 3.141592

#define BAR_WIDTH 80	// 게이지 가로
#define BAR_HEIGHT 15   // 게이지 세로

#define STARTGOLD 100000	// 테스트용 금액

typedef struct SURFACEINFOtag
{
	HDC			dcSurface;		// 비트맵이 적재되있는 표면 DC
	HBITMAP		hBmp;			// 비트맵 핸들
	HBITMAP		hOldBmp;		// 이전 비트맵 핸들 ( dc 해제시 참조 )
	int			nTop;
	int			nLeft;
	int			nWidth;
	int			nHeight;
	COLORREF	crColorKey;		// 컬러키
} SURFACEINFO;

enum Screen
{
	titleScreen = 0,	// 이어하기, 새로시작, 종료
	readyScreen,		// 현재 스테이지 보여주기, 게임 시작, 유닛 건물 능력 업그레이드
	gameScreen,			// 게임 화면
	pauseScreen,		// 일시정지 창
	stageEndScreen		// 게임 결과 창
};

#endif