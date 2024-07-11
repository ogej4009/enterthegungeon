#include "SoundManager.h"
#include "PathManager.h"
#include "SceneManager.h"


DEFINITION_SINGLE(CSoundManager)

CSoundManager::CSoundManager() :
	m_pSystem(nullptr)
{
	m_vecChannel.resize(CT_END);
}

CSoundManager::~CSoundManager()
{
	unordered_map<string, PSoundInfo>::iterator	iter;
	unordered_map<string, PSoundInfo>::iterator	iterEnd = m_mapSound.end();

	for (iter = m_mapSound.begin(); iter != iterEnd; ++iter)
	{
		iter->second->pSound->release();
		SAFE_RELEASE(iter->second);
	}

	m_mapSound.clear();

	if (m_pSystem)
	{
		m_pSystem->close();
		m_pSystem->release();
	}
}

bool CSoundManager::Init()
{
	// System을 생성한다.
	System_Create(&m_pSystem);

	// System을 초기화한다.
	m_pSystem->init(FMOD_MAX_CHANNEL_WIDTH, FMOD_INIT_NORMAL, nullptr);

	return true;
}

bool CSoundManager::LoadSound(const string& strName, bool bLoop, const char* pFileName,
	const string& strPathName)
{
	// 이미지 생성시 카운트를 2로 해놓고 장면 전환시
	// 원하는 이미지들만 지워지도록 처리한다.
	PSoundInfo	pInfo = FindSound(strName);

	if (pInfo)
	{
		SAFE_RELEASE(pInfo);
		return false;
	}

	pInfo = new SoundInfo;

	const char* pPath = GET_SINGLE(CPathManager)->FindPathMultibyte(strPathName);

	string	strFullPath;

	if (pPath)
		strFullPath = pPath;

	strFullPath += pFileName;

	FMOD_MODE	eMode = FMOD_DEFAULT;

	if (bLoop)
		eMode = FMOD_LOOP_NORMAL;

	m_pSystem->createSound(strFullPath.c_str(), eMode, nullptr, &pInfo->pSound);

	if (GET_SINGLE(CSceneManager)->CreateSceneEnable())
		pInfo->AddRef();

	m_mapSound.insert(make_pair(strName, pInfo));

	return true;
}

void CSoundManager::ClearScene()
{
	unordered_map<string, PSoundInfo>::iterator	iter;
	unordered_map<string, PSoundInfo>::iterator	iterEnd = m_mapSound.end();

	for (iter = m_mapSound.begin(); iter != iterEnd;)
	{
		if (iter->second->Release() == 0)
		{
			iter = m_mapSound.erase(iter);
			iterEnd = m_mapSound.end();
			continue;
		}

		++iter;
	}
}

void CSoundManager::Play(const string& strName, CHANNEL_TYPE eType)
{
	PSoundInfo	pInfo = FindSound(strName);

	if (!pInfo)
		return;

	Channel* pChannel = nullptr;
	m_pSystem->playSound(pInfo->pSound, nullptr, false, &m_vecChannel[eType]);

	SAFE_RELEASE(pInfo);
}

void CSoundManager::Stop(CHANNEL_TYPE eType)
{
	if (m_vecChannel[eType])
	{
		bool	bPlay = false;
		m_vecChannel[eType]->isPlaying(&bPlay);

		if (bPlay)
			m_vecChannel[eType]->stop();
	}
}

void CSoundManager::Update(float fTime)
{
	m_pSystem->update();
}

PSoundInfo CSoundManager::FindSound(const string& strName)
{
	unordered_map<string, PSoundInfo>::iterator	iter = m_mapSound.find(strName);

	if (iter == m_mapSound.end())
		return nullptr;

	iter->second->AddRef();

	return iter->second;
}
