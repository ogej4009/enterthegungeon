#pragma once
#include "Game.h"

class CCamera
{

private:
	Position	m_tPos; // 카메라의 위치 
	Size		m_tResolution; // 카메라 해상도 
	Position	m_tSceneStartPos; // (카메라와 상관없는) 장면의 시작지점 
	Size		m_tSceneSize; // 장면의 전체 크기 
	Position	m_tPivot;	// 타겟을 화면의 어디를 중심으로 할지 결정한다.
	class CObj* m_pTarget; // 카메라가 (따라 다녀야 할) 초점 
	class CInputComponent* m_pInputComponent; // 카메라의 이동키를 설정하기 위함 

public:// get

	Position GetPos()	const
	{
		return m_tPos;
	}

	Size GetResolution()	const
	{
		return m_tResolution;
	}

	Position GetSceneStartPos()	const
	{
		return m_tSceneStartPos;
	}

	Size GetSceneSize()	const
	{
		return m_tSceneSize;
	}

	bool IsTarget()	const
	{
		return m_pTarget != nullptr;
	}

public: // set

	void SetPos(const Position& tPos)
	{
		m_tPos = tPos;
	}

	void SetPos(float x, float y)
	{
		m_tPos.x = x;
		m_tPos.y = y;
	}

	void SetPivot(const Position& tPivot)
	{
		m_tPivot = tPivot;
	}

	void SetPivot(float x, float y)
	{
		m_tPivot.x = x;
		m_tPivot.y = y;
	}

	void SetSceneStartPos(const Position& tPos)
	{
		m_tSceneStartPos = tPos;
	}

	void SetSceneStartPos(float x, float y)
	{
		m_tSceneStartPos.x = x;
		m_tSceneStartPos.y = y;
	}

	void SetResolution(const Size& tResolution)
	{
		m_tResolution = tResolution;
	}

	void SetResolution(float x, float y)
	{
		m_tResolution.x = x;
		m_tResolution.y = y;
	}

	void SetSceneSize(const Size& tSize)
	{
		m_tSceneSize = tSize;
	}

	void SetSceneSize(float x, float y)
	{
		m_tSceneSize.x = x;
		m_tSceneSize.y = y;
	}

	void SetTarget(class CObj* pTarget); // 카메라의 타겟을 정한다. 
	void OnFreeCamera(); // 컴포넌트를 통해 카메라의 키를 등록하는 함수 
	void MoveFront(float fScale, float fTime); // 실제 이동 
	void MoveSide(float fScale, float fTime); // 실제 이동 

public:

	// ※ Update는 타겟을 지정하고 카메라가 타겟을 따라 다니게 한다. 
	void Update(float fTime);

	// 싱글톤 선언  
	DECLARE_SINGLE(CCamera)
};

// "Data"
