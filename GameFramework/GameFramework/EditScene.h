#pragma once
#include "Scene.h"

enum EDIT_MODE
{
	EM_NONE = -1,
	EM_IMAGE,
	EM_OPTION,
	EM_END
};

class CEditScene :
	public CScene
{
public:
	CEditScene();
	~CEditScene();

private:
	EDIT_MODE	m_eMode;
	TILE_OPTION	m_eOption;
	class CInputComponent* m_pInputComponent;
	class CStage* m_pStage;

public:
	void ModeChangeNext(float fTime);
	void ModeChangePrev(float fTime);
	void OptionChangeNext(float fTime);
	void OptionChangePrev(float fTime);
	void Save(float fTime);
	void Load(float fTime);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float fTime);
	virtual void Render(HDC hDC, float fTime);
};

