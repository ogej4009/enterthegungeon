#pragma once
#include "Ref.h"


class CCollider :
	public CRef
{
	friend class CObj;
	friend class CCollisionManager;

protected:
	CCollider();
	CCollider(const CCollider& collider);
	virtual ~CCollider() = 0;

protected:
	COLLIDER_TYPE	m_eColliderType; // ���浹, �ȼ�, ��Ʈ, ���Ǿ� 
	class CObj* m_pObj;
	bool			m_bCollision; // �浹�� �Ǵ��Ѵ�. 
	class CScene* m_pScene;
	bool			m_bIsUI; // UI�� ���ߴ��� �Ǵ��Ѵ�. 
	Position		m_tIntersect; // �浹�� ���� 
	unsigned int	m_iSerialNumber;
	PCollisionProfile	m_pProfile; // �������� 
	int					m_iChannelIndex; // ä�� 
	list<CCollider*>	m_PrevColliderList; // ���� �浹 ��� 
	function<void(CCollider*, CCollider*, float)>	m_Callback[CS_END]; // ���, ������, ������ 
	// ù��° ���ڴ� �ҽ� �浹ü, �ι�°���ڴ� ���(����Ʈ) �浹ü
public:
	void SetProfile(const string& strName);
	void SetChannel(const string& strName);
	PCollisionProfile GetProfile()	const
	{
		return m_pProfile;
	}

	int GetChannelIndex()	const
	{
		return m_iChannelIndex;
	}

public:
	unsigned int GetSerialNumber()	const
	{
		return m_iSerialNumber;
	}

	Position GetIntersect()	const
	{
		return m_tIntersect;
	}

	COLLIDER_TYPE GetColliderType()	const
	{
		return m_eColliderType;
	}

	class CScene* GetScene()	const
	{
		return m_pScene;
	}

	void SetScene(class CScene* pScene)
	{
		m_pScene = pScene;
	}

	bool IsUI()	const
	{
		return m_bIsUI;
	}

	void EnableUI()
	{
		m_bIsUI = true;
	}

public:
	void AddPrevCollider(CCollider* pCollider);
	void DeletePrevCollider(CCollider* pCollider);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CCollider* Clone()	const;
	virtual bool Collision(CCollider* pCollider) = 0; /*
													  ���� �浹�Ǵ� ���� �׽�Ʈ�Ѵ�.
													  ���������Լ� : �θ𿡼� ���Ͼ���. �ڽĿ��� �籸��
															CColliderPoint
															CColliderRect
															CColliderSphere
													  */

													  // ����, �ε带 ���� �Լ� 
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void UpdateObjPos(const Position& tPos);/*
														������Ʈ ��ġ������ ������ ��
														�浹ü���� ��ġ������ ������ �ְ� ������� �Լ�
													*/

protected:
	bool CollisionRectToRect(CCollider* pSrc, CCollider* pDest);
	bool CollisionSphereToSphere(CCollider* pSrc, CCollider* pDest);
	bool CollisionRectToSphere(CCollider* pSrc, CCollider* pDest);
	bool CollisionRectToPoint(CCollider* pSrc, CCollider* pDest);
	bool CollisionSphereToPoint(CCollider* pSrc, CCollider* pDest);
	bool CollisionPointToPoint(CCollider* pSrc, CCollider* pDest);

protected:// �پ��� ����� ������ ��, �׵鳢�� �浹�� �Ǵ��� �ִ� �Լ� : �ڱ��ڽ��� �Լ��� ȣ���Ѵ�
	bool CollisionRectToRect(const RectInfo& tSrc,
		const RectInfo& tDest, Position& tIntersect);
	bool CollisionSphereToSphere(const SphereInfo& tSrc,
		const SphereInfo& tDest, Position& tIntersect);
	bool CollisionRectToSphere(const RectInfo& tSrc,
		const SphereInfo& tDest, Position& tIntersect);
	bool CollisionRectToPoint(const RectInfo& tSrc,
		const Position& tDest, Position& tIntersect);
	bool CollisionSphereToPoint(const SphereInfo& tSrc,
		const Position& tDest, Position& tIntersect);
	bool CollisionPointToPoint(const Position& tSrc,
		const Position& tDest, Position& tIntersect);

public: /*
			������
			function<void(CCollider*, CCollider*, float)>	m_Callback[CS_END]; // �浹 �ݹ� 3�� ¥��
		*/

		// ���� 3 : �浹����, �浹���, ���ӽð�����
	void Call(COLLISION_STATE eState, CCollider* pDest, float fTime)
	{
		if (m_Callback[eState] != nullptr)
			m_Callback[eState](this, pDest, fTime);
	}

	// ���� 2 : �浹����, �Լ��ּ� 
	void SetCollisionCallback(COLLISION_STATE eState,
		void(*pFunc)(CCollider*, CCollider*, float))
	{
		m_Callback[eState] = bind(pFunc,
			placeholders::_1, placeholders::_2, placeholders::_3);
	}
	// ���� 3 : �浹����, ������Ʈ, �Լ��ּ�
	template <typename T>
	void SetCollisionCallback(COLLISION_STATE eState,
		T* pObj, void(T::* pFunc)(CCollider*, CCollider*, float))
	{
		m_Callback[eState] = bind(pFunc, pObj,
			placeholders::_1, placeholders::_2, placeholders::_3);
	}
};

