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

// 아틀라스 단위 프레임 텍스쳐 
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

// 프레임단위  클립텍스쳐 
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

// 텍스쳐의 컬러키 설정 
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

// 아틀라스 AT_ATLAS
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

// 프레임 AT_FRAME
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
	// 중복거부 
	if (m_pClip->strName == strName)
		return;

	// 기존 내용 초기화
	m_pClip->iFrame = 0;
	m_pClip->fTime = 0.f;
	m_pClip->fOptionTime = 0.f;

	m_pClip = FindClip(strName); // 클립지정

	m_pObj->SetTexture(m_pClip->pTexture); // 현 오브젝트의 텍스쳐를 지정 

	m_pObj->SetSize(m_pClip->vecCoord[0].tEnd - m_pClip->vecCoord[0].tStart); // 현 오브젝트의 클립 사이즈 지정 
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
	//  옵션시간을 0으로 강제로 만든다. 
	m_pClip->fOptionTime = 0.f;
}

void CAnimation::AddRefAllClipTexture() // ★
{
	unordered_map<string, PAnimationClip>::iterator	iter;
	unordered_map<string, PAnimationClip>::iterator	iterEnd = m_mapClip.end();

	for (iter = m_mapClip.begin(); iter != iterEnd; ++iter)
	{
		iter->second->pTexture->AddRef();
	}
}

void CAnimation::SetStopFrame(const string& strName, int iStopFrame) // ★
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

	m_pClip->fTime += fTime; // 시간을 누적한다. 

	/*
	버프를 썼다 .버프를 다시 썼다 .
	30초 짜리인데 15초만 썼다. (버프 실행 중간에 버프를 한번 더 실행한 경우)
	다시 시간이 갱신된다. (버프 시간이 갱신된다)
	void CAnimation::ClearOptionTime()에서 초기화한다.
	*/
	// 일정 시간을 반복하는 애니메이션옵션인 경우 
	if (m_pClip->eOption == AO_TIME_DESTROY)
	{
		m_pClip->fOptionTime += fTime;
		// 누적시간이 클립의 옵션실행시간보다 큰 경우 
		if (m_pClip->fOptionTime >= m_pClip->fOptionPlayTime)
		{
			m_pObj->Active(false);// 파괴한다. 
		}
	}
	// 루프로 들어오는 조건은 애니메이션 시간을 누적시켜서 1프레임에 흘러야 하는 시간이 흘렀는지 판단하고, 
	// 시간이 지났다면 1프레임을 증가시키면서 모션 1개가 프레임이 다 지나갔는지 등을 체크하기위해
	// 누적시간이 1프레임 돌아가는 시간보다 큰 경우, 정상이다
	while (m_pClip->fTime >= m_pClip->fFrameTime)
	{
		// 누적시간에서 1프레임이 돌아가는 시간을 빼준다. 
		// 다음 프레임 시간을 체크를 해서 애니메이션 모션을 다음 프레임으로 진행해야 할지 말아야 할지를 정할 수 있다. 
		m_pClip->fTime -= m_pClip->fFrameTime; // ＠ 시간으로 판단하는 법

		// 추가 
		if (m_pClip->eOption == AO_ONCE_FRAMESTOP) // ★
		{
			if (m_pClip->iFrame == m_pClip->iStopFrame)
				continue;
		}
		// 다음 프레임을 체크한다. 
		++m_pClip->iFrame;
		// 클립좌표벡터의 사이즈와 프레임 넘버가 같은 경우 : 끝까지 지나간 경우 
		if (m_pClip->iFrame == m_pClip->vecCoord.size())
		{
			// 프레임 넘버를 0번으로 맞춘다. 
			m_pClip->iFrame = 0;
			switch (m_pClip->eOption)
			{
			case AO_ONCE_RETURN:
				ChangeClip(m_strDefaultClip);/*
												옵션이 한 번 실행되고 리턴하는 설정인 경우
												디폴트 설정된 클립정보로 변경한다.
											*/
				break;
			case AO_ONCE_DESTROY:
				m_pObj->Active(false);/*
											옵션이 한번 실행되고 파괴하는 모션설정인 경우
											현 오브젝트를 죽인다.
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
