#pragma once
#include "Ref.h"
#include "Scene.h"
#include "SerialNumber.h"


class CObj :
	public CRef
{
protected:
	CObj();
	CObj(const CObj& obj);
	virtual ~CObj() = 0;

private:
	static CSerialNumber	m_SerialNumber;

protected:
	string		m_strLayerName;
	CScene* m_pScene;
	bool		m_bIsUI;
	unsigned int	m_iSerialNumber;

public:
	bool IsUI()	const
	{
		return m_bIsUI;
	}

	CScene* GetScene()	const
	{
		return m_pScene;
	}

protected:
	Position	m_tPos;
	Position	m_tPrevPos;
	Position	m_tPivot;
	Position	m_tMove;
	Size		m_tSize;
	Size		m_tTileColSize;
	float		m_fAngle;
	float		m_fMoveSpeed;
	class CTexture* m_pTexture;
	list<class CCollider*>	m_ColliderList;
	class CAnimation* m_pAnimation;
	bool		m_bGravity;
	float		m_fGravityTime;
	float		m_fGravityForce;
	bool		m_bCameraEnable;

public:
	void DisEnableCamera()
	{
		m_bCameraEnable = false;
	}

	void SetPos(float x, float y)
	{
		m_tPos.x = x;
		m_tPos.y = y;
	}

	void SetPos(const Position& tPos)
	{
		m_tPos = tPos;
	}

	void SetPivot(float x, float y)
	{
		m_tPivot.x = x;
		m_tPivot.y = y;
	}

	void SetPivot(const Position& tPivot)
	{
		m_tPivot = tPivot;
	}

	void SetSize(float x, float y)
	{
		m_tSize.x = x;
		m_tSize.y = y;
	}

	void SetSize(const Size& tSize)
	{
		m_tSize = tSize;
	}

	void SetTileColSize(float x, float y)
	{
		m_tTileColSize.x = x;
		m_tTileColSize.y = y;
	}

	void SetTileColSize(const Size& tSize)
	{
		m_tTileColSize = tSize;
	}

	void SetAngle(float fAngle)
	{
		m_fAngle = fAngle;
	}

	void UseGravity(bool bGravity)
	{
		m_bGravity = bGravity;
	}

	void SetGravityForce(float fForce)
	{
		m_fGravityForce = fForce;
	}

public:
	Position GetPos()	const
	{
		return m_tPos;
	}

	Position GetPivot()	const
	{
		return m_tPivot;
	}

	Position GetPrevPos()	const
	{
		return m_tPrevPos;
	}

	Position GetMove()	const
	{
		return m_tPos - m_tPrevPos;
	}

	Position GetSize()	const
	{
		return m_tSize;
	}

	Size GetTileColSize()	const
	{
		return m_tTileColSize;
	}

	float GetAngle()	const
	{
		return m_fAngle;
	}

	class CAnimation* GetAnimation()	const
	{
		return m_pAnimation;
	}

public:
	void SetLayerName(const string& strName)
	{
		m_strLayerName = strName;
	}

	void SetScene(CScene* pScene)
	{
		m_pScene = pScene;
	}

	string GetLayerName()	const
	{
		return m_strLayerName;
	}

	void SetMoveSpeed(float fSpeed)
	{
		m_fMoveSpeed = fSpeed;
	}

	float GetMoveSpeed()	const
	{
		return m_fMoveSpeed;
	}

	float GetGravityForce()	const
	{
		return m_fGravityForce;
	}

	void ClearGravity()
	{
		m_fGravityTime = 0.f;
		m_fGravityForce = 0.f;
	}

	void UpdatePos(const Position& tPos);

public:
	void SetTexture(const string& strName, const TCHAR* pFileName = nullptr,
		const string& strPathName = TEXTURE_PATH);
	void SetTexture(class CTexture* pTexture);
	void SetColorKey(unsigned char r, unsigned char g, unsigned char b);
	CObj* FindObject(const string& strName);
	void CreateAnimation(const string& strName);
	void AddRefAllClipTexture(); // ¡Ú
	void AddRefTexture(); // ¡Ú

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CObj* Clone()	const;
	virtual void SaveFromFullPath(const char* pFullPath);
	virtual void SaveFromFileName(const char* pFileName, const string& strPath = DATA_PATH);
	virtual void Save(FILE* pFile);
	virtual void LoadFromFullPath(const char* pFullPath);
	virtual void LoadFromFileName(const char* pFileName, const string& strPath = DATA_PATH);
	virtual void Load(FILE* pFile);

public:
	void Move(const Position& tDir);
	void Move(const Position& tDir, float fTime);
	void Move(DIR eDir);
	void Move(DIR eDir, float fTime);
	void MoveAngle();
	void MoveAngle(float fTime);

public:
	CCollider* FindCollider(const string& strName);

public:
	template <typename T>
	static T* CreateObject(const string& strTag, const string& strLayerName,
		CScene* pScene)
	{
		CObj* pObj = new T;

		pObj->SetLayerName(strLayerName);
		pObj->SetTag(strTag);
		pObj->m_iSerialNumber = m_SerialNumber.CreateSerialNumber();

		if (!pObj->Init())
		{
			delete	pObj;
			return nullptr;
		}

		if (pScene)
			pScene->AddObject(pObj);

		return (T*)pObj;
	}

	template <typename T>
	static CObj* CreatePrototype(const string& strTag,
		CScene* pScene)
	{
		CObj* pObj = new T;

		pObj->SetTag(strTag);

		if (!pObj->Init())
		{
			delete	pObj;
			return nullptr;
		}

		//pScene->AddObject(pObj);
		pScene->AddPrototype(pObj);

		return pObj;
	}

	template <typename T>
	static CObj* CreateCloneObject(const string& strPrototypeName,
		const string& strTag, const string& strLayerName,
		CScene* pScene)
	{
		CObj* pObj = pScene->ClonePrototype(strPrototypeName, strTag);

		if (!pObj)
			return nullptr;

		pObj->SetLayerName(strLayerName);
		pObj->m_iSerialNumber = m_SerialNumber.CreateSerialNumber();

		pScene->AddObject(pObj);

		return pObj;
	}

	template <typename T>
	T* AddCollider(const string& strName)
	{
		T* pCollider = new T;

		pCollider->SetTag(strName);
		pCollider->m_pObj = this;

		pCollider->Init();
		pCollider->SetScene(m_pScene);

		if (m_bIsUI)
			pCollider->EnableUI();

		pCollider->AddRef();
		m_ColliderList.push_back(pCollider);

		return pCollider;
	}
};

