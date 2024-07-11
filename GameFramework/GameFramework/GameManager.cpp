#include "GameManager.h"
#include "Input.h"
#include "Timer.h"
#include "Player.h"
#include "SceneManager.h"
#include "PathManager.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "CollisionManager.h"
#include "Camera.h"
#include "SoundManager.h"

// CGameManager* CGameManager::m_pInst = nullptr;
DEFINITION_SINGLE(CGameManager)

// 전역변수 설정 
bool CGameManager::m_bLoop = true;

CGameManager::CGameManager()
{
	// 디버그모드 (번호설정)
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(5325);
#endif // _DEBUG

	m_pTimer = nullptr;
	m_pBackBuffer = nullptr;
	m_fTimeScale = 1.f; // ＠
}

CGameManager::~CGameManager()
{
	DESTROY_SINGLE(CSceneManager);

	DESTROY_SINGLE(CCamera);
	DESTROY_SINGLE(CInput);

	GET_SINGLE(CCollisionManager)->Clear();/*
												CCollisionManager가 충돌체 목록을 가지고 있다. CCollisionManager에서 release()를 해서 충돌체가 지워졌다면
												충돌체가 지워질 때 CCollider::~CCollider() 소멸자에서 GET_SINGLE(CCollisionManager)을 해주고 있다.
												해당 충돌체를 지워서 delete()를 해서 소멸자에 갔는데 그 안에서 다시 해당 충돌체를 부활시킴.
												결국 CCollisionManager가 지워졌는데, 새로 생성이 된다. 그래서 Clear()에서 m_ColliderList 내용을 지워준다.

												CCollisionManager가 지워지기 전에 Clear()로 안에 있는 내용들을 정리하고,
												역할이 없어졌을 때, CCollisionManager만 지운다.
										   */
	DESTROY_SINGLE(CCollisionManager);

	DESTROY_SINGLE(CSoundManager);

	SAFE_RELEASE(m_pBackBuffer);
	DESTROY_SINGLE(CResourceManager);
	DESTROY_SINGLE(CPathManager);
	SAFE_DELETE(m_pTimer);

	DeleteObject(m_hRedBrush);
	DeleteObject(m_hGreenBrush);

	DeleteObject(m_hRedPen);
	DeleteObject(m_hGreenPen);

	ReleaseDC(m_hWnd, m_hDC);
}

HINSTANCE CGameManager::GetWindowInstance() const
{
	return m_hInst;
}

HWND CGameManager::GetWindowHandle() const
{
	return m_hWnd;
}

HDC CGameManager::GetWindowDC() const
{
	return m_hDC;
}

Resolution CGameManager::GetWindowResolution() const
{
	return m_tRS;
}

float CGameManager::GetTimeScale() const
{
	return m_fTimeScale;
}

float CGameManager::GetDeltaTime() const
{
	return m_pTimer->GetDeltaTime();
}

void CGameManager::SetTimeScale(float fScale)
{
	m_fTimeScale = fScale;
}

bool CGameManager::Init(HINSTANCE hInst)
{
	/* 
	bin
	binObj
	GameFramework
	Res
	TP
	*/

	// 윈도우 인스턴스 
	m_hInst = hInst;
	/*
		const char* pText = "aaa";
		 L"" 처럼 앞에 L을 붙여주면 2바이트 문자열이 된다.
		const wchar_t* pTextW = L"abc";
	*/
	RegisterWindowClass(TEXT("GameFramework"));

	// 윈도우 창을 생성한다.
	Create(TEXT("GameFramework"), TEXT("GameFramework"));

	m_hDC = GetDC(m_hWnd);

	// Brush를 만든다.
	m_hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
	m_hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));

	// Pen을 만든다.
	m_hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	m_hGreenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));

	// 타이머를 초기화한다.
	m_pTimer = new CTimer;
	m_pTimer->Init();

	// 경로관리자 초기화
	if (!GET_SINGLE(CPathManager)->Init())
		return false;

	// 리소스 관리자 초기화
	if (!GET_SINGLE(CResourceManager)->Init())
		return false;

	// 사운드 관리자 초기화
	if (!GET_SINGLE(CSoundManager)->Init())
		return false;

	// 충돌 관리자 초기화
	if (!GET_SINGLE(CCollisionManager)->Init())
		return false;

	// 입력관리자를 초기화한다.
	if (!GET_SINGLE(CInput)->Init(m_hWnd))
		return false;

	// 사용할 키를 등록한다. 모두 활성화 한다.  
	//★ 버튼설정 : 플레이어의 이동, 대쉬, 공격, 특수스킬 
	AddInput();

	// ★ 백버퍼를 얻어온다.
	m_pBackBuffer = GET_SINGLE(CResourceManager)->FindTexture("BackBuffer");
	// 사용후 릴리즈 한다. 
	m_pBackBuffer->Release();

	// 카메라의 해상도를 세팅하는 순간 -> 장면사이즈를 세팅해주면 좌상단이 (0,0)으로 지정된다. 
	GET_SINGLE(CCamera)->SetResolution(m_tRS.iWidth, m_tRS.iHeight);
	GET_SINGLE(CCamera)->SetSceneSize(m_tRS.iWidth, m_tRS.iHeight);

	// 장면관리자 초기화
	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	return true;
}

int CGameManager::Run()
{
	MSG msg;

	// 기본 메시지 루프입니다:
	while (m_bLoop)
	{
		// PeekMessage 메세지를 꺼내오면 true, 메세지가 비어있을 경우 false 
		// PM_REMOVE = 0x0001 메세지큐의 메세지를 지워준다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// 윈도우 데드타임일때 else로 들어온다.
		else
		{
			// 여기에 게임 로직을 구현한다.
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CGameManager::Logic()
{
	/*
		프레임을 계산하고 적용한다. GlobalTime으로 컨트롤 한다
		m_fTimeScale = 1
		Update에서 계산된 m_fDeltaTime이 0이면 -> 일시정지
		프레임 전환 속도가 느리면(틱 간의 길이를 넘어가는 속도) -> 전체 슬로우
	*/
	float	fDeltaTime = m_pTimer->Update() * m_fTimeScale;

	// 사운드를 업데이트 한다. 
	GET_SINGLE(CSoundManager)->Update(fDeltaTime);

	// 전체 입력에 대한 업데이트를 한다.
	GET_SINGLE(CInput)->Update(fDeltaTime);

	// 장면을 전환한다. 
	if (Update(fDeltaTime) == SC_CHANGE)
		return;
	// 장면을 전환한다. 
	if (LateUpdate(fDeltaTime) == SC_CHANGE)
		return;

	// 카메라를 업데이트 한다. 
	// ※ Update는 타겟을 지정하고 카메라가 타겟을 따라 다니게 한다. 
	GET_SINGLE(CCamera)->Update(fDeltaTime);

	// 델타 타임을 적용한다. 
	Collision(fDeltaTime);
	Render(fDeltaTime);
}

int CGameManager::Update(float fTime)
{
	// ☆ 이미 생성된 장면 관리자 객체가 관리하는 Update를 실행한다. 
	SCENE_CHANGE sc = (SCENE_CHANGE)GET_SINGLE(CSceneManager)->Update(fTime);

	return sc;
}

int CGameManager::LateUpdate(float fTime)
{
	// ☆ 이미 생성된 장면 관리자 객체가 관리하는 LateUpdate를 실행한다. 
	SCENE_CHANGE sc = (SCENE_CHANGE)GET_SINGLE(CSceneManager)->LateUpdate(fTime);

	return sc;
}

int CGameManager::Collision(float fTime)
{
	// ☆ 이미 생성된 장면 관리자 객체가 관리하는 Collision를 실행한다. 
	GET_SINGLE(CCollisionManager)->Collision(fTime);

	return 0;
}

// 백버퍼를 이용해 한번에 그린다. : 더블버퍼링 
void CGameManager::Render(float fTime) // fDeltaTime을 적용
{
	// 화면을 전체를 다 덮는 사각형을 만든다. 시작전에 덮어버리고 그 위에 그려버리는 효과 
	// 사각형 하나를 그린다. 
	// 백버퍼에서 출력정보를 가져온다.
	Rectangle(m_pBackBuffer->GetDC(), 0, 0, m_tRS.iWidth, m_tRS.iHeight);

	// 사각형에 백버퍼의 정보를 활용해 화면에 그린다. 
	GET_SINGLE(CSceneManager)->PrevRender(m_pBackBuffer->GetDC(), fTime);
	GET_SINGLE(CSceneManager)->Render(m_pBackBuffer->GetDC(), fTime);
	GET_SINGLE(CSceneManager)->PostRender(m_pBackBuffer->GetDC(), fTime);

	SetBkMode(m_pBackBuffer->GetDC(), TRANSPARENT);
	TextOut(m_pBackBuffer->GetDC(), 640, 360, TEXT("Test"), 4);

	// 가장 마지막에 마우스를 출력한다.
	GET_SINGLE(CInput)->Render(m_pBackBuffer->GetDC(), fTime);

	// 최종적으로 완성된 백버퍼를 화면에 그려낸다.
	m_pBackBuffer->Render(m_hDC, 0, 0, 0, 0, m_tRS.iWidth, m_tRS.iHeight);
}

void CGameManager::TestRender(float fTime)
{
	TextOut(m_hDC, 0, 0, TEXT("aa"), lstrlen(TEXT("aa")));
	TextOut(m_hDC, 100, 100, TEXT("게임만들자"), lstrlen(TEXT("게임만들자")));

	Rectangle(m_hDC, 200, 200, 300, 300); // 사각형

	Ellipse(m_hDC, 200, 200, 300, 300); // 구

	// 직선의 시작점을 정의한다.
	MoveToEx(m_hDC, 300, 200, nullptr);

	// 직선의 끝점을 정의한다.
	LineTo(m_hDC, 400, 300);

	// 마지막에 지정된 점을 시작점으로 하여 이 점을
	// 끝점으로한 직선이 그려진다.
	LineTo(m_hDC, 500, 300);

	// 테스트 
	MoveToEx(m_hDC, 200, 400, nullptr);
	LineTo(m_hDC, 300, 500);
}

// ★ 사용할 키를 등록한다.
// 버튼설정 : 플레이어의 이동, 대쉬, 공격, 특수스킬 
void CGameManager::AddInput() // 키를 활성화 한다. 
{
	// 상하 
	GET_SINGLE(CInput)->AddAxisKey("MoveFront", 'W', -1.f);
	GET_SINGLE(CInput)->AddAxisKey("MoveFront", 'S', 1.f);
	GET_SINGLE(CInput)->AddAxisKey("Move", 'S', -1.f, ST_CTRL | ST_ALT);

	// 좌우 
	GET_SINGLE(CInput)->AddAxisKey("MoveSide", 'A', -1.f);
	GET_SINGLE(CInput)->AddAxisKey("MoveSide", 'D', 1.f);

	GET_SINGLE(CInput)->AddActionKey("Dash", 'D');

	GET_SINGLE(CInput)->AddActionKey("Attack", VK_SPACE);

	GET_SINGLE(CInput)->AddActionKey("Skill1", '1', ST_CTRL);
	GET_SINGLE(CInput)->AddActionKey("Skill2", '2', ST_CTRL);
}

ATOM CGameManager::RegisterWindowClass(const TCHAR* pClass)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = CGameManager::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInst;
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDC_ICON));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;// MAKEINTRESOURCEW(IDC_MY190701);
	wcex.lpszClassName = pClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDC_ICON));

	return RegisterClassExW(&wcex);
}

bool CGameManager::Create(const TCHAR* pClass, const TCHAR* pTitle)
{
	m_hWnd = CreateWindowW(pClass, pTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 1280, 720, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return false;
	}

	// ★ 전체 해상도 결정 
	m_tRS.iWidth = 1280;
	m_tRS.iHeight = 720;

	// WIN32 사각형구조체를 지정한다. 
	RECT	rc = { 0, 0, 1280, 720 };

	// 윈도우 창 영역을 얻어오는 함수이다.
	// 지정한 사각형 구조체를 적용해서 창을 활성화 한다. 
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER);

	// 게임 창을 보여준다.
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

LRESULT CGameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY: // 데드타임
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
