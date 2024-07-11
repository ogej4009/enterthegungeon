#pragma once

enum INPUT_EVENT
{
	IE_PRESS,
	IE_UP,
	IE_DOUBLE,
	IE_HOLD
};

enum MOVE_DIR
{
	BACK = -1,
	NONE,
	FRONT
};

enum DIR
{
	LEFT,
	RIGHT,
	UP,
	DOWN,
	DIR_END
};

enum COLLIDER_TYPE
{
	CT_RECT,
	CT_SPHERE,
	CT_POINT,
	CT_PIXEL
};

enum COLLISION_CHANNEL_STATE
{
	CCS_BLOCK,
	CCS_IGNORE
};

enum COLLISION_STATE
{
	CS_BLOCK, // �ε�ġ�� ���� ���ϰ� ���� : �̺�Ʈ �߻� ��Ű�ڴ�. 
	CS_OVERLAP, // Ʈ���� ���� ������ ������ �ǽð����� �̺�Ʈ�� �߻� 
	CS_RELEASE, // �浹 ��ü�� ����
	CS_END
};

enum ANIMATION_TYPE
{
	AT_ATLAS,
	AT_FRAME
};

enum ANIMATION_OPTION
{
	AO_LOOP, // ��� ����Ǵ� �ɼ� 
	AO_ONCE_RETURN, // �ѹ��ϰ� ��ȯ�Ǵ� �ɼ� 
	AO_ONCE_DESTROY, // �ѹ��ϰ� �ı��Ǵ� �ɼ� 
	AO_ONCE_FRAMESTOP, // �� ���� ���ϴ� Ư�� �����ӿ��� ���߰� �ϰ� �ʹ�. 
	AO_TIME_DESTROY	// �ð��������� �ı��Ǵ� �ɼ� 	
};

enum SIDE_DIR
{
	SD_LEFT,
	SD_RIGHT,
	SD_END
};

enum SCENE_CHANGE
{
	SC_NONE,
	SC_CHANGE
};

enum TILE_OPTION
{
	TO_NONE = -1,
	TO_NORMAL,
	TO_WALL,
	TO_END
};

