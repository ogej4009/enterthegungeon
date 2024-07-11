#include "Obj.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "ColliderRect.h"
#include "ColliderSphere.h"
#include "ColliderPoint.h"
#include "CollisionManager.h"
#include "Animation.h"
#include "Camera.h"
#include "PathManager.h"

// 전역변수 
CSerialNumber CObj::m_SerialNumber;

CObj::CObj() :
	m_pTexture(nullptr),
	m_pAnimation(nullptr),
	m_fMoveSpeed(0.f),
	m_bIsUI(false),
	m_iSerialNumber(0),
	m_fAngle(0.f),
	m_bGravity(false),
	m_fGravityTime(0.f),
	m_fGravityForce(0.f),
	m_pScene(nullptr),
	m_bCameraEnable(true)
{
}

CObj::CObj(const CObj& obj)
{
	// 얇은 복사 
	*this = obj;

	if (m_pTexture)
		m_pTexture->AddRef();

	m_ColliderList.clear();

	list<CCollider*>::const_iterator	iter;
	list<CCollider*>::const_iterator	iterEnd = obj.m_ColliderList.end();

	for (iter = obj.m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		CCollider* pCollider = (*iter)->Clone();

		pCollider->m_pObj = this;
		pCollider->SetScene(m_pScene);
		m_ColliderList.push_back(pCollider);
	}

	if (obj.m_pAnimation)
		m_pAnimation = obj.m_pAnimation->Clone();
}

CObj::~CObj()
{
	list<CCollider*>::const_iterator	iter;
	list<CCollider*>::const_iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Active(false); // 충돌체 지움 
	}

	Safe_Release_VecList(m_ColliderList);  //충돌목록
	SAFE_RELEASE(m_pTexture); // 지정 택스쳐 
	SAFE_RELEASE(m_pAnimation); // 지정 애니메이션 

	if (m_pScene)
		m_pScene->DeleteObject(this); // 씬 정보 
}

// 오브젝트의 위치정보를 갱신할 때, 충돌체들의 위치에 까지 영향을 주게 만들어준 함수 
void CObj::UpdatePos(const Position& tPos)
{
	m_tPos = tPos;

	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->UpdateObjPos(m_tPos);
	}
}

void CObj::SetTexture(const string& strName, const TCHAR* pFileName, const string& strPathName)
{
	SAFE_RELEASE(m_pTexture);

	if (pFileName)
		GET_SINGLE(CResourceManager)->LoadTexture(strName, pFileName, strPathName);

	m_pTexture = GET_SINGLE(CResourceManager)->FindTexture(strName);
}

void CObj::SetTexture(CTexture* pTexture)
{
	SAFE_RELEASE(m_pTexture);
	m_pTexture = pTexture;

	if (pTexture)
		pTexture->AddRef();
}

void CObj::SetColorKey(unsigned char r, unsigned char g, unsigned char b) // 컬러키 적용 
{
	if (m_pTexture)
		m_pTexture->SetColorKey(r, g, b);
}

CObj* CObj::FindObject(const string& strName)
{// 씬에 등록된 오브젝트의 목록에서 찾는다. 
	return m_pScene->FindObject(strName);
}

void CObj::CreateAnimation(const string& strName)
{
	m_pAnimation = new CAnimation;

	m_pAnimation->m_pObj = this;
	m_pAnimation->SetTag(strName);
}

void CObj::AddRefAllClipTexture() // ★
{// AddRefAllClipTexture <- 이 함수를 호출하는 조건을 만든다. 
	if (m_pAnimation)
		m_pAnimation->AddRefAllClipTexture();
}

void CObj::AddRefTexture()
{
	if (m_pTexture)
		m_pTexture->AddRef();
}

void CObj::Start()
{
	/*
		프로토 타입은 스타트 함수가 아예 호출이 안된다.
		월드에 배치되어있는 오브젝트만 스타트 함수가 호출이 가능하다.

		월드에 배치된 충돌체들만, 따로 충돌관리자에 자동으로 등록되게 만든다.
		추가되어 있는 충돌체를 관리자에 등록한다.
		각각 자기 자신을 스스로 충돌 매니저에 등록이 되는 방식
	*/

	// 추가되어 있는 충돌체를 관리자에 등록한다.
	list<CCollider*>::const_iterator	iter;
	list<CCollider*>::const_iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	// 충돌매니져가 월드에 포함된 오브젝트 목록들만 등록, 자기 자신을 등록한다.  
	GET_SINGLE(CCollisionManager)->AddWorldObjList(this);

	if (m_pAnimation)
	{
		SAFE_RELEASE(m_pTexture);
		m_pTexture = m_pAnimation->GetClip()->pTexture;
		m_pTexture->AddRef();

		m_pAnimation->m_pObj = this;

		PAnimationClip	pClip = m_pAnimation->GetClip();

		m_tSize = pClip->vecCoord[0].tEnd - pClip->vecCoord[0].tStart;
	}
}

bool CObj::Init()
{
	return true;
}

void CObj::Update(float fTime)
{
	if (m_pAnimation)
		m_pAnimation->Update(fTime);

	// 중력 적용 
	if (m_bGravity)
	{
		m_fGravityTime += fTime;

		float	fForce = GRAVITY * m_fGravityTime * m_fGravityTime; // 중력가속도 : 9.8 * 시간 제곱
		m_fGravityForce -= fForce; // 위로 오르는 힘에서 - 중력을 뺴준다. 
		/*
			아래로 떨어지는 것은 포스값이 증가하는 것
			포스가 남아있을 떄는 위로 가는 것은 -값,  -가 되는 순간 떨어짐
			델타 타임에 따라서 점프값들이 영향을 받아야 함
			m_tPos.y 좌표가 영향을 받음
			시간에 영향을 받기 때문에 포물선 발사체는 단발로 쓰면 타격하는데 힘들다
			일정한 속도를 나오게 하기 위해 fTime을 이용한다
		*/
		m_tPos.y -= m_fGravityForce * fTime; // 물체의 위치는 당연히 그힘에 영향을 받는다. 
	}

	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		// 예외처리 
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

		// 업데이트
		(*iter)->Update(fTime);
		++iter;
	}
}

void CObj::LateUpdate(float fTime)
{
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

		(*iter)->LateUpdate(fTime);
		++iter;
	}
}

void CObj::Collision(float fTime)
{
}

void CObj::PrevRender(HDC hDC, float fTime)
{
	m_tMove = m_tPos - m_tPrevPos;

	m_tPrevPos = m_tPos;

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

		(*iter)->PrevRender(hDC, fTime);
		++iter;
	}
}

void CObj::Render(HDC hDC, float fTime)
{
	Position	tWorldPos = m_tPos;

	// ＠ 피봇을 적용한다.
	tWorldPos -= m_tPivot * m_tSize;

	// ＠ 카메라를 적용한다.
	if (m_bCameraEnable)
		tWorldPos -= GET_SINGLE(CCamera)->GetPos();  /*
														월드에 배치된 어떤 물체가 카메라 외부에있다.
														출력좌표는 카메라의 좌상단을 기준으로 처리하기 떄문에
														출력을 하기 위한 상대적인 좌표가 나온다.
														월드좌표에서 카메라 위치 뺴버리면 된다.
														카메라가 모든 오브젝트에 영향을 준다.
														UI는 그대로 화면을 따라 다닌다.
														UI아닌 월드상에 배치된 물체들은 이 스크롤에 영향을 받는다.
													*/

	if (m_pTexture)
	{
		Position	tImagePos;

		int	iFrame = 0;

		if (m_pAnimation)
		{
			PAnimationClip	pClip = m_pAnimation->GetClip();

			tImagePos = pClip->vecCoord[pClip->iFrame].tStart;

			if (pClip->eType == AT_FRAME)
				iFrame = pClip->iFrame;
		}

		m_pTexture->Render(hDC, tWorldPos, tImagePos, m_tSize, iFrame);
	}


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

		(*iter)->Render(hDC, fTime);
		++iter;
	}
}

void CObj::PostRender(HDC hDC, float fTime)
{
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

		(*iter)->PostRender(hDC, fTime);
		++iter;
	}
}

CObj* CObj::Clone()	const
{
	return nullptr;
}

void CObj::SaveFromFullPath(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "wb");

	if (!pFile)
		return;

	Save(pFile);

	fclose(pFile);
}

void CObj::SaveFromFileName(const char* pFileName,
	const string& strPath)
{
	char	strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(strPath);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFileName);

	SaveFromFullPath(strFullPath);
}

void CObj::Save(FILE* pFile)
{
	// Tag 저장
	int	iLength = m_strTag.length();
	fwrite(&iLength, sizeof(int), 1, pFile);
	fwrite(m_strTag.c_str(), 1, iLength, pFile);

	iLength = m_strLayerName.length();
	fwrite(&iLength, sizeof(int), 1, pFile);
	fwrite(m_strLayerName.c_str(), 1, iLength, pFile);

	fwrite(&m_bIsUI, sizeof(bool), 1, pFile);

	fwrite(&m_tPos, sizeof(Position), 1, pFile);
	fwrite(&m_tPivot, sizeof(Position), 1, pFile);
	fwrite(&m_tSize, sizeof(Size), 1, pFile);
	fwrite(&m_fAngle, sizeof(float), 1, pFile);
	fwrite(&m_fMoveSpeed, sizeof(float), 1, pFile);
	fwrite(&m_bGravity, sizeof(bool), 1, pFile);
	fwrite(&m_fGravityTime, sizeof(float), 1, pFile);
	fwrite(&m_fGravityForce, sizeof(float), 1, pFile);
	fwrite(&m_bCameraEnable, sizeof(bool), 1, pFile);

	// Texture가 있는지 저장한다.
	bool	bEnable = false;

	if (m_pTexture)
		bEnable = true;

	fwrite(&bEnable, sizeof(bool), 1, pFile);

	if (m_pTexture)
	{
		m_pTexture->Save(pFile);
	}

	iLength = m_ColliderList.size();

	fwrite(&iLength, sizeof(int), 1, pFile);

	list<class CCollider*>::iterator	iter;
	list<class CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		// 충돌체 타입을 먼저 저장한다.
		COLLIDER_TYPE	eType = (*iter)->GetColliderType();

		fwrite(&eType, sizeof(COLLIDER_TYPE), 1, pFile);

		(*iter)->Save(pFile);
	}

	bEnable = false;

	if (m_pAnimation)
		bEnable = true;

	fwrite(&bEnable, sizeof(bool), 1, pFile);

	if (m_pAnimation)
	{
		m_pAnimation->Save(pFile);
	}
}

void CObj::LoadFromFullPath(const char* pFullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, pFullPath, "rb");

	if (!pFile)
		return;

	Load(pFile);

	fclose(pFile);
}

void CObj::LoadFromFileName(const char* pFileName, const string& strPath)
{
	char	strFullPath[MAX_PATH] = {};

	const char* pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(strPath);

	if (pPath)
		strcpy_s(strFullPath, pPath);

	strcat_s(strFullPath, pFileName);

	LoadFromFullPath(strFullPath);
}

void CObj::Load(FILE* pFile)
{
	// Tag 저장
	int	iLength = 0;
	char	strText[256] = {};
	fread(&iLength, sizeof(int), 1, pFile);
	fread(strText, 1, iLength, pFile);

	m_strTag = strText;

	fread(&iLength, sizeof(int), 1, pFile);
	memset(strText, 0, 256);
	fread(strText, 1, iLength, pFile);

	m_strLayerName = strText;

	fread(&m_bIsUI, sizeof(bool), 1, pFile);

	fread(&m_tPos, sizeof(Position), 1, pFile);
	fread(&m_tPivot, sizeof(Position), 1, pFile);
	fread(&m_tSize, sizeof(Size), 1, pFile);
	fread(&m_fAngle, sizeof(float), 1, pFile);
	fread(&m_fMoveSpeed, sizeof(float), 1, pFile);
	fread(&m_bGravity, sizeof(bool), 1, pFile);
	fread(&m_fGravityTime, sizeof(float), 1, pFile);
	fread(&m_fGravityForce, sizeof(float), 1, pFile);
	fread(&m_bCameraEnable, sizeof(bool), 1, pFile);

	// Texture가 있는지 저장한다.
	bool	bEnable = false;

	fread(&bEnable, sizeof(bool), 1, pFile);

	if (bEnable)
	{
		SAFE_RELEASE(m_pTexture);
		fread(&iLength, sizeof(int), 1, pFile);
		memset(strText, 0, 256);
		fread(strText, 1, iLength, pFile);

		string	strKey = strText;

		// 파일 경로를 저장한다.
		size_t iCount = 0;

		fread(&iCount, sizeof(size_t), 1, pFile);

		vector<const TCHAR*>	vecFileName;

		for (size_t i = 0; i < iCount; ++i)
		{
			TCHAR* pFileName = new TCHAR[MAX_PATH];
			memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
			fread(&iLength, sizeof(int), 1, pFile);
			fread(pFileName, sizeof(TCHAR), iLength, pFile);

			vecFileName.push_back(pFileName);
		}

		fread(&iLength, sizeof(int), 1, pFile);
		memset(strText, 0, 256);
		fread(strText, sizeof(char), iLength, pFile);

		string	strPath = strText;

		if (vecFileName.size() == 1)
			GET_SINGLE(CResourceManager)->LoadTexture(strKey, vecFileName[0], strPath);

		else
			GET_SINGLE(CResourceManager)->LoadTexture(strKey, vecFileName, strPath);

		Safe_Delete_Array_VecList(vecFileName);

		m_pTexture = GET_SINGLE(CResourceManager)->FindTexture(strKey);

		m_pTexture->Load(pFile);
	}

	fread(&iLength, sizeof(int), 1, pFile);

	for (int i = 0; i < iLength; ++i)
	{
		COLLIDER_TYPE	eType;

		fread(&eType, sizeof(COLLIDER_TYPE), 1, pFile);

		CCollider* pCollider = nullptr;

		switch (eType)
		{
		case CT_RECT:
			pCollider = new CColliderRect;
			break;
		case CT_SPHERE:
			pCollider = new CColliderSphere;
			break;
		case CT_POINT:
			pCollider = new CColliderPoint;
			break;
		}

		pCollider->m_pObj = this;
		pCollider->Load(pFile);

		m_ColliderList.push_back(pCollider);
	}

	bEnable = false;

	fread(&bEnable, sizeof(bool), 1, pFile);

	if (bEnable)
	{
		m_pAnimation = new CAnimation;
		m_pAnimation->m_pObj = this;
		m_pAnimation->Load(pFile);
	}
}

// ★실제 물체의 움직임 
void CObj::Move(const Position& tDir)
{
	m_tPos += tDir * m_fMoveSpeed;
}

// ★실제 물체의 움직임
void CObj::Move(const Position& tDir, float fTime)
{
	m_tPos += tDir * m_fMoveSpeed * fTime;
}

//  ★실제 물체의 움직임
void CObj::Move(DIR eDir)
{
	switch (eDir)
	{
	case LEFT:
		m_tPos.x -= m_fMoveSpeed;
		break;
	case RIGHT:
		m_tPos.x += m_fMoveSpeed;
		break;
	case UP:
		m_tPos.y -= m_fMoveSpeed;
		break;
	case DOWN:
		m_tPos.y += m_fMoveSpeed;
		break;
	}
}

//  ★실제 물체의 움직임
void CObj::Move(DIR eDir, float fTime)
{
	switch (eDir)
	{
	case LEFT:
		m_tPos.x -= m_fMoveSpeed * fTime;
		break;
	case RIGHT:
		m_tPos.x += m_fMoveSpeed * fTime;
		break;
	case UP:
		m_tPos.y -= m_fMoveSpeed * fTime;
		break;
	case DOWN:
		m_tPos.y += m_fMoveSpeed * fTime;
		break;
	}
}
//  ★실제 물체의 움직임
void CObj::MoveAngle()
{
	m_tPos.x += cosf(CMath::AngleToRadian(m_fAngle)) * m_fMoveSpeed;
	m_tPos.y += sinf(CMath::AngleToRadian(m_fAngle)) * m_fMoveSpeed;
}
//  ★실제 물체의 움직임
void CObj::MoveAngle(float fTime)
{
	m_tPos.x += cosf(CMath::AngleToRadian(m_fAngle)) * m_fMoveSpeed * fTime;
	m_tPos.y += sinf(CMath::AngleToRadian(m_fAngle)) * m_fMoveSpeed * fTime;
}


CCollider* CObj::FindCollider(const string& strName)
{
	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTag() == strName)
		{
			(*iter)->AddRef();
			return *iter;
		}
	}

	return nullptr;
}
