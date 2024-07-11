#include "Camera.h"
#include "Obj.h"
#include "InputComponent.h"
#include "Input.h"

// 어디서든 손쉽게 쓸수 있어야 한다. 
//	CCamera* CCamera::m_pInst = nullptr;
DEFINITION_SINGLE(CCamera)

CCamera::CCamera() :
	m_pTarget(nullptr),
	m_pInputComponent(nullptr)
{
}

CCamera::~CCamera()
{
	SAFE_RELEASE(m_pInputComponent);
	SAFE_RELEASE(m_pTarget);
}

void CCamera::SetTarget(CObj* pTarget)
{
	SAFE_RELEASE(m_pTarget);
	m_pTarget = pTarget;

	// 타겟 오브젝트의 레퍼런스 카운터를 +1
	if (m_pTarget)
		m_pTarget->AddRef();
}

void CCamera::OnFreeCamera()
{
	m_pInputComponent = new CInputComponent;

	// ★ 카메라의 이동키 등록 
	m_pInputComponent->BindAxis<CCamera>("MoveFront", this, &CCamera::MoveFront, "MoveFront");
	m_pInputComponent->BindAxis<CCamera>("MoveSide", this, &CCamera::MoveSide, "MoveSide");
}

void CCamera::MoveFront(float fScale, float fTime)
{
	// ★ 카메라의 이동속도 
	m_tPos += Position(0.f, fScale) * 500.f * fTime;
}

void CCamera::MoveSide(float fScale, float fTime)
{
	// ★ 카메라의 이동속도 
	m_tPos += Position(fScale, 0.f) * 500.f * fTime;
}

void CCamera::Update(float fTime)
{
	// 타겟이 지정되어 있는 경우 
	if (m_pTarget)
	{
		// 활성화 실패  
		if (!m_pTarget->IsActive())
			SAFE_RELEASE(m_pTarget);

		// ★ 타겟의 위치에 따른 카메라의 위치를 설정한다. 타겟이 카메라에 의존적인 위치를 가지게 한다. 
		Position	tTargetPos = m_pTarget->GetPos();
		m_tPos.x = tTargetPos.x - m_tResolution.x * m_tPivot.x;
		m_tPos.y = tTargetPos.y - m_tResolution.y * m_tPivot.y;
	}

	// 예외 처리 

	// x기준 : 왼쪽으로 넘어가버림
	if (m_tPos.x < m_tSceneStartPos.x)
		m_tPos.x = m_tSceneStartPos.x;

	// 오른쪽 좌표보다 커버린 경우 
	else if (m_tPos.x + m_tResolution.x > m_tSceneStartPos.x + m_tSceneSize.x)
		m_tPos.x = m_tSceneStartPos.x + m_tSceneSize.x - m_tResolution.x;

	// y기준 : 위쪽으로 가버린 경우 
	if (m_tPos.y < m_tSceneStartPos.y)
		m_tPos.y = m_tSceneStartPos.y;

	// 밑으로 가버린 경우 
	else if (m_tPos.y + m_tResolution.y > m_tSceneStartPos.y + m_tSceneSize.y)
		m_tPos.y = m_tSceneStartPos.y + m_tSceneSize.y - m_tResolution.y;
}
