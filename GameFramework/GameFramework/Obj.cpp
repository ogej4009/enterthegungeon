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

// �������� 
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
	// ���� ���� 
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
		(*iter)->Active(false); // �浹ü ���� 
	}

	Safe_Release_VecList(m_ColliderList);  //�浹���
	SAFE_RELEASE(m_pTexture); // ���� �ý��� 
	SAFE_RELEASE(m_pAnimation); // ���� �ִϸ��̼� 

	if (m_pScene)
		m_pScene->DeleteObject(this); // �� ���� 
}

// ������Ʈ�� ��ġ������ ������ ��, �浹ü���� ��ġ�� ���� ������ �ְ� ������� �Լ� 
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

void CObj::SetColorKey(unsigned char r, unsigned char g, unsigned char b) // �÷�Ű ���� 
{
	if (m_pTexture)
		m_pTexture->SetColorKey(r, g, b);
}

CObj* CObj::FindObject(const string& strName)
{// ���� ��ϵ� ������Ʈ�� ��Ͽ��� ã�´�. 
	return m_pScene->FindObject(strName);
}

void CObj::CreateAnimation(const string& strName)
{
	m_pAnimation = new CAnimation;

	m_pAnimation->m_pObj = this;
	m_pAnimation->SetTag(strName);
}

void CObj::AddRefAllClipTexture() // ��
{// AddRefAllClipTexture <- �� �Լ��� ȣ���ϴ� ������ �����. 
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
		������ Ÿ���� ��ŸƮ �Լ��� �ƿ� ȣ���� �ȵȴ�.
		���忡 ��ġ�Ǿ��ִ� ������Ʈ�� ��ŸƮ �Լ��� ȣ���� �����ϴ�.

		���忡 ��ġ�� �浹ü�鸸, ���� �浹�����ڿ� �ڵ����� ��ϵǰ� �����.
		�߰��Ǿ� �ִ� �浹ü�� �����ڿ� ����Ѵ�.
		���� �ڱ� �ڽ��� ������ �浹 �Ŵ����� ����� �Ǵ� ���
	*/

	// �߰��Ǿ� �ִ� �浹ü�� �����ڿ� ����Ѵ�.
	list<CCollider*>::const_iterator	iter;
	list<CCollider*>::const_iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}

	// �浹�Ŵ����� ���忡 ���Ե� ������Ʈ ��ϵ鸸 ���, �ڱ� �ڽ��� ����Ѵ�.  
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

	// �߷� ���� 
	if (m_bGravity)
	{
		m_fGravityTime += fTime;

		float	fForce = GRAVITY * m_fGravityTime * m_fGravityTime; // �߷°��ӵ� : 9.8 * �ð� ����
		m_fGravityForce -= fForce; // ���� ������ ������ - �߷��� ���ش�. 
		/*
			�Ʒ��� �������� ���� �������� �����ϴ� ��
			������ �������� ���� ���� ���� ���� -��,  -�� �Ǵ� ���� ������
			��Ÿ Ÿ�ӿ� ���� ���������� ������ �޾ƾ� ��
			m_tPos.y ��ǥ�� ������ ����
			�ð��� ������ �ޱ� ������ ������ �߻�ü�� �ܹ߷� ���� Ÿ���ϴµ� �����
			������ �ӵ��� ������ �ϱ� ���� fTime�� �̿��Ѵ�
		*/
		m_tPos.y -= m_fGravityForce * fTime; // ��ü�� ��ġ�� �翬�� ������ ������ �޴´�. 
	}

	list<CCollider*>::iterator	iter;
	list<CCollider*>::iterator	iterEnd = m_ColliderList.end();

	for (iter = m_ColliderList.begin(); iter != iterEnd;)
	{
		// ����ó�� 
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

		// ������Ʈ
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

	// �� �Ǻ��� �����Ѵ�.
	tWorldPos -= m_tPivot * m_tSize;

	// �� ī�޶� �����Ѵ�.
	if (m_bCameraEnable)
		tWorldPos -= GET_SINGLE(CCamera)->GetPos();  /*
														���忡 ��ġ�� � ��ü�� ī�޶� �ܺο��ִ�.
														�����ǥ�� ī�޶��� �»���� �������� ó���ϱ� ������
														����� �ϱ� ���� ������� ��ǥ�� ���´�.
														������ǥ���� ī�޶� ��ġ �������� �ȴ�.
														ī�޶� ��� ������Ʈ�� ������ �ش�.
														UI�� �״�� ȭ���� ���� �ٴѴ�.
														UI�ƴ� ����� ��ġ�� ��ü���� �� ��ũ�ѿ� ������ �޴´�.
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
	// Tag ����
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

	// Texture�� �ִ��� �����Ѵ�.
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
		// �浹ü Ÿ���� ���� �����Ѵ�.
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
	// Tag ����
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

	// Texture�� �ִ��� �����Ѵ�.
	bool	bEnable = false;

	fread(&bEnable, sizeof(bool), 1, pFile);

	if (bEnable)
	{
		SAFE_RELEASE(m_pTexture);
		fread(&iLength, sizeof(int), 1, pFile);
		memset(strText, 0, 256);
		fread(strText, 1, iLength, pFile);

		string	strKey = strText;

		// ���� ��θ� �����Ѵ�.
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

// �ڽ��� ��ü�� ������ 
void CObj::Move(const Position& tDir)
{
	m_tPos += tDir * m_fMoveSpeed;
}

// �ڽ��� ��ü�� ������
void CObj::Move(const Position& tDir, float fTime)
{
	m_tPos += tDir * m_fMoveSpeed * fTime;
}

//  �ڽ��� ��ü�� ������
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

//  �ڽ��� ��ü�� ������
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
//  �ڽ��� ��ü�� ������
void CObj::MoveAngle()
{
	m_tPos.x += cosf(CMath::AngleToRadian(m_fAngle)) * m_fMoveSpeed;
	m_tPos.y += sinf(CMath::AngleToRadian(m_fAngle)) * m_fMoveSpeed;
}
//  �ڽ��� ��ü�� ������
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
