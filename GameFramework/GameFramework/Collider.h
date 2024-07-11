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
	COLLIDER_TYPE	m_eColliderType; // 점충돌, 픽셀, 렉트, 스피어 
	class CObj* m_pObj;
	bool			m_bCollision; // 충돌을 판단한다. 
	class CScene* m_pScene;
	bool			m_bIsUI; // UI에 속했는지 판단한다. 
	Position		m_tIntersect; // 충돌시 교점 
	unsigned int	m_iSerialNumber;
	PCollisionProfile	m_pProfile; // 프로파일 
	int					m_iChannelIndex; // 채널 
	list<CCollider*>	m_PrevColliderList; // 이전 충돌 목록 
	function<void(CCollider*, CCollider*, float)>	m_Callback[CS_END]; // 블록, 오버랩, 릴리즈 
	// 첫번째 인자는 소스 충돌체, 두번째인자는 상대(데스트) 충돌체
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
													  나와 충돌되는 것을 테스트한다.
													  순수가상함수 : 부모에서 할일없다. 자식에서 재구현
															CColliderPoint
															CColliderRect
															CColliderSphere
													  */

													  // 저장, 로드를 위한 함수 
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void UpdateObjPos(const Position& tPos);/*
														오브젝트 위치정보를 갱신할 때
														충돌체들의 위치에까지 영향을 주게 만들어준 함수
													*/

protected:
	bool CollisionRectToRect(CCollider* pSrc, CCollider* pDest);
	bool CollisionSphereToSphere(CCollider* pSrc, CCollider* pDest);
	bool CollisionRectToSphere(CCollider* pSrc, CCollider* pDest);
	bool CollisionRectToPoint(CCollider* pSrc, CCollider* pDest);
	bool CollisionSphereToPoint(CCollider* pSrc, CCollider* pDest);
	bool CollisionPointToPoint(CCollider* pSrc, CCollider* pDest);

protected:// 다양한 모양이 존재할 때, 그들끼리 충돌을 판단해 주는 함수 : 자기자신의 함수를 호출한다
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
			위에서
			function<void(CCollider*, CCollider*, float)>	m_Callback[CS_END]; // 충돌 콜백 3개 짜리
		*/

		// 인자 3 : 충돌상태, 충돌대상, 게임시간적용
	void Call(COLLISION_STATE eState, CCollider* pDest, float fTime)
	{
		if (m_Callback[eState] != nullptr)
			m_Callback[eState](this, pDest, fTime);
	}

	// 인자 2 : 충돌상태, 함수주소 
	void SetCollisionCallback(COLLISION_STATE eState,
		void(*pFunc)(CCollider*, CCollider*, float))
	{
		m_Callback[eState] = bind(pFunc,
			placeholders::_1, placeholders::_2, placeholders::_3);
	}
	// 인자 3 : 충돌상태, 오브젝트, 함수주소
	template <typename T>
	void SetCollisionCallback(COLLISION_STATE eState,
		T* pObj, void(T::* pFunc)(CCollider*, CCollider*, float))
	{
		m_Callback[eState] = bind(pFunc, pObj,
			placeholders::_1, placeholders::_2, placeholders::_3);
	}
};

