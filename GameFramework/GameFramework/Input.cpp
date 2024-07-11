#include "Input.h"
#include "UIImage.h"
#include "ColliderPoint.h"
#include "Animation.h"


// 어디서든 손쉽게 쓸수 있어야 한다. 
// CInput* CInput::m_pInst = nullptr;
DEFINITION_SINGLE(CInput)

CInput::CInput() :
	// ＠ 다른 클래스의 정보 초기화 
	m_pMouseAnimation(nullptr),
	m_pMouseWorldCollider(nullptr),
	m_pMouseUICollider(nullptr),
	m_pMouseObj(nullptr)
{
}

CInput::~CInput()
{
	SAFE_RELEASE(m_pMouseAnimation);
	SAFE_RELEASE(m_pMouseWorldCollider);
	SAFE_RELEASE(m_pMouseUICollider);
	SAFE_RELEASE(m_pMouseObj);

	// ＠ STL을 한번에 지우는 방법 
	Safe_Delete_Map(m_mapBindAxis);
	Safe_Delete_Map(m_mapBindAction);
}

void CInput::CreateMouse() // 마우스 객체 생성 
{
	// ★☆ 마우스 객체를 생성한다. CObj > CUI > CUIImage : CObj* pObj = new CUIImage; 
	m_pMouseObj = CObj::CreateObject<CUIImage>("Mouse", "", nullptr); // 어떤 레이어도 속하지 않는다. 
	m_pMouseObj->SetSize(32.f, 31.f);

	// 애니메이션 설정 
	m_pMouseObj->CreateAnimation("MouseAnimation");
	m_pMouseAnimation = m_pMouseObj->GetAnimation();
	m_pMouseAnimation->AddClip("MouseDefault", AO_LOOP, 1.f);

	// ☆★ 이미지 파일저장 방법 
	vector<const TCHAR*>	vecFileName;
	for (int i = 0; i <= 10; ++i)
	{
		TCHAR* pFileName = new TCHAR[MAX_PATH];
		memset(pFileName, 0, sizeof(TCHAR) * MAX_PATH);
		wsprintf(pFileName, TEXT("Mouse/%d.bmp"), i);
		vecFileName.push_back(pFileName);
	}

	m_pMouseAnimation->SetClipTexture("MouseDefault", "MouseDefault", vecFileName);
	Safe_Delete_Array_VecList(vecFileName);

	ClipCoord	tCoord = {};
	tCoord.tEnd.x = 32.f;
	tCoord.tEnd.y = 31.f;

	m_pMouseAnimation->AddClipCoord("MouseDefault", tCoord, 11);
	m_pMouseAnimation->SetTextureColorKey("MouseDefault", 255, 0, 255);

	// ★ 마우스 충돌체 생성(월드)
	m_pMouseWorldCollider = m_pMouseObj->AddCollider<CColliderPoint>("MouseWorld");
	m_pMouseWorldCollider->SetProfile("Mouse");
	m_pMouseWorldCollider->SetChannel("Mouse");
	m_pMouseWorldCollider->EnableUI();
	/*
		콜백함수 :
		pBody->SetCollisionCallback<CPlayer>(CS_BLOCK, this, &CPlayer::Hit);
	*/
	// ★ 마우스 충돌체 생성(UI)
	m_pMouseUICollider = m_pMouseObj->AddCollider<CColliderPoint>("MouseUI");
	m_pMouseUICollider->EnableUI();
	m_pMouseUICollider->SetProfile("Mouse");
	m_pMouseUICollider->SetChannel("Mouse");

	m_pMouseObj->Start();
}

bool CInput::Init(HWND hWnd)
{
	// 핸들 윈도우 인스턴스 
	m_hWnd = hWnd;

	// 동적배열 미리 세팅 
	m_KeyList.resize(256);

	// 고유 키보드 번호 지정 
	for (int i = 0; i < 256; ++i)
	{
		m_KeyList[i].cKey = (unsigned char)i;
	}

	// 고유 스페셜 키 번호 지정 
	m_tSKey[SKEY_CTRL].cKey = VK_CONTROL;
	m_tSKey[SKEY_ALT].cKey = VK_MENU;
	m_tSKey[SKEY_SHIFT].cKey = VK_SHIFT;

	// 마우스 커서 현재, 이전, 이동량 좌표 위치 저장공간 주소정보 
	memset(&m_tMousePos, 0, sizeof(POINT));
	memset(&m_tMousePrevPos, 0, sizeof(POINT));
	memset(&m_tMouseMove, 0, sizeof(POINT));

	// 실시간 마우스 정보 
	POINT	ptMousePos;

	// 이 함수는 스크린 좌표 기준으로 마우스 위치를 얻어온다.
	GetCursorPos(&ptMousePos);

	// 마우스 위치를 클라이언트 기준으로 변환한다.
	ScreenToClient(m_hWnd, &ptMousePos);

	// 마우스의 좌표, 이동량 계산  이전 <- 현재 
	m_tMousePos.x = ptMousePos.x;
	m_tMousePos.y = ptMousePos.y;
	m_tMousePrevPos = m_tMousePos;

	// 마우스 객체 생성 
	CreateMouse();

	// 마우스 커서 화면에서 비활성화 
	ShowCursor(FALSE);

	// ☆★ 마우스 키 번호 지정 
	AddActionKey("LButton", VK_LBUTTON);
	AddActionKey("RButton", VK_RBUTTON);

	// ☆★ 
	BindAction<CInput>("LButton", IE_PRESS, this, &CInput::LButtonDown, "LButtonDown");
	BindAction<CInput>("LButton", IE_HOLD, this, &CInput::LButtonHold, "LButtonHold");
	BindAction<CInput>("LButton", IE_UP, this, &CInput::LButtonUp, "LButtonUp");
	BindAction<CInput>("LButton", IE_DOUBLE, this, &CInput::LButtonDouble, "LButtonDouble");

	BindAction<CInput>("RButton", IE_PRESS, this, &CInput::RButtonDown, "RButtonDown");
	BindAction<CInput>("RButton", IE_HOLD, this, &CInput::RButtonHold, "RButtonHold");
	BindAction<CInput>("RButton", IE_UP, this, &CInput::RButtonUp, "RButtonUp");
	BindAction<CInput>("RButton", IE_DOUBLE, this, &CInput::RButtonDouble, "RButtonDouble");

	return true;
}

void CInput::Update(float fTime)
{
	MouseUpdate(fTime);

	SKeyInput();
	KeyInput();

	AxisInput(fTime);
	ActionInput(fTime);
}

void CInput::Render(HDC hDC, float fTime)
{
	// 마우스 포인터를 그려준다. 
	m_pMouseObj->Render(hDC, fTime);
}

void CInput::ChangeScene()
{
	m_pMouseObj->AddRefAllClipTexture();
	// ★ 마우스obj가 가지고 있는 텍스쳐의 크기를 늘려준다. 
	// 결과적으로 마우스의 클립 텍스쳐들 add처리를 해주어야 한다. 
	// 장면을 2,3번 넘어갔을 때, 마우스 텍스쳐가 릴리즈가 되지 않도록 막아준다. 
}

void CInput::MouseUpdate(float fTime)
{
	m_tMousePrevPos = m_tMousePos;

	POINT	ptMousePos;

	// 이 함수는 스크린 좌표 기준으로 마우스 위치를 얻어온다.
	GetCursorPos(&ptMousePos);

	// 마우스 위치를 클라이언트 기준으로 변환한다.
	ScreenToClient(m_hWnd, &ptMousePos);

	m_tMousePos.x = ptMousePos.x;
	m_tMousePos.y = ptMousePos.y;

	m_tMouseMove.x = m_tMousePos.x - m_tMousePrevPos.x;
	m_tMouseMove.y = m_tMousePos.y - m_tMousePrevPos.y;

	m_pMouseObj->SetPos(m_tMousePos.x, m_tMousePos.y);

	m_pMouseObj->Update(fTime);
	m_pMouseObj->LateUpdate(fTime);

	m_tLButtonInfo.tPos = m_tMousePos;
	m_tLButtonInfo.tMove = m_tMouseMove;
	m_tLButtonInfo.tPrevPos = m_tMousePrevPos;

	m_tRButtonInfo.tPos = m_tMousePos;
	m_tRButtonInfo.tMove = m_tMouseMove;
	m_tRButtonInfo.tPrevPos = m_tMousePrevPos;

	m_tLButtonInfo.bDown = false;
	m_tLButtonInfo.bHold = false;
	m_tLButtonInfo.bUp = false;
	m_tLButtonInfo.bDouble = false;

	m_tRButtonInfo.bDown = false;
	m_tRButtonInfo.bHold = false;
	m_tRButtonInfo.bUp = false;
	m_tRButtonInfo.bDouble = false;
}

void CInput::SKeyInput()
{
	for (int i = 0; i < SKEY_END; ++i)
	{
		// 등록된 키를 눌렀는지 판단한다.
		if (GetAsyncKeyState(m_tSKey[i].cKey) & 0x8000)
		{
			// 처음 이 키를 누를 경우
			if (!m_tSKey[i].bDown && !m_tSKey[i].bHold)
			{
				m_tSKey[i].bDown = true;
				m_tSKey[i].bHold = true;
			}

			// 키를 계속 누르고 있을 경우
			else if (m_tSKey[i].bDown)
			{
				m_tSKey[i].bDown = false;
			}
		}

		// 처음 누른 것이 아닌경우 
		else if (m_tSKey[i].bDown || m_tSKey[i].bHold)
		{
			m_tSKey[i].bDown = false;
			m_tSKey[i].bHold = false;
			m_tSKey[i].bUp = true;
		}

		else if (m_tSKey[i].bUp)
			m_tSKey[i].bUp = false;
	}
}

void CInput::KeyInput() /*
							내가 원하는 곳에서 input을 호출해 눌렀는지 안눌렀는지 확인을 한다.
							함수포인터를 활용하여 어떤 키를 눌렀을 때 호출될 함수를 지정을 해준다.
							동작할 함수를 만들어놓고, 그 키를 눌렀을 때 그 함수가 동작이 된다.
						*/
{
	// 등록된 키의 상태를 판단한다.
	for (int i = 0; i < 256; ++i)
	{
		if (!m_KeyList[i].bEnable)
			continue;

		// 등록된 키를 눌렀는지 판단한다.
		if (GetAsyncKeyState(m_KeyList[i].cKey) & 0x8000)
		{
			// 처음 이 키를 누를 경우
			if (!m_KeyList[i].bDown && !m_KeyList[i].bHold)
			{
				m_KeyList[i].bDown = true;
				m_KeyList[i].bHold = true;
			}

			// 키를 계속 누르고 있을 경우
			else if (m_KeyList[i].bDown)
			{
				m_KeyList[i].bDown = false;
			}
		}

		// 처음 누른 것이 아닌 경우 
		else if (m_KeyList[i].bDown || m_KeyList[i].bHold)
		{
			m_KeyList[i].bDown = false;
			m_KeyList[i].bHold = false;
			m_KeyList[i].bUp = true;
		}

		else if (m_KeyList[i].bUp)
			m_KeyList[i].bUp = false;
	}
}

void CInput::AxisInput(float fTime)
{
	unordered_map<string, PBindAxisInfo>::iterator	iter;
	unordered_map<string, PBindAxisInfo>::iterator	iterEnd = m_mapBindAxis.end();

	for (iter = m_mapBindAxis.begin(); iter != iterEnd; ++iter)
	{
		list<AxisInfo>::iterator	iterKey;
		list<AxisInfo>::iterator	iterKeyEnd = iter->second->KeyList.end();

		for (iterKey = iter->second->KeyList.begin(); iterKey != iterKeyEnd; ++iterKey)
		{
			(*iterKey).tKey.bDown = m_KeyList[(*iterKey).tKey.cKey].bDown;
			(*iterKey).tKey.bHold = m_KeyList[(*iterKey).tKey.cKey].bHold;
			(*iterKey).tKey.bUp = m_KeyList[(*iterKey).tKey.cKey].bUp;

			// Ctrl, Alt, Shift 조합키에 대해 통과했는지
			// 판단한다.
			bool	bSKey = true;

			for (int i = 0; i < SKEY_END; ++i)
			{
				// 해당 SKey를 눌러야 된다는 것이다.
				if ((*iterKey).tKey.bSKey[i])
				{
					// 키를 눌러야 하는데 안눌렀을 경우
					// bSKey를 false로 만들고 for문을
					// 종료한다.
					if (!m_tSKey[i].bHold)
					{
						bSKey = false;
						break;
					}
				}

				else
				{
					if (m_tSKey[i].bHold)
					{
						bSKey = false;
						break;
					}
				}
			}

			list<KeyFunction>::iterator	iterF;
			list<KeyFunction>::iterator	iterFEnd = iter->second->FunctionList.end();

			if (bSKey && (*iterKey).tKey.bDown)
			{
			}

			if (bSKey && (*iterKey).tKey.bHold)
			{
				for (iterF = iter->second->FunctionList.begin(); iterF != iterFEnd; ++iterF)
				{
					(*iterF).func((*iterKey).fScale, fTime);
				}
			}

			if (bSKey && (*iterKey).tKey.bUp)
			{
			}
		}
	}
}

void CInput::ActionInput(float fTime)
{
	unordered_map<string, PBindActionInfo>::iterator	iter;
	unordered_map<string, PBindActionInfo>::iterator	iterEnd = m_mapBindAction.end();

	for (iter = m_mapBindAction.begin(); iter != iterEnd; ++iter)
	{
		list<KeyInfo>::iterator	iterKey;
		list<KeyInfo>::iterator	iterKeyEnd = iter->second->KeyList.end();

		for (iterKey = iter->second->KeyList.begin(); iterKey != iterKeyEnd; ++iterKey)
		{
			(*iterKey).bDown = m_KeyList[(*iterKey).cKey].bDown;
			(*iterKey).bHold = m_KeyList[(*iterKey).cKey].bHold;
			(*iterKey).bUp = m_KeyList[(*iterKey).cKey].bUp;

			// Ctrl, Alt, Shift 조합키에 대해 통과했는지
			// 판단한다.
			bool	bSKey = true;

			for (int i = 0; i < SKEY_END; ++i)
			{
				// 해당 SKey를 눌러야 된다는 것이다.
				if ((*iterKey).bSKey[i])
				{
					// 키를 눌러야 하는데 안눌렀을 경우
					// bSKey를 false로 만들고 for문을
					// 종료한다.
					if (!m_tSKey[i].bHold)
					{
						bSKey = false;
						break;
					}
				}

				else
				{
					if (m_tSKey[i].bHold)
					{
						bSKey = false;
						break;
					}
				}
			}

			list<ActionKeyFunction>::iterator	iterF;
			list<ActionKeyFunction>::iterator	iterFEnd = iter->second->FunctionList.end();

			for (iterF = iter->second->FunctionList.begin();
				iterF != iterFEnd; ++iterF)
			{
				switch ((*iterF).eEvent)
				{
				case IE_PRESS:
					if (bSKey && (*iterKey).bDown)
						(*iterF).func(fTime);
					break;
				case IE_HOLD:
					if (bSKey && (*iterKey).bHold)
						(*iterF).func(fTime);
					break;
				case IE_UP:
					if (bSKey && (*iterKey).bUp)
						(*iterF).func(fTime);
					break;
				case IE_DOUBLE:
					if (iter->second->bDoubleStart)
					{
						iter->second->fDoubleTime += fTime;

						if (iter->second->fDoubleTime > 0.5f) // ★ 더블클릭 제한시간 설정 
						{
							iter->second->bDoubleStart = false;
							iter->second->fDoubleTime = 0.f;
						}
					}

					if (bSKey && (*iterKey).bDown)
					{
						if (!iter->second->bDoubleStart)
						{
							iter->second->bDoubleStart = true;
						}

						else
						{
							(*iterF).func(fTime);

							iter->second->fDoubleTime = 0.f;
							iter->second->bDoubleStart = false;
						}
					}
					break;
				}
			}
		}
	}
}

void CInput::AddAxisKey(const string& strName,
	unsigned char cKey, float fScale, int iSKey)
{
	PBindAxisInfo	pAxis = FindAxis(strName);

	// Axis가 없을 경우 하나 새로 생성해준다.
	if (!pAxis)
	{
		pAxis = new BindAxisInfo;
		pAxis->strName = strName;
		m_mapBindAxis.insert(make_pair(strName, pAxis));
	}

	list<AxisInfo>::iterator	iter;
	list<AxisInfo>::iterator	iterEnd = pAxis->KeyList.end();

	for (iter = pAxis->KeyList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter).tKey.cKey == cKey)
			return;
	}

	AxisInfo	tInfo = {};

	tInfo.fScale = fScale;
	tInfo.tKey.cKey = cKey;

	if (iSKey & ST_CTRL)
		tInfo.tKey.bSKey[SKEY_CTRL] = true;

	if (iSKey & ST_ALT)
		tInfo.tKey.bSKey[SKEY_ALT] = true;

	if (iSKey & ST_SHIFT)
		tInfo.tKey.bSKey[SKEY_SHIFT] = true;

	pAxis->KeyList.push_back(tInfo);

	m_KeyList[cKey].bEnable = true;
}

void CInput::AddActionKey(const string& strName,
	unsigned char cKey, int iSKey)
{
	PBindActionInfo	pAction = FindAction(strName);

	// Action이 없을 경우 하나 새로 생성해준다.
	if (!pAction)
	{
		pAction = new BindActionInfo;
		pAction->strName = strName;
		m_mapBindAction.insert(make_pair(strName, pAction));
	}

	list<KeyInfo>::iterator	iter;
	list<KeyInfo>::iterator	iterEnd = pAction->KeyList.end();

	for (iter = pAction->KeyList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter).cKey == cKey)
			return;
	}

	KeyInfo	tInfo = {};

	tInfo.cKey = cKey;

	if (iSKey & ST_CTRL)
		tInfo.bSKey[SKEY_CTRL] = true;

	if (iSKey & ST_ALT)
		tInfo.bSKey[SKEY_ALT] = true;

	if (iSKey & ST_SHIFT)
		tInfo.bSKey[SKEY_SHIFT] = true;

	pAction->KeyList.push_back(tInfo);

	m_KeyList[cKey].bEnable = true;
}

void CInput::LButtonDown(float fTime)
{
	m_tLButtonInfo.bHold = true;
	m_tLButtonInfo.bDown = true;
}

void CInput::LButtonHold(float fTime)
{
	m_tLButtonInfo.bHold = true;
}

void CInput::LButtonUp(float fTime)
{
	m_tLButtonInfo.bUp = true;
}

void CInput::LButtonDouble(float fTime)
{
	m_tLButtonInfo.bDouble = true;
}

void CInput::RButtonDown(float fTime)
{
	m_tRButtonInfo.bHold = true;
	m_tRButtonInfo.bDown = true;
}

void CInput::RButtonHold(float fTime)
{
	m_tRButtonInfo.bHold = true;
}

void CInput::RButtonUp(float fTime)
{
	m_tRButtonInfo.bUp = true;
}

void CInput::RButtonDouble(float fTime)
{
	m_tRButtonInfo.bDouble = true;
}

PBindAxisInfo CInput::FindAxis(const string& strName)
{
	unordered_map<string, PBindAxisInfo>::iterator	iter = m_mapBindAxis.find(strName);

	if (iter == m_mapBindAxis.end())
		return nullptr;

	return iter->second;
}

PBindActionInfo CInput::FindAction(const string& strName)
{
	unordered_map<string, PBindActionInfo>::iterator	iter = m_mapBindAction.find(strName);

	if (iter == m_mapBindAction.end())
		return nullptr;

	return iter->second;
}

bool CInput::DeleteBindAxis(const string& strName,
	void* pObj,
	const string& strFuncName)
{
	PBindAxisInfo	pAxis = FindAxis(strName);

	if (!pAxis)
		return false;

	list<KeyFunction>::iterator	iter;
	list<KeyFunction>::iterator	iterEnd = pAxis->FunctionList.end();

	for (iter = pAxis->FunctionList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter).pObj == pObj && (*iter).strFuncName == strFuncName)
		{
			pAxis->FunctionList.erase(iter);
			break;
		}
	}

	return true;
}

bool CInput::DeleteBindAction(const string& strName,
	void* pObj,
	const string& strFuncName)
{
	PBindActionInfo	pAction = FindAction(strName);

	if (!pAction)
		return false;

	list<ActionKeyFunction>::iterator	iter;
	list<ActionKeyFunction>::iterator	iterEnd = pAction->FunctionList.end();

	for (iter = pAction->FunctionList.begin(); iter != iterEnd; ++iter)
	{
		if ((*iter).pObj == pObj && (*iter).strFuncName == strFuncName)
		{
			pAction->FunctionList.erase(iter);
			break;
		}
	}

	return true;
}
