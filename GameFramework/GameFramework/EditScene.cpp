#include "EditScene.h"
#include "Stage.h"
#include "GameManager.h"
#include "Camera.h"
#include "InputComponent.h"
#include "Input.h"
#include "Solid.h"
#include "ColliderRect.h"
#include "Collider.h"

CEditScene::CEditScene()
{
	m_eOption = TO_NORMAL;
	m_eMode = EM_OPTION;
	m_pInputComponent = nullptr;
}

CEditScene::~CEditScene()
{
	SAFE_RELEASE(m_pInputComponent);
}

void CEditScene::ModeChangeNext(float fTime)
{
	int	iMode = m_eMode + 1;

	if (iMode >= EM_END)
		iMode = EM_END - 1;

	m_eMode = (EDIT_MODE)iMode;
}

void CEditScene::ModeChangePrev(float fTime)
{
	int	iMode = m_eMode - 1;

	if (iMode <= EM_NONE)
		iMode = EM_NONE + 1;

	m_eMode = (EDIT_MODE)iMode;
}

void CEditScene::OptionChangeNext(float fTime)
{
	int	iOption = m_eOption + 1;

	if (iOption >= TO_END)
		iOption = TO_END - 1;

	m_eOption = (TILE_OPTION)iOption;
}

void CEditScene::OptionChangePrev(float fTime)
{
	int	iOption = m_eOption - 1;

	if (iOption <= TO_NONE)
		iOption = TO_NONE + 1;

	m_eOption = (TILE_OPTION)iOption;
}

void CEditScene::Save(float fTime)
{
	OPENFILENAME	tOFN;

	memset(&tOFN, 0, sizeof(OPENFILENAME));

	TCHAR	strPath[MAX_PATH] = {};

	tOFN.lStructSize = sizeof(OPENFILENAME);
	tOFN.hwndOwner = WINDOWHANDLE;
	tOFN.lpstrFilter = TEXT("Stage File(*.stg)\0*.stg\0All Files(*.*)\0*.*\0");
	tOFN.lpstrFile = strPath;
	tOFN.nMaxFile = MAX_PATH;
	tOFN.lpstrInitialDir = TEXT("..\\Bin\\");

	ShowCursor(TRUE);

	if (GetSaveFileName(&tOFN) != 0)
	{
		// Ǯ��ΰ� ������ �ǰ� �� ��θ� �̿��ؼ�
		// ������ ������.
		char	strFullPath[MAX_PATH] = {};

		// �ٲ� ���̸� ���´�.
		int iLength = WideCharToMultiByte(CP_ACP, 0, strPath,
			-1, nullptr, 0,
			nullptr, nullptr);

		// ������ ���� ���̸� �̿��ؼ� ���ڿ��� ��ȯ�Ѵ�.
		WideCharToMultiByte(CP_ACP, 0, strPath, -1, strFullPath,
			iLength, nullptr, nullptr);

		m_pStage->SaveFromFullPath(strFullPath);
	}

	ShowCursor(FALSE);
}

void CEditScene::Load(float fTime)
{
	OPENFILENAME	tOFN;

	memset(&tOFN, 0, sizeof(OPENFILENAME));

	TCHAR	strPath[MAX_PATH] = {};

	tOFN.lStructSize = sizeof(OPENFILENAME);
	tOFN.hwndOwner = WINDOWHANDLE;
	tOFN.lpstrFilter = TEXT("Stage File(*.stg)\0*.stg\0All Files(*.*)\0*.*\0");
	tOFN.lpstrFile = strPath;
	tOFN.nMaxFile = MAX_PATH;
	tOFN.lpstrInitialDir = TEXT("..\\Bin\\");

	ShowCursor(TRUE);

	if (GetOpenFileName(&tOFN) != 0)
	{
		// Ǯ��ΰ� ������ �ǰ� �� ��θ� �̿��ؼ�
		// ������ ������.
		char	strFullPath[MAX_PATH] = {};

		// �ٲ� ���̸� ���´�.
		int iLength = WideCharToMultiByte(CP_ACP, 0, strPath,
			-1, nullptr, 0,
			nullptr, nullptr);

		// ������ ���� ���̸� �̿��ؼ� ���ڿ��� ��ȯ�Ѵ�.
		WideCharToMultiByte(CP_ACP, 0, strPath, -1, strFullPath,
			iLength, nullptr, nullptr);

		m_pStage->LoadFromFullPath(strFullPath);
	}

	ShowCursor(FALSE);
}

void CEditScene::Start()
{
	CScene::Start();
}

bool CEditScene::Init()
{
	CScene::Init();

	// �� ���� ������ ���� 
	m_tSceneSize.x = 8730.f;
	m_tSceneSize.y = 1200.f;

	// ���� ����� ���� 
	CObj::CreateObject<CStage>("MainStage", "BackGround", this);

	// �� �������� ���� 
	m_pStage = CObj::CreateObject<CStage>("MainStage", "BackGround", this);

	// �� Ÿ�� ũ��, ������ ����  
	m_pStage->SetTexture("Stage1", TEXT("Stage1.bmp"));
	m_pStage->CreateTile(116, 20, Size(75.f, 60.f));

	// �� ī�޶��� �̵�Ű ��� 
	GET_SINGLE(CCamera)->OnFreeCamera();

	// �� �׼� Ű�� ����Ѵ�. 
	GET_SINGLE(CInput)->AddActionKey("EditModePrev", '1');
	GET_SINGLE(CInput)->AddActionKey("EditModeNext", '2');
	GET_SINGLE(CInput)->AddActionKey("EditOptionPrev", '3');
	GET_SINGLE(CInput)->AddActionKey("EditOptionNext", '4');
	GET_SINGLE(CInput)->AddActionKey("Save", 'S', ST_CTRL);
	GET_SINGLE(CInput)->AddActionKey("Load", 'O', ST_CTRL);

	// �� Ű�� �׼Ǳ���� ����Ѵ�.
	m_pInputComponent = new CInputComponent;
	m_pInputComponent->BindAction<CEditScene>("EditModeNext", IE_PRESS, this, &CEditScene::ModeChangeNext, "ModeChangeNext");
	m_pInputComponent->BindAction<CEditScene>("EditModePrev", IE_PRESS, this, &CEditScene::ModeChangePrev, "ModeChangePrev");
	m_pInputComponent->BindAction<CEditScene>("EditOptionNext", IE_PRESS, this, &CEditScene::OptionChangeNext, "OptionChangeNext");
	m_pInputComponent->BindAction<CEditScene>("EditOptionPrev", IE_PRESS, this, &CEditScene::OptionChangePrev, "OptionChangePrev");
	m_pInputComponent->BindAction<CEditScene>("Save", IE_PRESS, this, &CEditScene::Save, "Save");
	m_pInputComponent->BindAction<CEditScene>("Load", IE_PRESS, this, &CEditScene::Load, "Load");

	return true;
}

void CEditScene::Update(float fTime)
{
	if (GET_SINGLE(CInput)->GetLButtonInfo().bHold)
	{
		Position	tPos = GET_SINGLE(CInput)->GetLButtonInfo().tPos +
			GET_SINGLE(CCamera)->GetPos();
		// Ÿ�� �ε����� ã�Ƽ� �ش� Ÿ���� �ɼ��� �����Ų��.
		m_pStage->ChangeTileOption(tPos,
			m_eOption);
	}


	CScene::Update(fTime);
}

void CEditScene::Render(HDC hDC, float fTime)
{
	CScene::Render(hDC, fTime);

	switch (m_eMode)
	{
	case EM_IMAGE:
		TextOut(hDC, 30, 20, TEXT("Mode : Image"), lstrlen(TEXT("Mode : Image")));
		break;
	case EM_OPTION:
		TextOut(hDC, 30, 20, TEXT("Mode : Option"), lstrlen(TEXT("Mode : Option")));
		break;
	}

	switch (m_eOption)
	{
	case TO_NORMAL:
		TextOut(hDC, 30, 50, TEXT("Option : Normal"), lstrlen(TEXT("Option : Normal")));
		break;
	case TO_WALL:
		TextOut(hDC, 30, 50, TEXT("Option : Wall"), lstrlen(TEXT("Option : Wall")));
		break;
	}
}
