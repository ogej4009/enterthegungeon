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
	CS_BLOCK, // 부딪치고 관통 못하게 막음 : 이벤트 발생 시키겠다. 
	CS_OVERLAP, // 트리거 형태 관통은 되지만 실시간으로 이벤트가 발생 
	CS_RELEASE, // 충돌 자체를 무시
	CS_END
};

enum ANIMATION_TYPE
{
	AT_ATLAS,
	AT_FRAME
};

enum ANIMATION_OPTION
{
	AO_LOOP, // 계속 실행되는 옵션 
	AO_ONCE_RETURN, // 한번하고 반환되는 옵션 
	AO_ONCE_DESTROY, // 한번하고 파괴되는 옵션 
	AO_ONCE_FRAMESTOP, // ★ 내가 원하는 특정 프레임에서 멈추게 하고 싶다. 
	AO_TIME_DESTROY	// 시간제한으로 파괴되는 옵션 	
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

