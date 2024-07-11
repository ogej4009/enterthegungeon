#include "Tile.h"
#include "ColliderRect.h"
#include "Animation.h"
#include "Texture.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "Camera.h"

CTile::CTile()
{
	m_eOption = TO_NORMAL;
}

CTile::CTile(const CTile& obj) :
	CObj(obj)
{
}

CTile::~CTile()
{
}

void CTile::Start()
{
	//CObj::Start();
}

bool CTile::Init()
{
	if (!CObj::Init())
		return false;

	return true;
}

void CTile::Update(float fTime)
{
	CObj::Update(fTime);
}

void CTile::LateUpdate(float fTime)
{
	CObj::LateUpdate(fTime);
}

void CTile::Collision(float fTime)
{
	CObj::Collision(fTime);
}

void CTile::PrevRender(HDC hDC, float fTime)
{
	CObj::PrevRender(hDC, fTime);
}

void CTile::Render(HDC hDC, float fTime)
{
	CObj::Render(hDC, fTime);

#ifdef _DEBUG

	// 옵션이 어떤 것인지에 따라 타일을 그려준다. 
	HBRUSH	hBrush = GREENBRUSH;

	switch (m_eOption)
	{
	case TO_WALL:
		hBrush = REDBRUSH;
		break;
	}

	RECT	tRC = { m_tPos.x, m_tPos.y, m_tPos.x + m_tSize.x, m_tPos.y + m_tSize.y };

	// 편집모드에서 타일의 위치에 영향을 준다. 
	// 카메라의 위치 적용을 받는다. 
	tRC.left -= GET_SINGLE(CCamera)->GetPos().x;
	tRC.top -= GET_SINGLE(CCamera)->GetPos().y;
	tRC.right -= GET_SINGLE(CCamera)->GetPos().x;
	tRC.bottom -= GET_SINGLE(CCamera)->GetPos().y;

	FrameRect(hDC, &tRC, hBrush);
#endif // _DEBUG

}

void CTile::PostRender(HDC hDC, float fTime)
{
	CObj::PostRender(hDC, fTime);
}

CTile* CTile::Clone() const
{
	return new CTile(*this);
}

void CTile::Save(FILE* pFile)
{
	CObj::Save(pFile);

	fwrite(&m_eOption, sizeof(TILE_OPTION), 1, pFile);
}

void CTile::Load(FILE* pFile)
{
	CObj::Load(pFile);

	fread(&m_eOption, sizeof(TILE_OPTION), 1, pFile);
}
