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

// �������� ���� 
bool CGameManager::m_bLoop = true;

CGameManager::CGameManager()
{
	// ����׸�� (��ȣ����)
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(5325);
#endif // _DEBUG

	m_pTimer = nullptr;
	m_pBackBuffer = nullptr;
	m_fTimeScale = 1.f; // ��
}

CGameManager::~CGameManager()
{
	DESTROY_SINGLE(CSceneManager);

	DESTROY_SINGLE(CCamera);
	DESTROY_SINGLE(CInput);

	GET_SINGLE(CCollisionManager)->Clear();/*
												CCollisionManager�� �浹ü ����� ������ �ִ�. CCollisionManager���� release()�� �ؼ� �浹ü�� �������ٸ�
												�浹ü�� ������ �� CCollider::~CCollider() �Ҹ��ڿ��� GET_SINGLE(CCollisionManager)�� ���ְ� �ִ�.
												�ش� �浹ü�� ������ delete()�� �ؼ� �Ҹ��ڿ� ���µ� �� �ȿ��� �ٽ� �ش� �浹ü�� ��Ȱ��Ŵ.
												�ᱹ CCollisionManager�� �������µ�, ���� ������ �ȴ�. �׷��� Clear()���� m_ColliderList ������ �����ش�.

												CCollisionManager�� �������� ���� Clear()�� �ȿ� �ִ� ������� �����ϰ�,
												������ �������� ��, CCollisionManager�� �����.
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

	// ������ �ν��Ͻ� 
	m_hInst = hInst;
	/*
		const char* pText = "aaa";
		 L"" ó�� �տ� L�� �ٿ��ָ� 2����Ʈ ���ڿ��� �ȴ�.
		const wchar_t* pTextW = L"abc";
	*/
	RegisterWindowClass(TEXT("GameFramework"));

	// ������ â�� �����Ѵ�.
	Create(TEXT("GameFramework"), TEXT("GameFramework"));

	m_hDC = GetDC(m_hWnd);

	// Brush�� �����.
	m_hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
	m_hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));

	// Pen�� �����.
	m_hRedPen = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	m_hGreenPen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));

	// Ÿ�̸Ӹ� �ʱ�ȭ�Ѵ�.
	m_pTimer = new CTimer;
	m_pTimer->Init();

	// ��ΰ����� �ʱ�ȭ
	if (!GET_SINGLE(CPathManager)->Init())
		return false;

	// ���ҽ� ������ �ʱ�ȭ
	if (!GET_SINGLE(CResourceManager)->Init())
		return false;

	// ���� ������ �ʱ�ȭ
	if (!GET_SINGLE(CSoundManager)->Init())
		return false;

	// �浹 ������ �ʱ�ȭ
	if (!GET_SINGLE(CCollisionManager)->Init())
		return false;

	// �Է°����ڸ� �ʱ�ȭ�Ѵ�.
	if (!GET_SINGLE(CInput)->Init(m_hWnd))
		return false;

	// ����� Ű�� ����Ѵ�. ��� Ȱ��ȭ �Ѵ�.  
	//�� ��ư���� : �÷��̾��� �̵�, �뽬, ����, Ư����ų 
	AddInput();

	// �� ����۸� ���´�.
	m_pBackBuffer = GET_SINGLE(CResourceManager)->FindTexture("BackBuffer");
	// ����� ������ �Ѵ�. 
	m_pBackBuffer->Release();

	// ī�޶��� �ػ󵵸� �����ϴ� ���� -> ������� �������ָ� �»���� (0,0)���� �����ȴ�. 
	GET_SINGLE(CCamera)->SetResolution(m_tRS.iWidth, m_tRS.iHeight);
	GET_SINGLE(CCamera)->SetSceneSize(m_tRS.iWidth, m_tRS.iHeight);

	// �������� �ʱ�ȭ
	if (!GET_SINGLE(CSceneManager)->Init())
		return false;

	return true;
}

int CGameManager::Run()
{
	MSG msg;

	// �⺻ �޽��� �����Դϴ�:
	while (m_bLoop)
	{
		// PeekMessage �޼����� �������� true, �޼����� ������� ��� false 
		// PM_REMOVE = 0x0001 �޼���ť�� �޼����� �����ش�.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// ������ ����Ÿ���϶� else�� ���´�.
		else
		{
			// ���⿡ ���� ������ �����Ѵ�.
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CGameManager::Logic()
{
	/*
		�������� ����ϰ� �����Ѵ�. GlobalTime���� ��Ʈ�� �Ѵ�
		m_fTimeScale = 1
		Update���� ���� m_fDeltaTime�� 0�̸� -> �Ͻ�����
		������ ��ȯ �ӵ��� ������(ƽ ���� ���̸� �Ѿ�� �ӵ�) -> ��ü ���ο�
	*/
	float	fDeltaTime = m_pTimer->Update() * m_fTimeScale;

	// ���带 ������Ʈ �Ѵ�. 
	GET_SINGLE(CSoundManager)->Update(fDeltaTime);

	// ��ü �Է¿� ���� ������Ʈ�� �Ѵ�.
	GET_SINGLE(CInput)->Update(fDeltaTime);

	// ����� ��ȯ�Ѵ�. 
	if (Update(fDeltaTime) == SC_CHANGE)
		return;
	// ����� ��ȯ�Ѵ�. 
	if (LateUpdate(fDeltaTime) == SC_CHANGE)
		return;

	// ī�޶� ������Ʈ �Ѵ�. 
	// �� Update�� Ÿ���� �����ϰ� ī�޶� Ÿ���� ���� �ٴϰ� �Ѵ�. 
	GET_SINGLE(CCamera)->Update(fDeltaTime);

	// ��Ÿ Ÿ���� �����Ѵ�. 
	Collision(fDeltaTime);
	Render(fDeltaTime);
}

int CGameManager::Update(float fTime)
{
	// �� �̹� ������ ��� ������ ��ü�� �����ϴ� Update�� �����Ѵ�. 
	SCENE_CHANGE sc = (SCENE_CHANGE)GET_SINGLE(CSceneManager)->Update(fTime);

	return sc;
}

int CGameManager::LateUpdate(float fTime)
{
	// �� �̹� ������ ��� ������ ��ü�� �����ϴ� LateUpdate�� �����Ѵ�. 
	SCENE_CHANGE sc = (SCENE_CHANGE)GET_SINGLE(CSceneManager)->LateUpdate(fTime);

	return sc;
}

int CGameManager::Collision(float fTime)
{
	// �� �̹� ������ ��� ������ ��ü�� �����ϴ� Collision�� �����Ѵ�. 
	GET_SINGLE(CCollisionManager)->Collision(fTime);

	return 0;
}

// ����۸� �̿��� �ѹ��� �׸���. : ������۸� 
void CGameManager::Render(float fTime) // fDeltaTime�� ����
{
	// ȭ���� ��ü�� �� ���� �簢���� �����. �������� ��������� �� ���� �׷������� ȿ�� 
	// �簢�� �ϳ��� �׸���. 
	// ����ۿ��� ��������� �����´�.
	Rectangle(m_pBackBuffer->GetDC(), 0, 0, m_tRS.iWidth, m_tRS.iHeight);

	// �簢���� ������� ������ Ȱ���� ȭ�鿡 �׸���. 
	GET_SINGLE(CSceneManager)->PrevRender(m_pBackBuffer->GetDC(), fTime);
	GET_SINGLE(CSceneManager)->Render(m_pBackBuffer->GetDC(), fTime);
	GET_SINGLE(CSceneManager)->PostRender(m_pBackBuffer->GetDC(), fTime);

	SetBkMode(m_pBackBuffer->GetDC(), TRANSPARENT);
	TextOut(m_pBackBuffer->GetDC(), 640, 360, TEXT("Test"), 4);

	// ���� �������� ���콺�� ����Ѵ�.
	GET_SINGLE(CInput)->Render(m_pBackBuffer->GetDC(), fTime);

	// ���������� �ϼ��� ����۸� ȭ�鿡 �׷�����.
	m_pBackBuffer->Render(m_hDC, 0, 0, 0, 0, m_tRS.iWidth, m_tRS.iHeight);
}

void CGameManager::TestRender(float fTime)
{
	TextOut(m_hDC, 0, 0, TEXT("aa"), lstrlen(TEXT("aa")));
	TextOut(m_hDC, 100, 100, TEXT("���Ӹ�����"), lstrlen(TEXT("���Ӹ�����")));

	Rectangle(m_hDC, 200, 200, 300, 300); // �簢��

	Ellipse(m_hDC, 200, 200, 300, 300); // ��

	// ������ �������� �����Ѵ�.
	MoveToEx(m_hDC, 300, 200, nullptr);

	// ������ ������ �����Ѵ�.
	LineTo(m_hDC, 400, 300);

	// �������� ������ ���� ���������� �Ͽ� �� ����
	// ���������� ������ �׷�����.
	LineTo(m_hDC, 500, 300);

	// �׽�Ʈ 
	MoveToEx(m_hDC, 200, 400, nullptr);
	LineTo(m_hDC, 300, 500);
}

// �� ����� Ű�� ����Ѵ�.
// ��ư���� : �÷��̾��� �̵�, �뽬, ����, Ư����ų 
void CGameManager::AddInput() // Ű�� Ȱ��ȭ �Ѵ�. 
{
	// ���� 
	GET_SINGLE(CInput)->AddAxisKey("MoveFront", 'W', -1.f);
	GET_SINGLE(CInput)->AddAxisKey("MoveFront", 'S', 1.f);
	GET_SINGLE(CInput)->AddAxisKey("Move", 'S', -1.f, ST_CTRL | ST_ALT);

	// �¿� 
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

	// �� ��ü �ػ� ���� 
	m_tRS.iWidth = 1280;
	m_tRS.iHeight = 720;

	// WIN32 �簢������ü�� �����Ѵ�. 
	RECT	rc = { 0, 0, 1280, 720 };

	// ������ â ������ ������ �Լ��̴�.
	// ������ �簢�� ����ü�� �����ؼ� â�� Ȱ��ȭ �Ѵ�. 
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top,
		SWP_NOMOVE | SWP_NOZORDER);

	// ���� â�� �����ش�.
	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);

	return true;
}

LRESULT CGameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY: // ����Ÿ��
		m_bLoop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
