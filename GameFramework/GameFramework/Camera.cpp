#include "Camera.h"
#include "Obj.h"
#include "InputComponent.h"
#include "Input.h"

// ��𼭵� �ս��� ���� �־�� �Ѵ�. 
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

	// Ÿ�� ������Ʈ�� ���۷��� ī���͸� +1
	if (m_pTarget)
		m_pTarget->AddRef();
}

void CCamera::OnFreeCamera()
{
	m_pInputComponent = new CInputComponent;

	// �� ī�޶��� �̵�Ű ��� 
	m_pInputComponent->BindAxis<CCamera>("MoveFront", this, &CCamera::MoveFront, "MoveFront");
	m_pInputComponent->BindAxis<CCamera>("MoveSide", this, &CCamera::MoveSide, "MoveSide");
}

void CCamera::MoveFront(float fScale, float fTime)
{
	// �� ī�޶��� �̵��ӵ� 
	m_tPos += Position(0.f, fScale) * 500.f * fTime;
}

void CCamera::MoveSide(float fScale, float fTime)
{
	// �� ī�޶��� �̵��ӵ� 
	m_tPos += Position(fScale, 0.f) * 500.f * fTime;
}

void CCamera::Update(float fTime)
{
	// Ÿ���� �����Ǿ� �ִ� ��� 
	if (m_pTarget)
	{
		// Ȱ��ȭ ����  
		if (!m_pTarget->IsActive())
			SAFE_RELEASE(m_pTarget);

		// �� Ÿ���� ��ġ�� ���� ī�޶��� ��ġ�� �����Ѵ�. Ÿ���� ī�޶� �������� ��ġ�� ������ �Ѵ�. 
		Position	tTargetPos = m_pTarget->GetPos();
		m_tPos.x = tTargetPos.x - m_tResolution.x * m_tPivot.x;
		m_tPos.y = tTargetPos.y - m_tResolution.y * m_tPivot.y;
	}

	// ���� ó�� 

	// x���� : �������� �Ѿ����
	if (m_tPos.x < m_tSceneStartPos.x)
		m_tPos.x = m_tSceneStartPos.x;

	// ������ ��ǥ���� Ŀ���� ��� 
	else if (m_tPos.x + m_tResolution.x > m_tSceneStartPos.x + m_tSceneSize.x)
		m_tPos.x = m_tSceneStartPos.x + m_tSceneSize.x - m_tResolution.x;

	// y���� : �������� ������ ��� 
	if (m_tPos.y < m_tSceneStartPos.y)
		m_tPos.y = m_tSceneStartPos.y;

	// ������ ������ ��� 
	else if (m_tPos.y + m_tResolution.y > m_tSceneStartPos.y + m_tSceneSize.y)
		m_tPos.y = m_tSceneStartPos.y + m_tSceneSize.y - m_tResolution.y;
}
