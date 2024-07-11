#pragma once
#include "Obj.h"


class CTile :
	public CObj
{
	DECLARE_OBJECT
	friend class CStage;

protected:
	CTile();
	CTile(const CTile& obj);
	~CTile();

private:
	TILE_OPTION	m_eOption;

public:
	TILE_OPTION GetTileOption()	const
	{
		return m_eOption;
	}

	void SetTileOption(TILE_OPTION eOption)
	{
		m_eOption = eOption;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void LateUpdate(float fTime);
	virtual void Collision(float fTime);
	virtual void PrevRender(HDC hDC, float fTime);
	virtual void Render(HDC hDC, float fTime);
	virtual void PostRender(HDC hDC, float fTime);
	virtual CTile* Clone()	const;
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

