#pragma once
#include "Game.h"

/*
	다양한 케이스 : 키를 눌렀을 때, 떌 때, 누르는 그 순간, 눌렀다 땔때
	내가 원하는 키를 등록할 수 있는 시스템
	내가 사용할 키에 대해서만 등록 -> 처리

	윈도우 내부에서 이벤트가 발생을 하면 그에 맞는 메세지를 만든다.
	GetAsyncKeyState : 입력 그 자체를 그자리에서 바로 체크하는 함수
	키 입력 자체만 처리하기 때문에, 사용할 때마다 GetAsyncKeyState를 사용하면 불편하다.
*/


// 스페셜 키 종류
enum SKEY
{
	SKEY_CTRL,
	SKEY_ALT,
	SKEY_SHIFT,
	SKEY_END
};

// 스페셜 키 넘버 지정 (비트)
enum SKEY_TYPE
{
	ST_CTRL = 0x1,
	ST_ALT = 0x2,
	ST_SHIFT = 0x4
};


typedef struct _tagKeyFunction
{
	// 키 하나에 여러개의 함수가 지정될 수 있음 
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
	// 키 하나에 여러개의 함수가 지정될 수 있음 
	function<void(float)>	func;

	void* pObj;
	string	strFuncName;
	INPUT_EVENT	eEvent; // 누름 상황 

	_tagActionKeyFunction()
	{
		pObj = nullptr;
	}
}ActionKeyFunction, * PActionKeyFunction;

/*
	키보드에 있는 키는 그렇게 많지 않다.
	GetAsyncKeyState 사용시 문자는 문자의 대문자를 넣어주면 됨,
	엔터, 화살표, 스페이스, CTRL, 등등		VK시리즈 : VK_RETURN
	#define VK_CLEAR          0x0C
	#define VK_RETURN         0x0D
	16진수 2자리면 16진수 1자리가 이진수 4자리  -> 1 * 4 * 8 = 32bit = 1byte
	키는 1byte변수 하나면 식별 가능
*/
typedef struct _tagKeyInfo
{

	unsigned char	cKey; // 키 이름 
	bool	bSKey[SKEY_END]; // 체크박스 
	/*
		이 키를 눌렀을 때 실시간으로 눌렀는지 판단한다.
		이 키를 누르는 상태 3가지를 표현한다.
		누르는 그 순간 true가 되는 변수
		누르고 땔때 true가 되는 변수
		누르고 있을 때 true가 되는 변수
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

// 이동 관련 축 시스템 
typedef struct _tagAxisInfo
{
	KeyInfo	tKey;
	float	fScale; // 스케일 
}AxisInfo, * PAxisInfo;

// Axis키 지정 완성 패키지 
typedef struct _tagBindAxisInfo
{
	string		strName;
	list<AxisInfo>	KeyList;
	list<KeyFunction>	FunctionList;
}BindAxisInfo, * PBindAxisInfo;

// Action키 지정 완성 패키지 
typedef struct _tagBindActionInfo
{
	string		strName; // 묶음 이름 
	bool		bDoubleStart; // 더블 클릭 여부  
	float		fDoubleTime; // 더블 클릭을 누르는 제한 시간 
	list<KeyInfo>	KeyList; // 키 목록 
	list<ActionKeyFunction>	FunctionList; // 함수 목록  

	_tagBindActionInfo() :
		fDoubleTime(0.f),
		bDoubleStart(false)
	{
	}
}BindActionInfo, * PBindActionInfo;

typedef struct _tagCommandInfo
{
	// 커맨드 키는 생략 
}CommandInfo, * PCommandInfo;


typedef struct _tagInputKey
{
	// 등록된 키인지 아닌지 판단하는 bool변수 
	bool	bEnable;
	unsigned char	cKey; // 등록된 키 이름 
	/*
		이 키를 눌렀을 때 실시간으로 눌렀는지 판단한다.
		이 키를 누르는 상태 3가지를 표현한다.
		누르는 그 순간 true가 되는 변수
		누르고 땔때 true가 되는 변수
		누르고 있을 때 true가 되는 변수
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
		bEnable = false; // 디폴트는 false
	}
}InputKey, * PInputKey;

// 마우스 정보 
typedef struct _tagMouseInfo
{
	Position	tPos; // 마우스 현재 프레임 위치 정보 
	Position	tMove; // 마우스 이동량 
	Position	tPrevPos; // 마우스 이전 프레임 위치 정보 
	// 클릭 상황 지정 정보 
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
	friend class CInputComponent; // 컴포넌트를 통해 지정한다. 
	// GET_SINGLE(CInput)-> 이렇게 해서 BindAxis함수로 접근한다. 

private:
	unordered_map<string, PBindAxisInfo>	m_mapBindAxis; // 지정Axis
	unordered_map<string, PBindActionInfo>	m_mapBindAction; // 지정Action
	vector<InputKey>	m_KeyList; // 등록된 키(누름) 모음 
	InputKey			m_tSKey[SKEY_END]; // 등록된 스페셜 키 모음 
	HWND				m_hWnd; // 핸들 윈도우 인스턴스 
	Position			m_tMousePos; // 마우스 현재 프레임에서의 위치 
	Position			m_tMousePrevPos; // 마우스 이전 프레임에서의 위치 
	Position			m_tMouseMove; // 마우스 이동량 
	class CObj* m_pMouseObj; // 현재 마우스 Obj 정보 
	class CCollider* m_pMouseWorldCollider; // 마우스 월드 충돌체 
	class CCollider* m_pMouseUICollider; // 마우스 UI 충돌체 
	class CAnimation* m_pMouseAnimation; // 마우스 애니메이션 
	MouseInfo			m_tLButtonInfo; // 마우스 L클릭 정보 
	MouseInfo			m_tRButtonInfo; // 마우스 R클릭 정보 

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

private: // 마우스 객체 생성
	void CreateMouse();

public: // 게임 로직 
	bool Init(HWND hWnd);
	void Update(float fTime);
	void Render(HDC hDC, float fTime);
	void ChangeScene();

private: // 누름 상태를 지정하는 함수 
	void MouseUpdate(float fTime);
	void SKeyInput();
	void KeyInput();
	void AxisInput(float fTime);
	void ActionInput(float fTime);

public: // 키를 등록한다. 
	void AddAxisKey(const string& strName, unsigned char cKey,
		float fScale, int iSKey = 0);
	void AddActionKey(const string& strName, unsigned char cKey,
		int iSKey = 0);

public: // 마우스 클릭 8가지 상태 
	void LButtonDown(float fTime);
	void LButtonHold(float fTime);
	void LButtonUp(float fTime);
	void LButtonDouble(float fTime);
	void RButtonDown(float fTime);
	void RButtonHold(float fTime);
	void RButtonUp(float fTime);
	void RButtonDouble(float fTime);

private: // 등록된 정보들을 탐색하는 함수 
	PBindAxisInfo FindAxis(const string& strName);
	PBindActionInfo FindAction(const string& strName);

private: // 등록된 정보들을 지워주는 함수 
	bool DeleteBindAxis(const string& strName, void* pObj,
		const string& strFuncName);
	bool DeleteBindAction(const string& strName, void* pObj,
		const string& strFuncName);

	/*
		실제 BindAxis를 사용하는 예 )
		컴포넌트에 등록한 후 사용한다.
		m_pInputComponent->BindAxis<CCamera>("MoveFront", this, &CCamera::MoveFront, "MoveFront");
	*/

	template <typename T>
	bool BindAxis(const string& strName, T* pObj,
		void(T::* pFunc)(float, float),
		const string& strFuncName)
	{
		// 탐색
		PBindAxisInfo	pAxis = FindAxis(strName);

		// 예외처리 
		if (!pAxis)
			return false;

		// 현재 상황을 등록 
		KeyFunction	tFunc;

		tFunc.pObj = pObj;
		tFunc.func = bind(pFunc, pObj, placeholders::_1, placeholders::_2);
		tFunc.strFuncName = strFuncName;

		// 목록 추가 
		pAxis->FunctionList.push_back(tFunc);

		return true; // ＠ 리턴이 true이면 무언가를 지정하는 함수 
	}

	template <typename T>
	bool BindAction(const string& strName, INPUT_EVENT eEvent,
		T* pObj,
		void(T::* pFunc)(float),
		const string& strFuncName)
	{
		// 탐색 
		PBindActionInfo	pAction = FindAction(strName);

		// 예외 처리 
		if (!pAction)
			return false;

		// 현재 상황을 등록 
		ActionKeyFunction	tFunc;

		tFunc.eEvent = eEvent;
		tFunc.pObj = pObj;
		tFunc.func = bind(pFunc, pObj, placeholders::_1);
		tFunc.strFuncName = strFuncName;

		// 목록에 추가 
		pAction->FunctionList.push_back(tFunc);

		return true; // ＠ 리턴이 true이면 무언가를 지정하는 함수 
	}

	// 싱글톤 선언 
	DECLARE_SINGLE(CInput)
};

