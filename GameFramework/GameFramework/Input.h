#pragma once
#include "Game.h"

/*
	�پ��� ���̽� : Ű�� ������ ��, �� ��, ������ �� ����, ������ ����
	���� ���ϴ� Ű�� ����� �� �ִ� �ý���
	���� ����� Ű�� ���ؼ��� ��� -> ó��

	������ ���ο��� �̺�Ʈ�� �߻��� �ϸ� �׿� �´� �޼����� �����.
	GetAsyncKeyState : �Է� �� ��ü�� ���ڸ����� �ٷ� üũ�ϴ� �Լ�
	Ű �Է� ��ü�� ó���ϱ� ������, ����� ������ GetAsyncKeyState�� ����ϸ� �����ϴ�.
*/


// ����� Ű ����
enum SKEY
{
	SKEY_CTRL,
	SKEY_ALT,
	SKEY_SHIFT,
	SKEY_END
};

// ����� Ű �ѹ� ���� (��Ʈ)
enum SKEY_TYPE
{
	ST_CTRL = 0x1,
	ST_ALT = 0x2,
	ST_SHIFT = 0x4
};


typedef struct _tagKeyFunction
{
	// Ű �ϳ��� �������� �Լ��� ������ �� ���� 
	function<void(float, float)>	func;

	void* pObj;
	string	strFuncName;

	_tagKeyFunction()
	{
		pObj = nullptr;
	}
}KeyFunction, * PKeyFunction;

typedef struct _tagActionKeyFunction
{
	// Ű �ϳ��� �������� �Լ��� ������ �� ���� 
	function<void(float)>	func;

	void* pObj;
	string	strFuncName;
	INPUT_EVENT	eEvent; // ���� ��Ȳ 

	_tagActionKeyFunction()
	{
		pObj = nullptr;
	}
}ActionKeyFunction, * PActionKeyFunction;

/*
	Ű���忡 �ִ� Ű�� �׷��� ���� �ʴ�.
	GetAsyncKeyState ���� ���ڴ� ������ �빮�ڸ� �־��ָ� ��,
	����, ȭ��ǥ, �����̽�, CTRL, ���		VK�ø��� : VK_RETURN
	#define VK_CLEAR          0x0C
	#define VK_RETURN         0x0D
	16���� 2�ڸ��� 16���� 1�ڸ��� ������ 4�ڸ�  -> 1 * 4 * 8 = 32bit = 1byte
	Ű�� 1byte���� �ϳ��� �ĺ� ����
*/
typedef struct _tagKeyInfo
{

	unsigned char	cKey; // Ű �̸� 
	bool	bSKey[SKEY_END]; // üũ�ڽ� 
	/*
		�� Ű�� ������ �� �ǽð����� �������� �Ǵ��Ѵ�.
		�� Ű�� ������ ���� 3������ ǥ���Ѵ�.
		������ �� ���� true�� �Ǵ� ����
		������ ���� true�� �Ǵ� ����
		������ ���� �� true�� �Ǵ� ����
	*/
	bool	bDown;
	bool	bHold;
	bool	bUp;

	_tagKeyInfo()
	{
		cKey = 0;
		bDown = false;
		bHold = false;
		bUp = false;

		for (int i = 0; i < SKEY_END; ++i)
		{
			bSKey[i] = false;
		}
	}
}KeyInfo, * PKeyInfo;

// �̵� ���� �� �ý��� 
typedef struct _tagAxisInfo
{
	KeyInfo	tKey;
	float	fScale; // ������ 
}AxisInfo, * PAxisInfo;

// AxisŰ ���� �ϼ� ��Ű�� 
typedef struct _tagBindAxisInfo
{
	string		strName;
	list<AxisInfo>	KeyList;
	list<KeyFunction>	FunctionList;
}BindAxisInfo, * PBindAxisInfo;

// ActionŰ ���� �ϼ� ��Ű�� 
typedef struct _tagBindActionInfo
{
	string		strName; // ���� �̸� 
	bool		bDoubleStart; // ���� Ŭ�� ����  
	float		fDoubleTime; // ���� Ŭ���� ������ ���� �ð� 
	list<KeyInfo>	KeyList; // Ű ��� 
	list<ActionKeyFunction>	FunctionList; // �Լ� ���  

	_tagBindActionInfo() :
		fDoubleTime(0.f),
		bDoubleStart(false)
	{
	}
}BindActionInfo, * PBindActionInfo;

typedef struct _tagCommandInfo
{
	// Ŀ�ǵ� Ű�� ���� 
}CommandInfo, * PCommandInfo;


typedef struct _tagInputKey
{
	// ��ϵ� Ű���� �ƴ��� �Ǵ��ϴ� bool���� 
	bool	bEnable;
	unsigned char	cKey; // ��ϵ� Ű �̸� 
	/*
		�� Ű�� ������ �� �ǽð����� �������� �Ǵ��Ѵ�.
		�� Ű�� ������ ���� 3������ ǥ���Ѵ�.
		������ �� ���� true�� �Ǵ� ����
		������ ���� true�� �Ǵ� ����
		������ ���� �� true�� �Ǵ� ����
	*/
	bool	bDown;
	bool	bHold;
	bool	bUp;

	_tagInputKey()
	{
		cKey = 0;
		bDown = false;
		bHold = false;
		bUp = false;
		bEnable = false; // ����Ʈ�� false
	}
}InputKey, * PInputKey;

// ���콺 ���� 
typedef struct _tagMouseInfo
{
	Position	tPos; // ���콺 ���� ������ ��ġ ���� 
	Position	tMove; // ���콺 �̵��� 
	Position	tPrevPos; // ���콺 ���� ������ ��ġ ���� 
	// Ŭ�� ��Ȳ ���� ���� 
	bool		bDown;
	bool		bHold;
	bool		bDouble;
	bool		bUp;

	_tagMouseInfo()
	{
		bDown = false;
		bHold = false;
		bUp = false;
		bDouble = false;
	}
}MouseInfo, * PMouseInfo;

class CInput
{
	friend class CInputComponent; // ������Ʈ�� ���� �����Ѵ�. 
	// GET_SINGLE(CInput)-> �̷��� �ؼ� BindAxis�Լ��� �����Ѵ�. 

private:
	unordered_map<string, PBindAxisInfo>	m_mapBindAxis; // ����Axis
	unordered_map<string, PBindActionInfo>	m_mapBindAction; // ����Action
	vector<InputKey>	m_KeyList; // ��ϵ� Ű(����) ���� 
	InputKey			m_tSKey[SKEY_END]; // ��ϵ� ����� Ű ���� 
	HWND				m_hWnd; // �ڵ� ������ �ν��Ͻ� 
	Position			m_tMousePos; // ���콺 ���� �����ӿ����� ��ġ 
	Position			m_tMousePrevPos; // ���콺 ���� �����ӿ����� ��ġ 
	Position			m_tMouseMove; // ���콺 �̵��� 
	class CObj* m_pMouseObj; // ���� ���콺 Obj ���� 
	class CCollider* m_pMouseWorldCollider; // ���콺 ���� �浹ü 
	class CCollider* m_pMouseUICollider; // ���콺 UI �浹ü 
	class CAnimation* m_pMouseAnimation; // ���콺 �ִϸ��̼� 
	MouseInfo			m_tLButtonInfo; // ���콺 LŬ�� ���� 
	MouseInfo			m_tRButtonInfo; // ���콺 RŬ�� ���� 

public:
	MouseInfo GetLButtonInfo()	const
	{
		return m_tLButtonInfo;
	}

	MouseInfo GetRButtonInfo()	const
	{
		return m_tRButtonInfo;
	}

	class CCollider* GetWorldCollider()	const
	{
		return m_pMouseWorldCollider;
	}

	class CCollider* GetUICollider()	const
	{
		return m_pMouseUICollider;
	}

	Position GetMousePos()	const
	{
		return m_tMousePos;
	}

	Position GetMouseMove()	const
	{
		return m_tMouseMove;
	}

	Position GetMousePrevPos()	const
	{
		return m_tMousePrevPos;
	}

private: // ���콺 ��ü ����
	void CreateMouse();

public: // ���� ���� 
	bool Init(HWND hWnd);
	void Update(float fTime);
	void Render(HDC hDC, float fTime);
	void ChangeScene();

private: // ���� ���¸� �����ϴ� �Լ� 
	void MouseUpdate(float fTime);
	void SKeyInput();
	void KeyInput();
	void AxisInput(float fTime);
	void ActionInput(float fTime);

public: // Ű�� ����Ѵ�. 
	void AddAxisKey(const string& strName, unsigned char cKey,
		float fScale, int iSKey = 0);
	void AddActionKey(const string& strName, unsigned char cKey,
		int iSKey = 0);

public: // ���콺 Ŭ�� 8���� ���� 
	void LButtonDown(float fTime);
	void LButtonHold(float fTime);
	void LButtonUp(float fTime);
	void LButtonDouble(float fTime);
	void RButtonDown(float fTime);
	void RButtonHold(float fTime);
	void RButtonUp(float fTime);
	void RButtonDouble(float fTime);

private: // ��ϵ� �������� Ž���ϴ� �Լ� 
	PBindAxisInfo FindAxis(const string& strName);
	PBindActionInfo FindAction(const string& strName);

private: // ��ϵ� �������� �����ִ� �Լ� 
	bool DeleteBindAxis(const string& strName, void* pObj,
		const string& strFuncName);
	bool DeleteBindAction(const string& strName, void* pObj,
		const string& strFuncName);

	/*
		���� BindAxis�� ����ϴ� �� )
		������Ʈ�� ����� �� ����Ѵ�.
		m_pInputComponent->BindAxis<CCamera>("MoveFront", this, &CCamera::MoveFront, "MoveFront");
	*/

	template <typename T>
	bool BindAxis(const string& strName, T* pObj,
		void(T::* pFunc)(float, float),
		const string& strFuncName)
	{
		// Ž��
		PBindAxisInfo	pAxis = FindAxis(strName);

		// ����ó�� 
		if (!pAxis)
			return false;

		// ���� ��Ȳ�� ��� 
		KeyFunction	tFunc;

		tFunc.pObj = pObj;
		tFunc.func = bind(pFunc, pObj, placeholders::_1, placeholders::_2);
		tFunc.strFuncName = strFuncName;

		// ��� �߰� 
		pAxis->FunctionList.push_back(tFunc);

		return true; // �� ������ true�̸� ���𰡸� �����ϴ� �Լ� 
	}

	template <typename T>
	bool BindAction(const string& strName, INPUT_EVENT eEvent,
		T* pObj,
		void(T::* pFunc)(float),
		const string& strFuncName)
	{
		// Ž�� 
		PBindActionInfo	pAction = FindAction(strName);

		// ���� ó�� 
		if (!pAction)
			return false;

		// ���� ��Ȳ�� ��� 
		ActionKeyFunction	tFunc;

		tFunc.eEvent = eEvent;
		tFunc.pObj = pObj;
		tFunc.func = bind(pFunc, pObj, placeholders::_1);
		tFunc.strFuncName = strFuncName;

		// ��Ͽ� �߰� 
		pAction->FunctionList.push_back(tFunc);

		return true; // �� ������ true�̸� ���𰡸� �����ϴ� �Լ� 
	}

	// �̱��� ���� 
	DECLARE_SINGLE(CInput)
};

