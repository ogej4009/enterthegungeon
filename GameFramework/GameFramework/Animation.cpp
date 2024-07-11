#include "Animation.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "Obj.h"

CAnimation::CAnimation()
{
	m_pClip = nullptr;
	m_pObj = nullptr;
	m_bAuto = true;
}

CAnimation::CAnimation(const CAnimation& anim)
{
	m_pObj = nullptr;
	m_bAuto = anim.m_bAuto;

	m_strDefaultClip = anim.m_strDefaultClip;

	m_mapClip.clear();

	unordered_map<string, PAnimationClip>::const_iterator	iter;
	unordered_map<string, PAnimationClip>::const_iterator	iterEnd = anim.m_mapClip.end();

	for (iter = anim.m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		PAnimationClip	pClip = new AnimationClip;

		*pClip = *iter->second;

		pClip->pTexture->AddRef();

		m_mapClip.insert(make_pair(iter->first, pClip));
	}

	m_pClip = FindClip(anim.m_pClip->strName);
}

CAnimation::~CAnimation()
{
	unordered_map<string, PAnimationClip>::iterator	iter;
	unordered_map<string, PAnimationClip>::iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE(iter->second->pTexture);
		SAFE_DELETE(iter->second);
	}

	m_mapClip.clear();
}

bool CAnimation::AddClip(const string& strName, ANIMATION_OPTION eOption,
	float fPlayTime, float fOptionPlayTime)
{
	PAnimationClip	pClip = FindClip(strName);

	if (pClip)
		return false;

	pClip = new AnimationClip;

	pClip->strName = strName;
	pClip->eOption = eOption;
	pClip->fPlayTime = fPlayTime;
	pClip->fOptionPlayTime = fOptionPlayTime;

	m_mapClip.insert(make_pair(strName, pClip));

	if (!m_pClip)
	{
		m_pClip = pClip;
		m_strDefaultClip = strName;
	}

	return true;
}

// ��Ʋ�� ���� ������ �ؽ��� 
bool CAnimation::SetClipTexture(const string& strClipName,
	const string& strTexName, const TCHAR* pFileName,
	const string& strPathName)
{
	PAnimationClip	pClip = FindClip(strClipName);

	if (!pClip)
		return false;

	pClip->eType = AT_ATLAS;

	GET_SINGLE(CResourceManager)->LoadTexture(strTexName, pFileName, strPathName);

	pClip->pTexture = GET_SINGLE(CResourceManager)->FindTexture(strTexName);

	return true;
}

// �����Ӵ���  Ŭ���ؽ��� 
bool CAnimation::SetClipTexture(const string& strClipName,
	const string& strTexName, const vector<const TCHAR*>& vecFileName,
	const string& strPathName)
{
	PAnimationClip	pClip = FindClip(strClipName);

	if (!pClip)
		return false;

	pClip->eType = AT_FRAME;

	GET_SINGLE(CResourceManager)->LoadTexture(strTexName, vecFileName, strPathName);

	pClip->pTexture = GET_SINGLE(CResourceManager)->FindTexture(strTexName);

	return true;
}

// �ؽ����� �÷�Ű ���� 
void CAnimation::SetTextureColorKey(const string& strName,
	unsigned char r, unsigned char g, unsigned char b)
{
	PAnimationClip	pClip = FindClip(strName);

	if (!pClip)
		return;

	pClip->pTexture->SetColorKey(r, g, b);
	pClip->iColorKey = RGB(r, g, b);
	pClip->bColorKey = true;
}

// ��Ʋ�� AT_ATLAS
bool CAnimation::AddClipCoord(const string& strName,
	const vector<ClipCoord>& vecCoord)
{
	PAnimationClip	pClip = FindClip(strName);

	if (!pClip)
		return false;

	pClip->vecCoord = vecCoord;

	pClip->fFrameTime = pClip->fPlayTime / pClip->vecCoord.size();

	return true;
}

// ������ AT_FRAME
bool CAnimation::AddClipCoord(const string& strName,
	const ClipCoord& tCoord, int iCount)
{
	PAnimationClip	pClip = FindClip(strName);

	if (!pClip)
		return false;

	for (int i = 0; i < iCount; ++i)
	{
		pClip->vecCoord.push_back(tCoord);
	}

	pClip->fFrameTime = pClip->fPlayTime / iCount;

	return true;
}

void CAnimation::SetDefaultClip(const string& strName)
{
	m_strDefaultClip = strName;
}

void CAnimation::SetCurrentClip(const string& strName)
{
	m_pClip = FindClip(strName);
}

void CAnimation::ChangeClip(const string& strName)
{
	// �ߺ��ź� 
	if (m_pClip->strName == strName)
		return;

	// ���� ���� �ʱ�ȭ
	m_pClip->iFrame = 0;
	m_pClip->fTime = 0.f;
	m_pClip->fOptionTime = 0.f;

	m_pClip = FindClip(strName); // Ŭ������

	m_pObj->SetTexture(m_pClip->pTexture); // �� ������Ʈ�� �ؽ��ĸ� ���� 

	m_pObj->SetSize(m_pClip->vecCoord[0].tEnd - m_pClip->vecCoord[0].tStart); // �� ������Ʈ�� Ŭ�� ������ ���� 
}

PAnimationClip CAnimation::FindClip(const string& strName)
{
	unordered_map<string, PAnimationClip>::const_iterator	iter = m_mapClip.find(strName);

	if (iter == m_mapClip.end())
		return nullptr;

	return iter->second;
}

void CAnimation::ClearOptionTime()
{
	//  �ɼǽð��� 0���� ������ �����. 
	m_pClip->fOptionTime = 0.f;
}

void CAnimation::AddRefAllClipTexture() // ��
{
	unordered_map<string, PAnimationClip>::iterator	iter;
	unordered_map<string, PAnimationClip>::iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		iter->second->pTexture->AddRef();
	}
}

void CAnimation::SetStopFrame(const string& strName, int iStopFrame) // ��
{
	PAnimationClip	pClip = FindClip(strName);

	if (!pClip)
		return;

	pClip->iStopFrame = iStopFrame;
}

void CAnimation::Start()
{
}

bool CAnimation::Init()
{
	return true;
}

void CAnimation::Update(float fTime)
{
	if (!m_bAuto)
		return;

	m_pClip->fTime += fTime; // �ð��� �����Ѵ�. 

	/*
	������ ��� .������ �ٽ� ��� .
	30�� ¥���ε� 15�ʸ� ���. (���� ���� �߰��� ������ �ѹ� �� ������ ���)
	�ٽ� �ð��� ���ŵȴ�. (���� �ð��� ���ŵȴ�)
	void CAnimation::ClearOptionTime()���� �ʱ�ȭ�Ѵ�.
	*/
	// ���� �ð��� �ݺ��ϴ� �ִϸ��̼ǿɼ��� ��� 
	if (m_pClip->eOption == AO_TIME_DESTROY)
	{
		m_pClip->fOptionTime += fTime;
		// �����ð��� Ŭ���� �ɼǽ���ð����� ū ��� 
		if (m_pClip->fOptionTime >= m_pClip->fOptionPlayTime)
		{
			m_pObj->Active(false);// �ı��Ѵ�. 
		}
	}
	// ������ ������ ������ �ִϸ��̼� �ð��� �������Ѽ� 1�����ӿ� �귯�� �ϴ� �ð��� �귶���� �Ǵ��ϰ�, 
	// �ð��� �����ٸ� 1�������� ������Ű�鼭 ��� 1���� �������� �� ���������� ���� üũ�ϱ�����
	// �����ð��� 1������ ���ư��� �ð����� ū ���, �����̴�
	while (m_pClip->fTime >= m_pClip->fFrameTime)
	{
		// �����ð����� 1�������� ���ư��� �ð��� ���ش�. 
		// ���� ������ �ð��� üũ�� �ؼ� �ִϸ��̼� ����� ���� ���������� �����ؾ� ���� ���ƾ� ������ ���� �� �ִ�. 
		m_pClip->fTime -= m_pClip->fFrameTime; // �� �ð����� �Ǵ��ϴ� ��

		// �߰� 
		if (m_pClip->eOption == AO_ONCE_FRAMESTOP) // ��
		{
			if (m_pClip->iFrame == m_pClip->iStopFrame)
				continue;
		}
		// ���� �������� üũ�Ѵ�. 
		++m_pClip->iFrame;
		// Ŭ����ǥ������ ������� ������ �ѹ��� ���� ��� : ������ ������ ��� 
		if (m_pClip->iFrame == m_pClip->vecCoord.size())
		{
			// ������ �ѹ��� 0������ �����. 
			m_pClip->iFrame = 0;
			switch (m_pClip->eOption)
			{
			case AO_ONCE_RETURN:
				ChangeClip(m_strDefaultClip);/*
												�ɼ��� �� �� ����ǰ� �����ϴ� ������ ���
												����Ʈ ������ Ŭ�������� �����Ѵ�.
											*/
				break;
			case AO_ONCE_DESTROY:
				m_pObj->Active(false);/*
											�ɼ��� �ѹ� ����ǰ� �ı��ϴ� ��Ǽ����� ���
											�� ������Ʈ�� ���δ�.
									  */
				break;
			}
		}
	}
}

CAnimation* CAnimation::Clone() const
{
	return new CAnimation(*this);
}

void CAnimation::Save(FILE* pFile)
{
	int	iLength = m_strTag.length();

	fwrite(&iLength, sizeof(int), 1, pFile);
	fwrite(m_strTag.c_str(), sizeof(char), iLength, pFile);

	fwrite(&m_bAuto, sizeof(bool), 1, pFile);

	iLength = m_mapClip.size();
	fwrite(&iLength, sizeof(int), 1, pFile);

	unordered_map<string, PAnimationClip>::iterator	iter;
	unordered_map<string, PAnimationClip>::iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		PAnimationClip	pClip = iter->second;
		iLength = pClip->strName.length();

		fwrite(&iLength, sizeof(int), 1, pFile);
		fwrite(pClip->strName.c_str(), sizeof(char), iLength, pFile);

		fwrite(&pClip->eType, sizeof(ANIMATION_TYPE), 1, pFile);
		fwrite(&pClip->eOption, sizeof(ANIMATION_OPTION), 1, pFile);
		fwrite(&pClip->iColorKey, sizeof(unsigned int), 1, pFile);
		fwrite(&pClip->bColorKey, sizeof(bool), 1, pFile);
		fwrite(&pClip->iFrame, sizeof(int), 1, pFile);

		fwrite(&pClip->fTime, sizeof(float), 1, pFile);
		fwrite(&pClip->fPlayTime, sizeof(float), 1, pFile);
		fwrite(&pClip->fFrameTime, sizeof(float), 1, pFile);
		fwrite(&pClip->fOptionTime, sizeof(float), 1, pFile);
		fwrite(&pClip->fOptionPlayTime, sizeof(float), 1, pFile);

		iLength = pClip->vecCoord.size();
		fwrite(&iLength, sizeof(int), 1, pFile);
		fwrite(&pClip->vecCoord[0], sizeof(ClipCoord), iLength, pFile);

		bool	bEnable = false;

		if (pClip->pTexture)
			bEnable = true;

		fwrite(&bEnable, sizeof(bool), 1, pFile);

		if (pClip->pTexture)
			pClip->pTexture->Save(pFile);
	}

	iLength = m_pClip->strName.length();
	fwrite(&iLength, sizeof(int), 1, pFile);
	fwrite(m_pClip->strName.c_str(), sizeof(char), iLength, pFile);

	iLength = m_strDefaultClip.length();
	fwrite(&iLength, sizeof(int), 1, pFile);
	fwrite(m_strDefaultClip.c_str(), sizeof(char), iLength, pFile);
}

void CAnimation::Load(FILE* pFile)
{
	int	iLength = 0;

	char	strText[256] = {};
	fread(&iLength, sizeof(int), 1, pFile);
	fread(strText, sizeof(char), iLength, pFile);
	m_strTag = strText;

	fread(&m_bAuto, sizeof(bool), 1, pFile);

	int	iClipCount = 0;
	fread(&iClipCount, sizeof(int), 1, pFile);

	for (int i = 0; i < iClipCount; ++i)
	{
		PAnimationClip	pClip = new AnimationClip;

		fread(&iLength, sizeof(int), 1, pFile);
		memset(strText, 0, 256);
		fread(strText, sizeof(char), iLength, pFile);

		pClip->strName = strText;

		fread(&pClip->eType, sizeof(ANIMATION_TYPE), 1, pFile);
		fread(&pClip->eOption, sizeof(ANIMATION_OPTION), 1, pFile);
		fread(&pClip->iColorKey, sizeof(unsigned int), 1, pFile);
		fread(&pClip->bColorKey, sizeof(bool), 1, pFile);
		fread(&pClip->iFrame, sizeof(int), 1, pFile);

		fread(&pClip->fTime, sizeof(float), 1, pFile);
		fread(&pClip->fPlayTime, sizeof(float), 1, pFile);
		fread(&pClip->fFrameTime, sizeof(float), 1, pFile);
		fread(&pClip->fOptionTime, sizeof(float), 1, pFile);
		fread(&pClip->fOptionPlayTime, sizeof(float), 1, pFile);

		fread(&iLength, sizeof(int), 1, pFile);
		pClip->vecCoord.resize(iLength);
		fread(&pClip->vecCoord[0], sizeof(ClipCoord), iLength, pFile);

		bool	bEnable = false;

		fread(&bEnable, sizeof(bool), 1, pFile);

		if (bEnable)
		{
			char	strText[256] = {};
			fread(&iLength, sizeof(int), 1, pFile);
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

			pClip->pTexture = GET_SINGLE(CResourceManager)->FindTexture(strKey);

			pClip->pTexture->Load(pFile);

			m_mapClip.insert(make_pair(pClip->strName, pClip));
		}
	}

	fread(&iLength, sizeof(int), 1, pFile);
	memset(strText, 0, 256);
	fread(strText, sizeof(char), iLength, pFile);

	m_pClip = FindClip(strText);

	fread(&iLength, sizeof(int), 1, pFile);
	memset(strText, 0, 256);
	fread(strText, sizeof(char), iLength, pFile);
	m_strDefaultClip = strText;

	m_pObj->SetTexture(m_pClip->pTexture);
}
