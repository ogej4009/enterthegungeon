#pragma once
#include "Game.h"

class CCamera
{

private:
	Position	m_tPos; // ī�޶��� ��ġ 
	Size		m_tResolution; // ī�޶� �ػ� 
	Position	m_tSceneStartPos; // (ī�޶�� �������) ����� �������� 
	Size		m_tSceneSize; // ����� ��ü ũ�� 
	Position	m_tPivot;	// Ÿ���� ȭ���� ��� �߽����� ���� �����Ѵ�.
	class CObj* m_pTarget; // ī�޶� (���� �ٳ�� ��) ���� 
	class CInputComponent* m_pInputComponent; // ī�޶��� �̵�Ű�� �����ϱ� ���� 

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

	void SetTarget(class CObj* pTarget); // ī�޶��� Ÿ���� ���Ѵ�. 
	void OnFreeCamera(); // ������Ʈ�� ���� ī�޶��� Ű�� ����ϴ� �Լ� 
	void MoveFront(float fScale, float fTime); // ���� �̵� 
	void MoveSide(float fScale, float fTime); // ���� �̵� 

public:

	// �� Update�� Ÿ���� �����ϰ� ī�޶� Ÿ���� ���� �ٴϰ� �Ѵ�. 
	void Update(float fTime);

	// �̱��� ����  
	DECLARE_SINGLE(CCamera)
};

// "Data"
