#pragma once
#include "Ref.h"


class CScene :
	public CRef
{
	friend class CSceneManager;
	friend class CObj;
	//  생성자, 완전가상소멸자 
protected:
	CScene();
	virtual ~CScene() = 0;

	// STL
protected:
	list<class CLayer*>* m_pLayerList;
	list<class CObj*>		m_ObjList;
	list<class CObj*>		m_WorldObjList;
	unordered_map<string, class CObj*>	m_mapPrototype;
	Size	m_tSceneSize;
	class CStage* m_pStage;

public:
	Size GetSceneSize()	const
	{
		return m_tSceneSize;
	}

	void SetStage(class CStage* pStage)
	{
		m_pStage = pStage;
	}

	class CStage* GetStage()	const
	{
		return m_pStage;
	}

	// 프로토타입
private:
	void AddPrototype(class CObj* pObj);
	class CObj* ClonePrototype(const string& strProtoName,
		const string& strName);
	class CObj* FindPrototype(const string& strName);
	bool DeletePrototype(const string& strName);

	// 씬에서 묶어서 관리하고 있는 정보들 
public:
	void SetLayerList(list<class CLayer*>* pLayerList);
	void AddObject(class CObj* pObj);
	void DeleteObject(class CObj* pObj);
	class CObj* FindObject(const string& strTag);

	// 월드상에 배치해주는 함수 
protected:
	void SetWorld();

	// 게임진행에 필요한 함수 
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);

};

