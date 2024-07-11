#include "CollisionManager.h"
#include "Collider.h"
#include "Input.h"
#include "UI.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Stage.h"

DEFINITION_SINGLE(CCollisionManager)

CCollisionManager::CCollisionManager()
{
	m_pMousePrevCollision = nullptr;
}

CCollisionManager::~CCollisionManager()
{
}

void CCollisionManager::Clear()
{
	unsigned int	iWidth = m_iPrevCollisionCount / 32;

	for (unsigned int i = 0; i < m_iPrevCollisionCount; ++i)
	{
		SAFE_DELETE_ARRAY(m_pPrevCollisionAdj[i]);
	}

	SAFE_DELETE_ARRAY(m_pPrevCollisionAdj);

	SAFE_DELETE_ARRAY(m_pProfile);
	SAFE_DELETE_ARRAY(m_pChannel);
	Safe_Release_VecList(m_WorldObjList);
	Safe_Release_VecList(m_ColliderList);
	Safe_Release_VecList(m_UIColliderList);
}

void CCollisionManager::ChangeScene()
{
	Safe_Release_VecList(m_WorldObjList);
	m_pMousePrevCollision = nullptr;
}

void CCollisionManager::ClearWorldList()
{
	Safe_Release_VecList(m_WorldObjList);
}

unsigned int CCollisionManager::CreateSerialNumber()
{
	return m_SerialNumber.CreateSerialNumber();
}

void CCollisionManager::ReturnSerialNumber(unsigned int iNumber)
{
	m_SerialNumber.ReturnSerialNumber(iNumber);
}

void CCollisionManager::AddPrevCollisionAdj(unsigned int iSrc,
	unsigned int iDest)
{
	// 32 �� ������ �迭�� �ε����� ���� : ��� ° �׷����� �˼� ���� : �迭�� �ε����� �˾ƺ��� ��� 
	unsigned int	iSrcIdx = iSrc / 32;// �������� ǥ�� ���� int�� �ִ� 32�ڸ� 
	unsigned int	iDestIdx = iDest / 32;

	// 32 �� ���� �������� 32��° (����31)���� ���� �տ������� ���� �ϴ� �迭ó�� ����� ������ �� �ְ� �����.
	// �ε����� �˾ƺ� �Ŀ� �� �ε������� ���� ��Ʈ�� �ɰ��� ���� ���
	unsigned int	iSrcBit = 31 - iSrc % 32;
	unsigned int	iDestBit = 31 - iDest % 32;

	//  ���� �Ѿߵ� ��Ʈ�� 1�� �ٲ��ְ�(ON : true) �������� �� 0�̵ȴ�
	unsigned int	iSrcAdd = 1 << iSrcBit;
	unsigned int	iDestAdd = 1 << iDestBit;

	// iSrc��Ʈ iDestIdx ��  iDestAdd�� or�����Ѵ�. : ������ ���ϴ� ��Ʈ�� ON�Ѵ�.
	m_pPrevCollisionAdj[iSrc][iDestIdx] |= iDestAdd;
	// iDest��Ʈ iSrcIdx ��  iSrcAdd�� or�����Ѵ�.  : ������ ���ϴ� ��Ʈ�� ON�Ѵ�
	m_pPrevCollisionAdj[iDest][iSrcIdx] |= iSrcAdd;
}

// ���� �����ӿ��� �浹�� �͵��� ������Ŀ��� �����. 
void CCollisionManager::DeletePrevCollisionAdj(unsigned int iSrc,
	unsigned int iDest)
{
	// 32 �� ������ �迭�� �ε����� ���� : ��� ° �׷����� �˼� ���� : �迭�� �ε����� �˾ƺ��� ��� 
	unsigned int	iSrcIdx = iSrc / 32;
	unsigned int	iDestIdx = iDest / 32;

	// 32 �� ���� �������� 32��° (����31)���� ���� �տ������� ���� �ϴ� �迭ó�� ����� ������ �� �ְ� �����. 
	// �ε����� �˾ƺ� �Ŀ� �� �ε������� ���� ��Ʈ�� �ɰ��� ���� ��� 
	unsigned int	iSrcBit = 31 - iSrc % 32;
	unsigned int	iDestBit = 31 - iDest % 32;

	//  ���� �Ѿߵ� ��Ʈ�� 1�� �ٲ��ְ�(ON : true) �������� �� 0�̵ȴ�
	unsigned int	iSrcAdd = 1 << iSrcBit;
	unsigned int	iDestAdd = 1 << iDestBit;

	// �ش� ��Ʈ�� �������� ���� true
	// ^ ( XOR : �ΰ��� ������ false, �ٸ��� true ) ������ �ؼ� ���� 
	if (m_pPrevCollisionAdj[iSrc][iDestIdx] & iDestAdd)
		m_pPrevCollisionAdj[iSrc][iDestIdx] ^= iDestAdd;

	// �ش� ��Ʈ�� �������� ���� true 
	// ^ ( XOR : �ΰ��� ������ false, �ٸ��� true ) ������ �ؼ� ����
	if (m_pPrevCollisionAdj[iDest][iSrcIdx] & iSrcAdd)
		m_pPrevCollisionAdj[iDest][iSrcIdx] ^= iSrcAdd;
}

// ���� �����ӿ��� �浹�� �͵��� ������Ŀ��� üũ�Ѵ�. 

bool CCollisionManager::CheckPrevCollisionAdj(unsigned int iSrc,
	unsigned int iDest)
{
	unsigned int	iDestIdx = iDest / 32;

	unsigned int	iDestBit = 31 - iDest % 32;

	unsigned int	iDestAdd = 1 << iDestBit;

	if (m_pPrevCollisionAdj[iSrc][iDestIdx] & iDestAdd)
		return true; // �ִ�

	return false; // ����
}

void CCollisionManager::AddWorldObjList(CObj* pObj)
{
	// UI�� ���ϸ� �����Ѵ�.   
	if (pObj->IsUI())
		return;

	pObj->AddRef();
	m_WorldObjList.push_back(pObj);
}

void CCollisionManager::AddCollider(CCollider* pCollider)
{
	// UI�� ���ϸ� �����Ѵ�.   
	if (pCollider->GetTag() == "MouseWorld" ||
		pCollider->GetTag() == "MouseUI")
		return;

	pCollider->AddRef();


	if (!pCollider->IsUI())
		m_ColliderList.push_back(pCollider);

	else
		m_UIColliderList.push_back(pCollider);
}

void CCollisionManager::DeleteCollider(CScene* pScene)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if ((*iter)->GetScene() == pScene)
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		++iter;
	}
}

void CCollisionManager::AddProfile(const string& strName)
{
	// ����ó�� 
	if (m_iProfileSize == m_iProfileCapacity)
		return;

	else if (FindProfile(strName))
		return;

	m_pProfile[m_iProfileSize].strName = strName;

	m_pProfile[m_iProfileSize].pChannelList = new CollisionChannel[m_iChannelCapacity];

	// ���������� ���� ä�������� ���� ���� 
	for (int i = 0; i < m_iChannelSize; ++i)
	{
		m_pProfile[m_iProfileSize].pChannelList[i] =
			m_pChannel[i];
	}
	// ���������� ���� ä�� ����� ���� 
	m_pProfile[m_iProfileSize].iChannelSize = m_iChannelSize;

	++m_iProfileSize; // ������ �� 
}

void CCollisionManager::AddChannel(const string& strName,
	COLLISION_CHANNEL_STATE eDefaultState) // ä���� �߰��Ѵ�. 
{
	// ���� ���� ä������ �� ������ ����. 
	if (m_iChannelSize == m_iChannelCapacity)
		return;
	// �ε����� -1�� �ݳ��ϸ� ����
	else if (GetChannelIndex(strName) != -1)
		return;
	// ä�� �̸�, ���� ���� 
	m_pChannel[m_iChannelSize].strName = strName;
	m_pChannel[m_iChannelSize].eState = eDefaultState;

	for (int i = 0; i < m_iProfileSize; ++i)
	{
		// ���������� ���� ä�θ���� ����  
		// ���������� ������ �� 
		m_pProfile[i].pChannelList[m_iChannelSize] = m_pChannel[m_iChannelSize];
		++m_pProfile[i].iChannelSize;
	}

	++m_iChannelSize;
}

// �������ϰ� ä���� ���踦 �����Ѵ�. 
void CCollisionManager::SetProfileChannelState(
	const string& strProfile, const string& strChannel,
	COLLISION_CHANNEL_STATE eState)
{
	for (int i = 0; i < m_iProfileSize; ++i)// �������� ������ ��ŭ
	{
		if (m_pProfile[i].strName == strProfile)// �������� Ž��
		{
			for (int j = 0; j < m_pProfile[i].iChannelSize; ++j)// ä���� ������ ��ŭ  
			{
				if (m_pProfile[i].pChannelList[j].strName == strChannel)// ä��Ž�� 
				{
					m_pProfile[i].pChannelList[j].eState = eState; // ä���� ���� ���� 
					return;
				}
			}
		}
	}
}

void CCollisionManager::SetProfileChannelStateAll(const string& strProfile, COLLISION_CHANNEL_STATE eState)
{
	for (int i = 0; i < m_iProfileSize; ++i)
	{
		if (m_pProfile[i].strName == strProfile)// �̸� Ž��
		{
			for (int j = 0; j < m_pProfile[i].iChannelSize; ++j)
			{
				m_pProfile[i].pChannelList[j].eState = eState;// ���¼��� Ž�� 
			}

			break;
		}
	}
}
// �������� Ž�� 
PCollisionProfile CCollisionManager::FindProfile(
	const string& strName)
{
	for (int i = 0; i < m_iProfileSize; ++i)
	{
		if (m_pProfile[i].strName == strName)
			return &m_pProfile[i];// ���������� �ּҸ� �ѱ��. 
	}

	return nullptr;
}
// ä���ε��� Get
int CCollisionManager::GetChannelIndex(const string& strName) const
{
	for (int i = 0; i < m_iChannelSize; ++i)
	{
		if (m_pChannel[i].strName == strName)
			return i;
	}

	return -1; // ���� 
}

bool CCollisionManager::Init()
{
	/*
	�浹 ��������
	�� 4096���� �浹ü�� ����ϰ� ������ �浹�ߴ�����
	�Ǵ��� �� �ִ� �ý����̴�.
	1��Ʈ�� 1���� �浹ü�� ���ؼ� ���������ӿ� �浹�ߴ�����
	�Ǵ��� �� �ִ� �ý����̴�.
	  0 1 2 3 4
	0 0 1 0 0 0
	1 1 0 0 1 0
	2 0 0 0 0 0
	3 0 1 0 0 0
	4 0 0 0 0 0
	*/
	// �����ڿ��� �ϴ� �� ���� �ʱ�ȭ �Ҷ� �ϴ� ���� �����ϴ�.
	m_iPrevCollisionCount = 4096; // ������ �迭�� ����(��)�� ������ ���� �����Ѵ�

	m_pPrevCollisionAdj = new unsigned int* [m_iPrevCollisionCount];

	unsigned int	iWidth = m_iPrevCollisionCount / 32;

	for (unsigned int i = 0; i < m_iPrevCollisionCount; ++i)
	{
		m_pPrevCollisionAdj[i] = new unsigned int[iWidth];
		memset(m_pPrevCollisionAdj[i], 0, sizeof(unsigned int) * iWidth);
	}

	m_iProfileSize = 0;
	m_iProfileCapacity = 64;
	m_pProfile = new CollisionProfile[m_iProfileCapacity];

	m_iChannelSize = 0;
	m_iChannelCapacity = 64;
	m_pChannel = new CollisionChannel[m_iChannelCapacity];

	// �� �浹 �������� - ä�� ���輳�� 

	// ���� ���� ������Ʈ 
	AddChannel("WorldStatic");
	AddChannel("WorldDynamic");
	AddChannel("Character");
	AddChannel("PlayerBullet");
	AddChannel("MonsterBullet");
	AddChannel("Mouse");
	AddChannel("Button", CCS_IGNORE);
	AddChannel("UI", CCS_IGNORE);

	// ������ ��ü �� ���
	AddProfile("BlockAll");
	SetProfileChannelStateAll("BlockAll", CCS_BLOCK);

	AddProfile("Player");
	SetProfileChannelState("Player", "Character", CCS_IGNORE);
	SetProfileChannelState("Player", "PlayerBullet", CCS_IGNORE);

	AddProfile("Monster");
	SetProfileChannelState("Monster", "Character", CCS_IGNORE);
	SetProfileChannelState("Monster", "MonsterBullet", CCS_IGNORE);

	// �߻�ü���� ���� �ε�ġ�� �ʰ� �ϳ� ������ 
	AddProfile("Bullet");
	// �߻�ü ���� �浹 ���ϰ� �����. 
	SetProfileChannelState("Bullet", "PlayerBullet", CCS_IGNORE);
	SetProfileChannelState("Bullet", "MonsterBullet", CCS_IGNORE);

	AddProfile("UI");
	SetProfileChannelState("UI", "Mouse", CCS_BLOCK);
	SetProfileChannelState("UI", "UI", CCS_BLOCK);

	AddProfile("Button");
	SetProfileChannelState("Button", "Mouse", CCS_BLOCK);

	AddProfile("Mouse");
	SetProfileChannelState("Mouse", "Mouse", CCS_IGNORE);
	SetProfileChannelState("Mouse", "Button", CCS_BLOCK);

	m_SerialNumber.SetMin(2); /*
									�� ��� �Ϲ� �浹ü�� �ø���ѹ��� 2������ �������� �����.
									����� �Ѿ�� ���콺 �浹ü�� �������� �ʴ´� .
									���콺 �浹ü�� �ø���ѹ��� 2������ ���۵ǵ��� �Ѵ�.
									Collision���� Ư���ϰ� ó���Ѵ�.
							  */
	return true;
}

// ���� �浹 ó���۾� 
void CCollisionManager::Collision(float fTime)
{
	// �ݵ�� �����ؼ� �浹 ó�� �ؾ��ϴ� ����� ����� �����ش�.
	m_SectionColliderList.clear();

	// ��ü �浹ü�� �ݺ��ϸ� �浹ó���� �ؾ��ϴ�
	// (���� ȭ�鿡 �������� �κе�) �浹ü����
	// ã�Ƽ� ������ ��Ͽ� �߰��ϰ� �������� ��
	// �浹ü���� �ɷ�����.
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		// ȭ�鿡 �������� �κ������� �Ǵ��Ѵ�.
		// ������ �׳� ��� �߰��ϰ� ���߿� ī�޶�
		// �ϰ� �� �Ŀ� ȭ�鿡 �������� �κ���
		// �Ǵ��ϴ� �ڵ带 �ۼ��Ѵ�.


		m_SectionColliderList.push_back((*iter));

		++iter;
	}

	// UI�� ���ؼ��� ó���Ѵ�.
	m_SectionUIColliderList.clear();

	iterEnd = m_UIColliderList.end();

	for (iter = m_UIColliderList.begin(); iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			SAFE_RELEASE((*iter));
			iter = m_UIColliderList.erase(iter);
			iterEnd = m_UIColliderList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		m_SectionUIColliderList.push_back(*iter);

		++iter;
	}

	// �� �� ������ �����´�. 
	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	// �� �������� ������ �����´�. 
	CStage* pStage = pScene->GetStage();

	// �� Ÿ�ϰ����� �� ������ �� �������� �����Ѵ�. 
	if (pStage && pStage->IsTile())
	{
		list<CObj*>::iterator	iter1;
		list<CObj*>::iterator	iter1End = m_WorldObjList.end();

		for (iter1 = m_WorldObjList.begin(); iter1 != iter1End;)
		{
			// ����ó�� 
			if (!(*iter1)->IsActive())
			{
				SAFE_RELEASE((*iter1));
				iter1 = m_WorldObjList.erase(iter1);
				iter1End = m_WorldObjList.end();
				continue;
			}

			else if (!(*iter1)->IsEnable())
			{
				++iter1;
				continue;
			}

			// Ÿ���� ��ǥ ����
			RectInfo	tTileRC;
			Position	tPos, tPrevPos;
			tPos = (*iter1)->GetPos();
			tPrevPos = (*iter1)->GetPrevPos();
			Position	tMove = tPos - tPrevPos;
			Position	tPivot = (*iter1)->GetPivot();
			Size tSize = (*iter1)->GetTileColSize(); // ������ Ÿ�ϻ����� ���� 

			// ������ �ʱ�ȭ �Ѵ�. 
			int	iDir = 0;

			// ������Ʈ�� �̵���ȭ�� -���, �������� �� ���
			if (tMove.x < 0)
				iDir = -1;

			// ������Ʈ�� �̵���ȭ�� +���, ���������� �� ��� 
			else if (tMove.x > 0)
				iDir = 1;

			// ����� 0���� �ʱ�ȭ �Ѵ�.
			float	fResult = 0.f;

			// ��������->���� üũ�Ѵ�. ��/�� : ��� ��ǥ, Ÿ�� ����, �ش� ������Ʈ, �̵����� 
			if (pStage->CheckWallW(fResult, tTileRC, *iter1,
				iDir))
			{
				// �̵������� �������� �̵��Ҷ� Ÿ���� ������ ����
				// �̵� ���� �ȿ� �����ϴ��� �Ǵ��Ͽ� ������ ��쿡��
				// �о��ش�.
				if (iDir == -1)
				{
					// �����浹ü��ġ �� ���� �浹ü ��ġ�� ���Ѵ�. 
					Position	tPrev = tPrevPos - tPivot * tSize;
					Position	tNext = tPos - tPivot * tSize;

					if (tNext.x < tTileRC.r && tTileRC.r < tPrev.x)// �浹�� ��� 
					{
						// ���� ��ġ + pStage���� CheckWallW() �� ��� ��ǥ�� ���� ��ġ�� �̵��ߴ�. 
						(*iter1)->UpdatePos((*iter1)->GetPos() + Position(fResult, 0.f));
					}
				}

				// �̵������� ������
				else if (iDir == 1)
				{
					// �����浹ü��ġ �� ���� �浹ü ��ġ�� ���Ѵ�. 
					Position	tPrev = tPrevPos + (Position(1.f, 1.f) - tPivot) * tSize;
					Position	tNext = tPos + (Position(1.f, 1.f) - tPivot) * tSize;

					// �浹�� ��� 
					if (tPrev.x < tTileRC.l && tTileRC.l < tNext.x)
					{
						(*iter1)->UpdatePos((*iter1)->GetPos() + Position(fResult, 0.f));
					}
				}
			}

			// ������ �ʱ�ȭ ���ش�. 
			iDir = 0;

			// �̵����� Y�� 0���� ������ �̵������� -1 
			if (tMove.y < 0)
				iDir = -1;

			// �� �ݴ� 
			else if (tMove.y > 0)
				iDir = 1;

			// ������������ ���� üũ�Ѵ�. ��/�Ʒ� : ��� ��ǥ, Ÿ�� ����, �ش� ������Ʈ, �̵����� 
			if (pStage->CheckWallH(fResult, tTileRC, *iter1,
				iDir))
			{

				// ���� ������ ���� ��Ȳ
				if (iDir == -1)
				{
					// ���� �浹ü�� ��ġ�� ���Ѵ�. 
					// ���� �浹ü�� ��ġ�� ���Ѵ�. 
					Position	tPrev = tPrevPos - tPivot * tSize;
					Position	tNext = tPos - tPivot * tSize;

					// �浹�Ǿ���. 
					if (tNext.y < tTileRC.b && tTileRC.b < tPrev.y)
					{
						// �� ��ġ�� ��� ��ǥ�� ����Ͽ� �浹 �� ��ġ�� ����. 
						(*iter1)->UpdatePos((*iter1)->GetPos() + Position(0.f, fResult));
					}
				}

				// �Ʒ��� ������ �������� ��Ȳ 
				else if (iDir == 1)
				{
					// ���� �浹ü�� ��ġ�� ���Ѵ�. 
					// ���� �浹ü�� ��ġ�� ���Ѵ�. 
					Position	tPrev = tPrevPos + (Position(1.f, 1.f) - tPivot) * tSize;
					Position	tNext = tPos + (Position(1.f, 1.f) - tPivot) * tSize;

					// �浹�Ǿ���. 
					if (tPrev.y < tTileRC.t && tTileRC.t < tNext.y)
					{
						// �� ��ġ�� ��� ��ǥ�� ����Ͽ� �浹 �� ��ġ�� ����. 
						(*iter1)->UpdatePos((*iter1)->GetPos() + Position(0.f, fResult));
						(*iter1)->ClearGravity();
						/*
								�̰��� ���� ������ �߷°���� ���ӵǾ� �Ʒ��� ��� ��������.
								�߷��� Ŭ�����Ѵ�.
								m_fGravityTime = 0.f;
								m_fGravityForce = 0.f;
						*/
					}
				}
			}

			++iter1;
		}
	}

	/*
		���콺 �浹ü�� Ư���ϰ� ����ϴ� ���� :
		UI ���� ĳ���͵��� �����ִ� ���¿��� �����ִ� �κ��� �������� ���� �ִ� �༮�� �����ϸ� �ٸ� ������Ʈ�� �����ϸ� �ȵȴ�
		����� �Ѿ�� ���콺�浹ü�� �������� �ʴ´� .
		��� �Ϲ� �浹ü�� �ø���ѹ��� 2�� ���� �������� �����.
	*/

	// �浹ó�� �ؾߵ� �浹ü�鿡 ���ؼ� �浹�۾���
	// �����Ѵ�.
	// ���콺�� UI�� �ϳ��� �浹�Ǹ� ���̻� �ٸ� ��ü��
	// �浹�� ���ص� �ȴ�.
	if (!CollisionMouseUI(fTime))
	{
		if (!CollisionMouseWorld(fTime))
		{
			// ������ ó���Ѵ�. 
		}
	}
	// �浹ó�� �ؾߵ� �浹ü�鿡 ���ؼ� �浹�۾���
	// �����Ѵ�.
	CollisionUI(fTime);
	CollisionWorld(fTime);
}

// UI���� ���콺 �浹ó�� 
bool CCollisionManager::CollisionMouseUI(float fTime)
{
	// ������� ��� �翬�� �浹 ���� 
	if (m_SectionUIColliderList.empty())
		return false;

	// UI���콺 �浹ü�� pSrc�� ���� 
	CCollider* pSrc = GET_SINGLE(CInput)->GetUICollider();
	// pSrc �������� �����´�. 
	PCollisionProfile	pSrcProfile = pSrc->GetProfile();
	// pSrc ä���� �����´�. 
	int	iSrcChannel = pSrc->GetChannelIndex();

	/*
		���콺 vs UI
		iterEnd���� �� ������.
	*/
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_SectionUIColliderList.end();

	for (iter = m_SectionUIColliderList.begin(); iter != iterEnd; ++iter)
	{
		// m_SectionUIColliderList ���⼭ �ϳ� pDest ���� 
		CCollider* pDest = *iter;

		// pDest ��������, ä�� ����  
		PCollisionProfile	pDestProfile = pDest->GetProfile();
		int	iDestChannel = pDest->GetChannelIndex();

		// �浹�Ǵ� ���� ���콺���� ���� �浹 ����� ������ �ְ��Ѵ�. 
		// m_pMousePrevCollision
		if (pSrcProfile->pChannelList[iDestChannel].eState != CCS_IGNORE &&
			pDestProfile->pChannelList[iSrcChannel].eState != CCS_IGNORE)
		{
			// �浹 �Ǿ��� ���
			if (pSrc->Collision(pDest))
			{
				// ���� �����ӿ� �浹�� �Ǿ����� �Ǵ��Ѵ�.
				if (!CheckPrevCollisionAdj(pSrc->GetSerialNumber(),
					pDest->GetSerialNumber()))
				{
					if (!m_pMousePrevCollision)
						m_pMousePrevCollision = pDest;  // null�̸� ������ �浹�ǰ� �ִ� ����� ����.

					else if (m_pMousePrevCollision != pDest)
					{
						// �� �浹ü�� ���� �浹���¿��� �����Ѵ�.
						DeletePrevCollisionAdj(pSrc->GetSerialNumber(),
							m_pMousePrevCollision->GetSerialNumber());

						pSrc->DeletePrevCollider(m_pMousePrevCollision);
						m_pMousePrevCollision->DeletePrevCollider(pSrc);

						pSrc->Call(CS_RELEASE, m_pMousePrevCollision, fTime);
						m_pMousePrevCollision->Call(CS_RELEASE, pSrc, fTime);

						m_pMousePrevCollision = pDest;// pDest�� m_pMousePrevCollision�� �����Ѵ�. 
					}

					// �� �浹ü�� ���� �浹���·� ����Ѵ�.
					AddPrevCollisionAdj(pSrc->GetSerialNumber(),
						pDest->GetSerialNumber());

					pSrc->AddPrevCollider(pDest);
					pDest->AddPrevCollider(pSrc);

					pSrc->Call(CS_BLOCK, pDest, fTime);
					pDest->Call(CS_BLOCK, pSrc, fTime);
				}

				else
				{
					pSrc->Call(CS_OVERLAP, pDest, fTime);
					pDest->Call(CS_OVERLAP, pSrc, fTime);
				}

				return true; // �浹�Ǿ���.
			}
		}
	}

	if (m_pMousePrevCollision)
	{
		// �� �浹ü�� ���� �浹���¿��� �����Ѵ�.
		DeletePrevCollisionAdj(pSrc->GetSerialNumber(),
			m_pMousePrevCollision->GetSerialNumber());

		pSrc->DeletePrevCollider(m_pMousePrevCollision);
		m_pMousePrevCollision->DeletePrevCollider(pSrc);

		pSrc->Call(CS_RELEASE, m_pMousePrevCollision, fTime);
		m_pMousePrevCollision->Call(CS_RELEASE, pSrc, fTime);

		m_pMousePrevCollision = nullptr;
	}

	return false;
}

// ���忡�� ���콺 �浹ó�� �ڡ�
bool CCollisionManager::CollisionMouseWorld(float fTime)
{
	return false;
}

// ���忡�� �浹���� 
void CCollisionManager::CollisionWorld(float fTime)
{
	if (m_SectionColliderList.size() >= 2)// ��Ͽ� �ݵ�� �� ���� �־�� �Ѵ�. 
	{
		list<CCollider*>::iterator	iter;
		list<CCollider*>::iterator	iterEnd = m_SectionColliderList.end();
		--iterEnd;

		for (iter = m_SectionColliderList.begin(); iter != iterEnd; ++iter)
		{
			list<CCollider*>::iterator	iter1 = iter;
			++iter1;// iter1 : iter �ٱ��ʳ���� ���� ���� ���� �ǰ� ���� 
			list<CCollider*>::iterator	iter1End = m_SectionColliderList.end();
			// iter�� �����ϴ� ���� ����. ����� ��� ������ �̸� �������� �޾Ƴ��� ó������ 
			CCollider* pSrc = *iter;

			PCollisionProfile	pSrcProfile = pSrc->GetProfile();
			int	iSrcChannel = pSrc->GetChannelIndex();

			for (; iter1 != iter1End; ++iter1)
			{
				// iter�� �����ϴ� ���� ����. ����� ��� ������ �̸� �������� �޾Ƴ��� ó������ 
				CCollider* pDest = *iter1;

				PCollisionProfile	pDestProfile = pDest->GetProfile();
				int	iDestChannel = pDest->GetChannelIndex();

				// �Ѵ� ���ð� �ƴ� ��� �浹�� �ȴ�. 
				if (pSrcProfile->pChannelList[iDestChannel].eState != CCS_IGNORE &&
					pDestProfile->pChannelList[iSrcChannel].eState != CCS_IGNORE)
				{
					// �浹 �Ǿ��� ���
					if (pSrc->Collision(pDest))
					{
						// ���� �����ӿ� �浹�� �Ǿ����� �Ǵ��Ѵ�.
						if (!CheckPrevCollisionAdj(pSrc->GetSerialNumber(),
							pDest->GetSerialNumber()))
						{
							// �� �浹ü�� ���� �浹���·� ����Ѵ�.
							AddPrevCollisionAdj(pSrc->GetSerialNumber(),
								pDest->GetSerialNumber());

							pSrc->AddPrevCollider(pDest);
							pDest->AddPrevCollider(pSrc);

							// �ε����� �� 

							pSrc->Call(CS_BLOCK, pDest, fTime);
							pDest->Call(CS_BLOCK, pSrc, fTime);
						}

						else
						{
							// �浹�ǰ� �ִ� ����
							pSrc->Call(CS_OVERLAP, pDest, fTime);
							pDest->Call(CS_OVERLAP, pSrc, fTime);
						}
					}

					// ���� �����ӿ��� �浹�� �ȵǰ� ���������ӿ�����
					// �浹���¶�� �浹�Ǵ� �浹ü�� ��������
					// ���¶�� ���̴�.
					else if (CheckPrevCollisionAdj(pSrc->GetSerialNumber(),
						pDest->GetSerialNumber()))
					{
						// �� �浹ü�� ���� �浹���¿��� �����Ѵ�.
						DeletePrevCollisionAdj(pSrc->GetSerialNumber(),
							pDest->GetSerialNumber());

						pSrc->DeletePrevCollider(pDest);
						pDest->DeletePrevCollider(pSrc);

						// �浹�Ǵٰ� �������� ���� 

						pSrc->Call(CS_RELEASE, pDest, fTime);
						pDest->Call(CS_RELEASE, pSrc, fTime);
					}
				}
			}
		}
	}
}

void CCollisionManager::CollisionUI(float fTime)
{
	if (m_SectionUIColliderList.size() >= 2)// ��Ͽ� �ݵ�� �� ���� �־�� �Ѵ�. 
	{
		list<CCollider*>::iterator	iter;
		list<CCollider*>::iterator	iterEnd = m_SectionUIColliderList.end();
		--iterEnd; // ���� ������� �������� 

		for (iter = m_SectionUIColliderList.begin(); iter != iterEnd; ++iter)
		{
			list<CCollider*>::iterator	iter1 = iter;
			++iter1;// iter1 : iter �ٱ��ʳ���� ���� ���� ���� �ǰ� ���� 
			list<CCollider*>::iterator	iter1End = m_SectionUIColliderList.end();
			// iter�� �����ϴ� ���� ����. ����� ��� ������ �̸� �������� �޾Ƴ��� ó������ 
			CCollider* pSrc = *iter;

			PCollisionProfile	pSrcProfile = pSrc->GetProfile();
			int	iSrcChannel = pSrc->GetChannelIndex();

			// (�ʱⰪ�� ������ ������ ����)
			for (; iter1 != iter1End; ++iter1)
			{
				// iter�� �����ϴ� ���� ����. ����� ��� ������ �̸� �������� �޾Ƴ��� ó������ 
				CCollider* pDest = *iter1;

				PCollisionProfile	pDestProfile = pDest->GetProfile();
				int	iDestChannel = pDest->GetChannelIndex();

				if (pSrcProfile->pChannelList[iDestChannel].eState != CCS_IGNORE &&
					pDestProfile->pChannelList[iSrcChannel].eState != CCS_IGNORE)
				{
					// �浹 �Ǿ��� ���
					if (pSrc->Collision(pDest))
					{
						// ���� �����ӿ� �浹�� �Ǿ����� �Ǵ��Ѵ�.
						if (!CheckPrevCollisionAdj(pSrc->GetSerialNumber(),
							pDest->GetSerialNumber()))
						{
							// �� �浹ü�� ���� �浹���·� ����Ѵ�.
							AddPrevCollisionAdj(pSrc->GetSerialNumber(),
								pDest->GetSerialNumber());

							pSrc->AddPrevCollider(pDest);
							pDest->AddPrevCollider(pSrc);

							// �ε�������
							pSrc->Call(CS_BLOCK, pDest, fTime);
							pDest->Call(CS_BLOCK, pSrc, fTime);
						}

						else
						{
							pSrc->Call(CS_OVERLAP, pDest, fTime);
							pDest->Call(CS_OVERLAP, pSrc, fTime);
						}
					}

					// ���� �����ӿ��� �浹�� �ȵǰ� ���������ӿ�����
					// �浹���¶�� �浹�Ǵ� �浹ü�� ��������
					// ���¶�� ���̴�.
					else if (CheckPrevCollisionAdj(pSrc->GetSerialNumber(),
						pDest->GetSerialNumber()))
					{
						// �� �浹ü�� ���� �浹���¿��� �����Ѵ�.
						DeletePrevCollisionAdj(pSrc->GetSerialNumber(),
							pDest->GetSerialNumber());

						pSrc->DeletePrevCollider(pDest);
						pDest->DeletePrevCollider(pSrc);

						// �浹�Ǵٰ� �������� ���� 
						pSrc->Call(CS_RELEASE, pDest, fTime);
						pDest->Call(CS_RELEASE, pSrc, fTime);
					}
				}
			}
		}
	}
}
