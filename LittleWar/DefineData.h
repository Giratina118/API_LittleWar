#ifndef __DEFINEDATA_H__
#define __DEFINEDATA_H__

#include <Windows.h>

#define UNIT_COUNT		6	// ���� ����(�÷��̾� 3 + �� 3)
#define UNIT_SF_COUNT	5	// ���� �� �̹��� ����(�⺻, ����, �ǰ�_����, �ǰ�, ����)

#define MAP_SIZE_RATE	5	// �� �̹��� ����
#define UNIT_SIZE_RATE	3	// ���� �̹��� ����
#define MAP_WIDTH		500 * MAP_SIZE_RATE // �� ���� ����
#define MAP_HEIGHT		110 * MAP_SIZE_RATE // �� ���� ���� = ������ ���� ����
#define WINDOW_WIDTH	500 * 2.5			// ������ ���� ����

#define MAX_UNIT_COUNT	64	// �ִ� ���� ��(�÷��̾� 64, �� 64)
#define FIREBALL_MAX	128 // �ִ� ���̾ ��

#define UNIT_MAX_LEVEL		20 // ���� ���׷��̵� �ִ� ����
#define ABILITY_MAX_LEVEL	10 // �ɷ� ���׷��̵� �ִ� ����

#define PI 3.141592		// ������

#define BAR_WIDTH 80	// ������ ���� ���� (���� ���� ��Ÿ�� ������)
#define BAR_HEIGHT 15   // ������ ���� ���� (���� ���� ��Ÿ�� ������)

#define STARTGOLD 100000	// �׽�Ʈ�� �ݾ�


typedef struct SURFACEINFOtag // �̹�������(��Ʈ��) ���� ����ִ� ����ü
{
	HDC			dcSurface;	// ��Ʈ���� ������ִ� ǥ�� DC
	HBITMAP		hBmp;		// ��Ʈ�� �ڵ�
	HBITMAP		hOldBmp;	// ���� ��Ʈ�� �ڵ� ( dc ������ ���� )
	int			nTop;		// ��Ʈ�� ũ��
	int			nLeft;
	int			nWidth;
	int			nHeight;
	COLORREF	crColorKey;	// �÷�Ű
} SURFACEINFO;

enum Screen
{
	Screen_Title,	// �̾��ϱ�, ���ν���, ����
	Screen_Ready,	// ���� �������� �����ֱ�, ���� ����, ���� �ǹ� �ɷ� ���׷��̵�
	Screen_Game,	// ���� ȭ��
	Screen_Pause,	// �Ͻ����� â
	Screen_StageEnd	// ���� ��� â
};

enum SURFACETYPE
{
	Surface_Background,					// ��� ǥ�� 
	Surface_Title,						// Ÿ��Ʋ �׸�
	Surface_Result_Win,					// ���� ��� �¸�
	Surface_Result_Lose,				// ���� ��� �й�
	Surface_Button_UnitCreate,			// ��ư(���� ��ȯ)
	Surface_Button_Screen,				// ��ư(���� ����, ������ ��)
	Surface_Button_Upgrade,				// ���׷���Ʈ ��ư
	Surface_Button_Round_Play,			// �ձ� ��ư, ��� ��ư
	Surface_Button_Round_PlayPush,		// �ձ� ��ư, ��� ��ư ����
	Surface_Button_Round_Pause,			// �ձ� ��ư, �Ͻ����� ��ư
	Surface_Button_Round_PausePush,		// �ձ� ��ư, �Ͻ����� ��ư ����
	Surface_Button_Round_RePlay,		// �ձ� ��ư, ���÷��� ��ư
	Surface_Button_Round_RePlayPush,	// �ձ� ��ư, ���÷��� ��ư ����
	Surface_Button_Round_Quit,			// �ձ� ��ư, ���� ��ư
	Surface_Button_Round_QuitPush,		// �ձ� ��ư, ���� ��ư ����
	Surface_Button_Rectangle_Blue,		// �簢 ��ư, �Ķ� ��ư
	Surface_Button_Rectangle_BluePush,	// �簢 ��ư, �Ķ� ��ư ����
	Surface_Button_Rectangle_Red,		// �簢 ��ư, ���� ��ư
	Surface_Button_Rectangle_RedPush,	// �簢 ��ư, ���� ��ư ����
	Surface_Icon_Mana,					// ���� ������
	Surface_Icon_Gold,					// ��� ������
	Surface_Icon_Tower,					// Ÿ�� ������
	Surface_Arrow_Right,				// ������ ȭ��ǥ
	Surface_Arrow_Up,					// ���� ȭ��ǥ

	Surface_Max,
};

#endif