#pragma once
#include "Ref.h"


class CScene :
	public CRef
{
	friend class CSceneManager;
	friend class CObj;
	//  ������, ��������Ҹ��� 
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

	// ������Ÿ��
private:
	void AddPrototype(class CObj* pObj);
	class CObj* ClonePrototype(const string& strProtoName,
		const string& strName);
	class CObj* FindPrototype(const string& strName);
	bool DeletePrototype(const string& strName);

	// ������ ��� �����ϰ� �ִ� ������ 
public:
	void SetLayerList(list<class CLayer*>* pLayerList);
	void AddObject(class CObj* pObj);
	void DeleteObject(class CObj* pObj);
	class CObj* FindObject(const string& strTag);

	// ����� ��ġ���ִ� �Լ� 
protected:
	void SetWorld();

	// �������࿡ �ʿ��� �Լ� 
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

