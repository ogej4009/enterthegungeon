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
	unordered_map<string, PAnimationClip>	m_mapClip; //  ���� ���� Ŭ�������� ��Ƴ��� ��ü�ϴ� ��� 
	PAnimationClip		m_pClip; // ������ Ŭ������
	class CObj* m_pObj; // ��� �ִϸ��̼��� �ڽ��� ���� Obj�� �ȴ�. 
	string		m_strDefaultClip; // ����Ʈ Ŭ������ 
	bool		m_bAuto;// �ִϸ��̼��� ������Ʈ�� �������� ������ ���� 

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
		const TCHAR* pFileName, const string& strPathName = TEXTURE_PATH);// AT_ATLAS ��
	bool SetClipTexture(const string& strClipName, const string& strTexName,
		const vector<const TCHAR*>& vecFileName, const string& strPathName = TEXTURE_PATH);// AT_FRAME ��  
	void SetTextureColorKey(const string& strName,
		unsigned char r, unsigned char g, unsigned char b);
	bool AddClipCoord(const string& strName, const vector<ClipCoord>& vecCoord);// �� �̹��� ������ ��Ʋ�� �� ���
	bool AddClipCoord(const string& strName, const ClipCoord& tCoord,// �� �̹��� ������ �����Ӵ����� ������ �ִ� ��� 
		int iCount);
	void SetDefaultClip(const string& strName);
	void SetCurrentClip(const string& strName);
	void ChangeClip(const string& strName);
	PAnimationClip FindClip(const string& strName);
	void ClearOptionTime();
	void AddRefAllClipTexture(); // ��
	void SetStopFrame(const string& strName, int iStopFrame); // ��

public:
	void Start();
	bool Init();
	void Update(float fTime);
	CAnimation* Clone()	const;
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

