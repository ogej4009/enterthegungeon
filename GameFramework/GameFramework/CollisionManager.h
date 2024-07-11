#pragma once
#include "Game.h"
#include "SerialNumber.h"

class CCollisionManager
{
private:
	list<class CCollider*>	m_ColliderList;// 모든 충돌체를 담는 목록 
	list<class CCollider*>	m_UIColliderList;// UI충돌체를 담는 목록 
	list<class CCollider*>	m_SectionColliderList;// 조사해서 충돌처리를 해야하는 목록 
	list<class CCollider*>	m_SectionUIColliderList;// 조사해서 충돌처리를 해야하는 UI목록 
	list<class CObj*>		m_WorldObjList; // 월드상에 배치될 모든 오브젝트 목록 
	CSerialNumber			m_SerialNumber;
	PCollisionProfile		m_pProfile;
	int						m_iProfileSize;
	int						m_iProfileCapacity;
	PCollisionChannel		m_pChannel;
	int						m_iChannelSize;
	int						m_iChannelCapacity;
	class CCollider* m_pMousePrevCollision;

private:
	unsigned int** m_pPrevCollisionAdj; // 이전 프레임 충돌체 인접행렬
	unsigned int	m_iPrevCollisionCount; // 이전 프레임 충돌체 (int타입=4)바이트수

public:
	void Clear();
	void ChangeScene();
	void ClearWorldList();

public:
	unsigned int CreateSerialNumber(); // 시리얼넘버를 생성하는 함수
	void ReturnSerialNumber(unsigned int iNumber); // 시리얼넘버를 반환하는 함수

private:
	void AddPrevCollisionAdj(unsigned int iSrc, unsigned int iDest); // 이전 프레임 충돌체 인접행렬 추가

public:
	void DeletePrevCollisionAdj(unsigned int iSrc, unsigned int iDest); // 이전 프레임 충돌체 인접행렬 삭제
	bool CheckPrevCollisionAdj(unsigned int iSrc, unsigned int iDest); // 이전 프레임 충돌체 인접행렬 체크 (있다,없다)

public:
	void AddWorldObjList(class CObj* pObj); // 월드상의 오브젝트 목록 추가 
	void AddCollider(class CCollider* pCollider); //  충돌체 추가
	void DeleteCollider(class CScene* pScene); // 어떤 장면에 소속된 충돌체를 전체 지움

private:
	void AddProfile(const string& strName); // 내가 원하는 충돌 프로파일을 하나 추가해주는 기능 
	void AddChannel(const string& strName,
		COLLISION_CHANNEL_STATE eDefaultState = CCS_BLOCK); // 내가 원하는 충돌 채널을 하나 추가해주는 기능
	void SetProfileChannelState(const string& strProfile,
		const string& strChannel, COLLISION_CHANNEL_STATE eState); // 프로파일과 채널의 관계를 설정하는 기능
	void SetProfileChannelStateAll(const string& strProfile,
		COLLISION_CHANNEL_STATE eState); // 프로파일과 채널의 관계를 설정하는 기능 모든 채널을 다 바꾸는 것

public:
	PCollisionProfile FindProfile(const string& strName); // 내가 원할 때 언제든지 찾아서 쓸수 있는 프로파일
	int GetChannelIndex(const string& strName)	const; // 채널 인덱스를 반환하는 기능

public:
	bool Init();
	void Collision(float fTime); // 실제 충돌처리 로직을 돌려주는 함수

private:
	bool CollisionMouseUI(float fTime);
	bool CollisionMouseWorld(float fTime);
	void CollisionWorld(float fTime);	// 월드에 배치된 충돌체 
	void CollisionUI(float fTime); // UI에 배치된 충돌체 

	DECLARE_SINGLE(CCollisionManager)
};

