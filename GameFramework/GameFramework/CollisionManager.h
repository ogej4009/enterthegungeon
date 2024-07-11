#pragma once
#include "Game.h"
#include "SerialNumber.h"

class CCollisionManager
{
private:
	list<class CCollider*>	m_ColliderList;// ��� �浹ü�� ��� ��� 
	list<class CCollider*>	m_UIColliderList;// UI�浹ü�� ��� ��� 
	list<class CCollider*>	m_SectionColliderList;// �����ؼ� �浹ó���� �ؾ��ϴ� ��� 
	list<class CCollider*>	m_SectionUIColliderList;// �����ؼ� �浹ó���� �ؾ��ϴ� UI��� 
	list<class CObj*>		m_WorldObjList; // ����� ��ġ�� ��� ������Ʈ ��� 
	CSerialNumber			m_SerialNumber;
	PCollisionProfile		m_pProfile;
	int						m_iProfileSize;
	int						m_iProfileCapacity;
	PCollisionChannel		m_pChannel;
	int						m_iChannelSize;
	int						m_iChannelCapacity;
	class CCollider* m_pMousePrevCollision;

private:
	unsigned int** m_pPrevCollisionAdj; // ���� ������ �浹ü �������
	unsigned int	m_iPrevCollisionCount; // ���� ������ �浹ü (intŸ��=4)����Ʈ��

public:
	void Clear();
	void ChangeScene();
	void ClearWorldList();

public:
	unsigned int CreateSerialNumber(); // �ø���ѹ��� �����ϴ� �Լ�
	void ReturnSerialNumber(unsigned int iNumber); // �ø���ѹ��� ��ȯ�ϴ� �Լ�

private:
	void AddPrevCollisionAdj(unsigned int iSrc, unsigned int iDest); // ���� ������ �浹ü ������� �߰�

public:
	void DeletePrevCollisionAdj(unsigned int iSrc, unsigned int iDest); // ���� ������ �浹ü ������� ����
	bool CheckPrevCollisionAdj(unsigned int iSrc, unsigned int iDest); // ���� ������ �浹ü ������� üũ (�ִ�,����)

public:
	void AddWorldObjList(class CObj* pObj); // ������� ������Ʈ ��� �߰� 
	void AddCollider(class CCollider* pCollider); //  �浹ü �߰�
	void DeleteCollider(class CScene* pScene); // � ��鿡 �Ҽӵ� �浹ü�� ��ü ����

private:
	void AddProfile(const string& strName); // ���� ���ϴ� �浹 ���������� �ϳ� �߰����ִ� ��� 
	void AddChannel(const string& strName,
		COLLISION_CHANNEL_STATE eDefaultState = CCS_BLOCK); // ���� ���ϴ� �浹 ä���� �ϳ� �߰����ִ� ���
	void SetProfileChannelState(const string& strProfile,
		const string& strChannel, COLLISION_CHANNEL_STATE eState); // �������ϰ� ä���� ���踦 �����ϴ� ���
	void SetProfileChannelStateAll(const string& strProfile,
		COLLISION_CHANNEL_STATE eState); // �������ϰ� ä���� ���踦 �����ϴ� ��� ��� ä���� �� �ٲٴ� ��

public:
	PCollisionProfile FindProfile(const string& strName); // ���� ���� �� �������� ã�Ƽ� ���� �ִ� ��������
	int GetChannelIndex(const string& strName)	const; // ä�� �ε����� ��ȯ�ϴ� ���

public:
	bool Init();
	void Collision(float fTime); // ���� �浹ó�� ������ �����ִ� �Լ�

private:
	bool CollisionMouseUI(float fTime);
	bool CollisionMouseWorld(float fTime);
	void CollisionWorld(float fTime);	// ���忡 ��ġ�� �浹ü 
	void CollisionUI(float fTime); // UI�� ��ġ�� �浹ü 

	DECLARE_SINGLE(CCollisionManager)
};

