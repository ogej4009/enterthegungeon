#pragma once
#include "Ref.h"

class CAnimation :
	public CRef
{
	friend class CObj;

private:
	CAnimation();
	CAnimation(const CAnimation& anim);
	~CAnimation();

private:
	unordered_map<string, PAnimationClip>	m_mapClip; //  여러 개의 클립정보를 담아놓고 교체하는 방식 
	PAnimationClip		m_pClip; // 현재의 클립정보
	class CObj* m_pObj; // 모든 애니메이션은 자신을 가진 Obj를 안다. 
	string		m_strDefaultClip; // 디폴트 클립정보 
	bool		m_bAuto;// 애니메이션의 업데이트를 진행할지 말지를 정함 

public:
	PAnimationClip GetClip()	const
	{
		return m_pClip;
	}

	void SetAuto(bool bAuto)
	{
		m_bAuto = bAuto;
	}

public:
	bool AddClip(const string& strName, ANIMATION_OPTION eOption,
		float fPlayTime, float fOptionPlayTime = 0.f);
	bool SetClipTexture(const string& strClipName, const string& strTexName,
		const TCHAR* pFileName, const string& strPathName = TEXTURE_PATH);// AT_ATLAS 용
	bool SetClipTexture(const string& strClipName, const string& strTexName,
		const vector<const TCHAR*>& vecFileName, const string& strPathName = TEXTURE_PATH);// AT_FRAME 용  
	void SetTextureColorKey(const string& strName,
		unsigned char r, unsigned char g, unsigned char b);
	bool AddClipCoord(const string& strName, const vector<ClipCoord>& vecCoord);// ★ 이미지 파일이 아틀라스 인 경우
	bool AddClipCoord(const string& strName, const ClipCoord& tCoord,// ★ 이미지 파일이 프레임단위로 여러개 있는 경우 
		int iCount);
	void SetDefaultClip(const string& strName);
	void SetCurrentClip(const string& strName);
	void ChangeClip(const string& strName);
	PAnimationClip FindClip(const string& strName);
	void ClearOptionTime();
	void AddRefAllClipTexture(); // ★
	void SetStopFrame(const string& strName, int iStopFrame); // ★

public:
	void Start();
	bool Init();
	void Update(float fTime);
	CAnimation* Clone()	const;
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

