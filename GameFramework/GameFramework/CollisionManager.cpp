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
	// 32 로 나누면 배열의 인덱스가 나옴 : 몇번 째 그룹인지 알수 있음 : 배열의 인덱스를 알아보는 방법 
	unsigned int	iSrcIdx = iSrc / 32;// 이진수의 표현 단위 int는 최대 32자리 
	unsigned int	iDestIdx = iDest / 32;

	// 32 로 나눈 나머지를 32번째 (숫자31)에서 빼면 앞에서부터 셈을 하는 배열처럼 계산이 용이할 수 있게 만든다.
	// 인덱스를 알아본 후에 그 인덱스안의 값을 비트로 쪼개서 쓰는 방법
	unsigned int	iSrcBit = 31 - iSrc % 32;
	unsigned int	iDestBit = 31 - iDest % 32;

	//  내가 켜야될 비트만 1로 바꿔주고(ON : true) 나머지는 다 0이된다
	unsigned int	iSrcAdd = 1 << iSrcBit;
	unsigned int	iDestAdd = 1 << iDestBit;

	// iSrc비트 iDestIdx 를  iDestAdd와 or연산한다. : 무조건 원하는 비트를 ON한다.
	m_pPrevCollisionAdj[iSrc][iDestIdx] |= iDestAdd;
	// iDest비트 iSrcIdx 를  iSrcAdd와 or연산한다.  : 무조건 원하는 비트를 ON한다
	m_pPrevCollisionAdj[iDest][iSrcIdx] |= iSrcAdd;
}

// 이전 프레임에서 충돌한 것들을 인접행렬에서 지운다. 
void CCollisionManager::DeletePrevCollisionAdj(unsigned int iSrc,
	unsigned int iDest)
{
	// 32 로 나누면 배열의 인덱스가 나옴 : 몇번 째 그룹인지 알수 있음 : 배열의 인덱스를 알아보는 방법 
	unsigned int	iSrcIdx = iSrc / 32;
	unsigned int	iDestIdx = iDest / 32;

	// 32 로 나눈 나머지를 32번째 (숫자31)에서 빼면 앞에서부터 셈을 하는 배열처럼 계산이 용이할 수 있게 만든다. 
	// 인덱스를 알아본 후에 그 인덱스안의 값을 비트로 쪼개서 쓰는 방법 
	unsigned int	iSrcBit = 31 - iSrc % 32;
	unsigned int	iDestBit = 31 - iDest % 32;

	//  내가 켜야될 비트만 1로 바꿔주고(ON : true) 나머지는 다 0이된다
	unsigned int	iSrcAdd = 1 << iSrcBit;
	unsigned int	iDestAdd = 1 << iDestBit;

	// 해당 비트가 켜져있을 때만 true
	// ^ ( XOR : 두개가 같으면 false, 다르면 true ) 연산을 해서 제거 
	if (m_pPrevCollisionAdj[iSrc][iDestIdx] & iDestAdd)
		m_pPrevCollisionAdj[iSrc][iDestIdx] ^= iDestAdd;

	// 해당 비트가 켜져있을 때만 true 
	// ^ ( XOR : 두개가 같으면 false, 다르면 true ) 연산을 해서 제거
	if (m_pPrevCollisionAdj[iDest][iSrcIdx] & iSrcAdd)
		m_pPrevCollisionAdj[iDest][iSrcIdx] ^= iSrcAdd;
}

// 이전 프레임에서 충돌한 것들을 인접행렬에서 체크한다. 

bool CCollisionManager::CheckPrevCollisionAdj(unsigned int iSrc,
	unsigned int iDest)
{
	unsigned int	iDestIdx = iDest / 32;

	unsigned int	iDestBit = 31 - iDest % 32;

	unsigned int	iDestAdd = 1 << iDestBit;

	if (m_pPrevCollisionAdj[iSrc][iDestIdx] & iDestAdd)
		return true; // 있다

	return false; // 없다
}

void CCollisionManager::AddWorldObjList(CObj* pObj)
{
	// UI에 속하면 제외한다.   
	if (pObj->IsUI())
		return;

	pObj->AddRef();
	m_WorldObjList.push_back(pObj);
}

void CCollisionManager::AddCollider(CCollider* pCollider)
{
	// UI에 속하면 제외한다.   
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
	// 예외처리 
	if (m_iProfileSize == m_iProfileCapacity)
		return;

	else if (FindProfile(strName))
		return;

	m_pProfile[m_iProfileSize].strName = strName;

	m_pProfile[m_iProfileSize].pChannelList = new CollisionChannel[m_iChannelCapacity];

	// 프로파일이 가진 채널정보에 전부 복사 
	for (int i = 0; i < m_iChannelSize; ++i)
	{
		m_pProfile[m_iProfileSize].pChannelList[i] =
			m_pChannel[i];
	}
	// 프로파일이 가진 채널 사이즈도 복사 
	m_pProfile[m_iProfileSize].iChannelSize = m_iChannelSize;

	++m_iProfileSize; // 사이즈 업 
}

void CCollisionManager::AddChannel(const string& strName,
	COLLISION_CHANNEL_STATE eDefaultState) // 채널을 추가한다. 
{
	// 전부 값이 채워지면 들어갈 공간이 없다. 
	if (m_iChannelSize == m_iChannelCapacity)
		return;
	// 인덱스가 -1을 반납하면 실패
	else if (GetChannelIndex(strName) != -1)
		return;
	// 채널 이름, 상태 설정 
	m_pChannel[m_iChannelSize].strName = strName;
	m_pChannel[m_iChannelSize].eState = eDefaultState;

	for (int i = 0; i < m_iProfileSize; ++i)
	{
		// 프로파일이 가진 채널목록을 복사  
		// 프로파일의 사이즈 업 
		m_pProfile[i].pChannelList[m_iChannelSize] = m_pChannel[m_iChannelSize];
		++m_pProfile[i].iChannelSize;
	}

	++m_iChannelSize;
}

// 프로파일과 채널의 관계를 설정한다. 
void CCollisionManager::SetProfileChannelState(
	const string& strProfile, const string& strChannel,
	COLLISION_CHANNEL_STATE eState)
{
	for (int i = 0; i < m_iProfileSize; ++i)// 프로파일 사이즈 만큼
	{
		if (m_pProfile[i].strName == strProfile)// 프로파일 탐색
		{
			for (int j = 0; j < m_pProfile[i].iChannelSize; ++j)// 채널의 사이즈 만큼  
			{
				if (m_pProfile[i].pChannelList[j].strName == strChannel)// 채널탐색 
				{
					m_pProfile[i].pChannelList[j].eState = eState; // 채널의 상태 지정 
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
		if (m_pProfile[i].strName == strProfile)// 이름 탐색
		{
			for (int j = 0; j < m_pProfile[i].iChannelSize; ++j)
			{
				m_pProfile[i].pChannelList[j].eState = eState;// 상태설정 탐색 
			}

			break;
		}
	}
}
// 프로파일 탐색 
PCollisionProfile CCollisionManager::FindProfile(
	const string& strName)
{
	for (int i = 0; i < m_iProfileSize; ++i)
	{
		if (m_pProfile[i].strName == strName)
			return &m_pProfile[i];// 프로파일의 주소를 넘긴다. 
	}

	return nullptr;
}
// 채널인덱스 Get
int CCollisionManager::GetChannelIndex(const string& strName) const
{
	for (int i = 0; i < m_iChannelSize; ++i)
	{
		if (m_pChannel[i].strName == strName)
			return i;
	}

	return -1; // 실패 
}

bool CCollisionManager::Init()
{
	/*
	충돌 인접정보
	총 4096개의 충돌체를 등록하고 이전에 충돌했는지를
	판단할 수 있는 시스템이다.
	1비트당 1개의 충돌체에 대해서 이전프레임에 충돌했는지를
	판단할 수 있는 시스템이다.
	  0 1 2 3 4
	0 0 1 0 0 0
	1 1 0 0 1 0
	2 0 0 0 0 0
	3 0 1 0 0 0
	4 0 0 0 0 0
	*/
	// 생성자에서 하는 것 보다 초기화 할때 하는 것이 유리하다.
	m_iPrevCollisionCount = 4096; // 이차원 배열의 세로(줄)의 갯수를 먼저 제공한다

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

	// ★ 충돌 프로파일 - 채널 관계설정 

	// 월드 상의 오브젝트 
	AddChannel("WorldStatic");
	AddChannel("WorldDynamic");
	AddChannel("Character");
	AddChannel("PlayerBullet");
	AddChannel("MonsterBullet");
	AddChannel("Mouse");
	AddChannel("Button", CCS_IGNORE);
	AddChannel("UI", CCS_IGNORE);

	// 무조건 전체 다 블록
	AddProfile("BlockAll");
	SetProfileChannelStateAll("BlockAll", CCS_BLOCK);

	AddProfile("Player");
	SetProfileChannelState("Player", "Character", CCS_IGNORE);
	SetProfileChannelState("Player", "PlayerBullet", CCS_IGNORE);

	AddProfile("Monster");
	SetProfileChannelState("Monster", "Character", CCS_IGNORE);
	SetProfileChannelState("Monster", "MonsterBullet", CCS_IGNORE);

	// 발사체들이 서로 부딪치지 않게 하나 만들자 
	AddProfile("Bullet");
	// 발사체 끼리 충돌 안하게 만든다. 
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
									＠ 모든 일반 충돌체는 시리얼넘버를 2번부터 가져가게 만든다.
									장면이 넘어가도 마우스 충돌체는 지워지지 않는다 .
									마우스 충돌체는 시리얼넘버가 2번부터 시작되도록 한다.
									Collision에서 특별하게 처리한다.
							  */
	return true;
}

// 실제 충돌 처리작업 
void CCollisionManager::Collision(float fTime)
{
	// 반드시 조사해서 충돌 처리 해야하는 목록을 깔끔히 지워준다.
	m_SectionColliderList.clear();

	// 전체 충돌체를 반복하며 충돌처리를 해야하는
	// (현재 화면에 보여지는 부분들) 충돌체들을
	// 찾아서 별도의 목록에 추가하고 지워져야 할
	// 충돌체들을 걸러낸다.
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

		// 화면에 보여지는 부분인지를 판단한다.
		// 지금은 그냥 모두 추가하고 나중에 카메라
		// 하고 난 후에 화면에 보여지는 부분을
		// 판단하는 코드를 작성한다.


		m_SectionColliderList.push_back((*iter));

		++iter;
	}

	// UI에 대해서도 처리한다.
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

	// ★ 씬 정보를 가져온다. 
	CScene* pScene = GET_SINGLE(CSceneManager)->GetScene();

	// ★ 스테이지 정보를 가져온다. 
	CStage* pStage = pScene->GetStage();

	// ★ 타일개념을 쓸 것인지 안 쓸것인지 결정한다. 
	if (pStage && pStage->IsTile())
	{
		list<CObj*>::iterator	iter1;
		list<CObj*>::iterator	iter1End = m_WorldObjList.end();

		for (iter1 = m_WorldObjList.begin(); iter1 != iter1End;)
		{
			// 예외처리 
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

			// 타일의 좌표 정보
			RectInfo	tTileRC;
			Position	tPos, tPrevPos;
			tPos = (*iter1)->GetPos();
			tPrevPos = (*iter1)->GetPrevPos();
			Position	tMove = tPos - tPrevPos;
			Position	tPivot = (*iter1)->GetPivot();
			Size tSize = (*iter1)->GetTileColSize(); // 설정한 타일사이즈 주의 

			// 방향을 초기화 한다. 
			int	iDir = 0;

			// 오브젝트의 이동변화가 -라면, 왼쪽으로 간 경우
			if (tMove.x < 0)
				iDir = -1;

			// 오브젝트의 이동변화가 +라면, 오른쪽으로 간 경우 
			else if (tMove.x > 0)
				iDir = 1;

			// 결과를 0으로 초기화 한다.
			float	fResult = 0.f;

			// 스테이지->벽을 체크한다. 좌/우 : 결과 좌표, 타일 정보, 해당 오브젝트, 이동방향 
			if (pStage->CheckWallW(fResult, tTileRC, *iter1,
				iDir))
			{
				// 이동방향이 왼쪽으로 이동할때 타일의 오른쪽 면이
				// 이동 범위 안에 존재하는지 판단하여 존재할 경우에만
				// 밀어준다.
				if (iDir == -1)
				{
					// 이전충돌체위치 와 다음 충돌체 위치를 구한다. 
					Position	tPrev = tPrevPos - tPivot * tSize;
					Position	tNext = tPos - tPivot * tSize;

					if (tNext.x < tTileRC.r && tTileRC.r < tPrev.x)// 충돌한 경우 
					{
						// 현재 위치 + pStage에서 CheckWallW() 의 결과 좌표를 더한 위치로 이동했다. 
						(*iter1)->UpdatePos((*iter1)->GetPos() + Position(fResult, 0.f));
					}
				}

				// 이동방향이 오른쪽
				else if (iDir == 1)
				{
					// 이전충돌체위치 와 다음 충돌체 위치를 구한다. 
					Position	tPrev = tPrevPos + (Position(1.f, 1.f) - tPivot) * tSize;
					Position	tNext = tPos + (Position(1.f, 1.f) - tPivot) * tSize;

					// 충돌한 경우 
					if (tPrev.x < tTileRC.l && tTileRC.l < tNext.x)
					{
						(*iter1)->UpdatePos((*iter1)->GetPos() + Position(fResult, 0.f));
					}
				}
			}

			// 방향을 초기화 해준다. 
			iDir = 0;

			// 이동량의 Y가 0보다 작으면 이동방향이 -1 
			if (tMove.y < 0)
				iDir = -1;

			// 그 반대 
			else if (tMove.y > 0)
				iDir = 1;

			// 스테이지에서 벽을 체크한다. 위/아래 : 결과 좌표, 타일 정보, 해당 오브젝트, 이동방향 
			if (pStage->CheckWallH(fResult, tTileRC, *iter1,
				iDir))
			{

				// 위의 벽으로 가는 상황
				if (iDir == -1)
				{
					// 이전 충돌체의 위치를 구한다. 
					// 현재 충돌체의 위치를 구한다. 
					Position	tPrev = tPrevPos - tPivot * tSize;
					Position	tNext = tPos - tPivot * tSize;

					// 충돌되었다. 
					if (tNext.y < tTileRC.b && tTileRC.b < tPrev.y)
					{
						// 현 위치와 결과 좌표를 계산하여 충돌 후 위치로 간다. 
						(*iter1)->UpdatePos((*iter1)->GetPos() + Position(0.f, fResult));
					}
				}

				// 아래의 벽으로 내려가는 상황 
				else if (iDir == 1)
				{
					// 이전 충돌체의 위치를 구한다. 
					// 현재 충돌체의 위치를 구한다. 
					Position	tPrev = tPrevPos + (Position(1.f, 1.f) - tPivot) * tSize;
					Position	tNext = tPos + (Position(1.f, 1.f) - tPivot) * tSize;

					// 충돌되었다. 
					if (tPrev.y < tTileRC.t && tTileRC.t < tNext.y)
					{
						// 현 위치와 결과 좌표를 계산하여 충돌 후 위치로 간다. 
						(*iter1)->UpdatePos((*iter1)->GetPos() + Position(0.f, fResult));
						(*iter1)->ClearGravity();
						/*
								이것을 하지 않으면 중력계산이 지속되어 아래로 계속 내려간다.
								중력을 클리어한다.
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
		마우스 충돌체를 특별하게 취급하는 이유 :
		UI 월드 캐릭터들이 겹쳐있는 상태에서 겹쳐있는 부분을 눌럿을때 덮고 있는 녀석을 선택하면 다른 오브젝트를 선택하면 안된다
		장면이 넘어가도 마우스충돌체는 지워지지 않는다 .
		모든 일반 충돌체는 시리얼넘버를 2번 부터 가져가게 만든다.
	*/

	// 충돌처리 해야될 충돌체들에 대해서 충돌작업을
	// 시작한다.
	// 마우스와 UI중 하나가 충돌되면 더이상 다른 물체는
	// 충돌을 안해도 된다.
	if (!CollisionMouseUI(fTime))
	{
		if (!CollisionMouseWorld(fTime))
		{
			// 별도로 처리한다. 
		}
	}
	// 충돌처리 해야될 충돌체들에 대해서 충돌작업을
	// 시작한다.
	CollisionUI(fTime);
	CollisionWorld(fTime);
}

// UI에서 마우스 충돌처리 
bool CCollisionManager::CollisionMouseUI(float fTime)
{
	// 비어있을 경우 당연히 충돌 안함 
	if (m_SectionUIColliderList.empty())
		return false;

	// UI마우스 충돌체를 pSrc로 지정 
	CCollider* pSrc = GET_SINGLE(CInput)->GetUICollider();
	// pSrc 프로파일 가져온다. 
	PCollisionProfile	pSrcProfile = pSrc->GetProfile();
	// pSrc 채널을 가져온다. 
	int	iSrcChannel = pSrc->GetChannelIndex();

	/*
		마우스 vs UI
		iterEnd까지 다 돌린다.
	*/
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_SectionUIColliderList.end();

	for (iter = m_SectionUIColliderList.begin(); iter != iterEnd; ++iter)
	{
		// m_SectionUIColliderList 여기서 하나 pDest 지정 
		CCollider* pDest = *iter;

		// pDest 프로파일, 채널 지정  
		PCollisionProfile	pDestProfile = pDest->GetProfile();
		int	iDestChannel = pDest->GetChannelIndex();

		// 충돌되는 순간 마우스와의 이전 충돌 목록을 가지고 있게한다. 
		// m_pMousePrevCollision
		if (pSrcProfile->pChannelList[iDestChannel].eState != CCS_IGNORE &&
			pDestProfile->pChannelList[iSrcChannel].eState != CCS_IGNORE)
		{
			// 충돌 되었을 경우
			if (pSrc->Collision(pDest))
			{
				// 이전 프레임에 충돌이 되었는지 판단한다.
				if (!CheckPrevCollisionAdj(pSrc->GetSerialNumber(),
					pDest->GetSerialNumber()))
				{
					if (!m_pMousePrevCollision)
						m_pMousePrevCollision = pDest;  // null이면 이전에 충돌되고 있던 목록이 없다.

					else if (m_pMousePrevCollision != pDest)
					{
						// 두 충돌체를 서로 충돌상태에서 해제한다.
						DeletePrevCollisionAdj(pSrc->GetSerialNumber(),
							m_pMousePrevCollision->GetSerialNumber());

						pSrc->DeletePrevCollider(m_pMousePrevCollision);
						m_pMousePrevCollision->DeletePrevCollider(pSrc);

						pSrc->Call(CS_RELEASE, m_pMousePrevCollision, fTime);
						m_pMousePrevCollision->Call(CS_RELEASE, pSrc, fTime);

						m_pMousePrevCollision = pDest;// pDest로 m_pMousePrevCollision를 변경한다. 
					}

					// 두 충돌체를 서로 충돌상태로 등록한다.
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

				return true; // 충돌되었다.
			}
		}
	}

	if (m_pMousePrevCollision)
	{
		// 두 충돌체를 서로 충돌상태에서 해제한다.
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

// 월드에서 마우스 충돌처리 ★☆
bool CCollisionManager::CollisionMouseWorld(float fTime)
{
	return false;
}

// 월드에서 충돌로직 
void CCollisionManager::CollisionWorld(float fTime)
{
	if (m_SectionColliderList.size() >= 2)// 목록에 반드시 두 개가 있어야 한다. 
	{
		list<CCollider*>::iterator	iter;
		list<CCollider*>::iterator	iterEnd = m_SectionColliderList.end();
		--iterEnd;

		for (iter = m_SectionColliderList.begin(); iter != iterEnd; ++iter)
		{
			list<CCollider*>::iterator	iter1 = iter;
			++iter1;// iter1 : iter 바깥쪽노드의 다음 부터 시작 되게 만듬 
			list<CCollider*>::iterator	iter1End = m_SectionColliderList.end();
			// iter로 접근하는 것이 느림. 비용이 들기 때문에 미리 역참조로 받아놓고 처리하자 
			CCollider* pSrc = *iter;

			PCollisionProfile	pSrcProfile = pSrc->GetProfile();
			int	iSrcChannel = pSrc->GetChannelIndex();

			for (; iter1 != iter1End; ++iter1)
			{
				// iter로 접근하는 것이 느림. 비용이 들기 때문에 미리 역참조로 받아놓고 처리하자 
				CCollider* pDest = *iter1;

				PCollisionProfile	pDestProfile = pDest->GetProfile();
				int	iDestChannel = pDest->GetChannelIndex();

				// 둘다 무시가 아닐 경우 충돌이 된다. 
				if (pSrcProfile->pChannelList[iDestChannel].eState != CCS_IGNORE &&
					pDestProfile->pChannelList[iSrcChannel].eState != CCS_IGNORE)
				{
					// 충돌 되었을 경우
					if (pSrc->Collision(pDest))
					{
						// 이전 프레임에 충돌이 되었는지 판단한다.
						if (!CheckPrevCollisionAdj(pSrc->GetSerialNumber(),
							pDest->GetSerialNumber()))
						{
							// 두 충돌체를 서로 충돌상태로 등록한다.
							AddPrevCollisionAdj(pSrc->GetSerialNumber(),
								pDest->GetSerialNumber());

							pSrc->AddPrevCollider(pDest);
							pDest->AddPrevCollider(pSrc);

							// 부딪쳤을 떄 

							pSrc->Call(CS_BLOCK, pDest, fTime);
							pDest->Call(CS_BLOCK, pSrc, fTime);
						}

						else
						{
							// 충돌되고 있는 상태
							pSrc->Call(CS_OVERLAP, pDest, fTime);
							pDest->Call(CS_OVERLAP, pSrc, fTime);
						}
					}

					// 현재 프레임에서 충돌이 안되고 이전프레임에서는
					// 충돌상태라면 충돌되던 충돌체와 떨어지는
					// 상태라는 것이다.
					else if (CheckPrevCollisionAdj(pSrc->GetSerialNumber(),
						pDest->GetSerialNumber()))
					{
						// 두 충돌체를 서로 충돌상태에서 해제한다.
						DeletePrevCollisionAdj(pSrc->GetSerialNumber(),
							pDest->GetSerialNumber());

						pSrc->DeletePrevCollider(pDest);
						pDest->DeletePrevCollider(pSrc);

						// 충돌되다가 떨어지는 상태 

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
	if (m_SectionUIColliderList.size() >= 2)// 목록에 반드시 두 개가 있어야 한다. 
	{
		list<CCollider*>::iterator	iter;
		list<CCollider*>::iterator	iterEnd = m_SectionUIColliderList.end();
		--iterEnd; // 제일 끝노드의 전까지만 

		for (iter = m_SectionUIColliderList.begin(); iter != iterEnd; ++iter)
		{
			list<CCollider*>::iterator	iter1 = iter;
			++iter1;// iter1 : iter 바깥쪽노드의 다음 부터 시작 되게 만듬 
			list<CCollider*>::iterator	iter1End = m_SectionUIColliderList.end();
			// iter로 접근하는 것이 느림. 비용이 들기 때문에 미리 역참조로 받아놓고 처리하자 
			CCollider* pSrc = *iter;

			PCollisionProfile	pSrcProfile = pSrc->GetProfile();
			int	iSrcChannel = pSrc->GetChannelIndex();

			// (초기값은 위에서 했으니 생략)
			for (; iter1 != iter1End; ++iter1)
			{
				// iter로 접근하는 것이 느림. 비용이 들기 때문에 미리 역참조로 받아놓고 처리하자 
				CCollider* pDest = *iter1;

				PCollisionProfile	pDestProfile = pDest->GetProfile();
				int	iDestChannel = pDest->GetChannelIndex();

				if (pSrcProfile->pChannelList[iDestChannel].eState != CCS_IGNORE &&
					pDestProfile->pChannelList[iSrcChannel].eState != CCS_IGNORE)
				{
					// 충돌 되었을 경우
					if (pSrc->Collision(pDest))
					{
						// 이전 프레임에 충돌이 되었는지 판단한다.
						if (!CheckPrevCollisionAdj(pSrc->GetSerialNumber(),
							pDest->GetSerialNumber()))
						{
							// 두 충돌체를 서로 충돌상태로 등록한다.
							AddPrevCollisionAdj(pSrc->GetSerialNumber(),
								pDest->GetSerialNumber());

							pSrc->AddPrevCollider(pDest);
							pDest->AddPrevCollider(pSrc);

							// 부딪쳤을때
							pSrc->Call(CS_BLOCK, pDest, fTime);
							pDest->Call(CS_BLOCK, pSrc, fTime);
						}

						else
						{
							pSrc->Call(CS_OVERLAP, pDest, fTime);
							pDest->Call(CS_OVERLAP, pSrc, fTime);
						}
					}

					// 현재 프레임에서 충돌이 안되고 이전프레임에서는
					// 충돌상태라면 충돌되던 충돌체와 떨어지는
					// 상태라는 것이다.
					else if (CheckPrevCollisionAdj(pSrc->GetSerialNumber(),
						pDest->GetSerialNumber()))
					{
						// 두 충돌체를 서로 충돌상태에서 해제한다.
						DeletePrevCollisionAdj(pSrc->GetSerialNumber(),
							pDest->GetSerialNumber());

						pSrc->DeletePrevCollider(pDest);
						pDest->DeletePrevCollider(pSrc);

						// 충돌되다가 떨어지는 상태 
						pSrc->Call(CS_RELEASE, pDest, fTime);
						pDest->Call(CS_RELEASE, pSrc, fTime);
					}
				}
			}
		}
	}
}
