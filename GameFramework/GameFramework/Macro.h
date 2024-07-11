#pragma once

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define	SAFE_DELETE_ARRAY(p)	if(p)	{ delete[] p; p = nullptr; }
#define	SAFE_RELEASE(p)	if(p)	{ p->Release(); p = nullptr; }

#define	DECLARE_SINGLE(Type)	\
private:\
	static Type*	m_pInst;\
public:\
	static Type* GetInst()\
	{\
		if(!m_pInst)\
			m_pInst = new Type;\
		return m_pInst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(m_pInst);\
	}\
private:\
	Type();\
	~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_pInst = nullptr;

#define	GET_SINGLE(Type)	Type::GetInst()
#define	DESTROY_SINGLE(Type)	Type::DestroyInst()

// 하나만 생성하고, 이미 생성된 객체는 지워지기 전까지 계속 쓴다. 
#define	WINDOWINSTANCE	GET_SINGLE(CGameManager)->GetWindowInstance()
#define	WINDOWHANDLE	GET_SINGLE(CGameManager)->GetWindowHandle()
#define	WINDOWDC		GET_SINGLE(CGameManager)->GetWindowDC()
#define	_RESOLUTION		GET_SINGLE(CGameManager)->GetWindowResolution()
#define	REDBRUSH		GET_SINGLE(CGameManager)->GetRedBrush()
#define	GREENBRUSH		GET_SINGLE(CGameManager)->GetGreenBrush()
#define	REDPEN			GET_SINGLE(CGameManager)->GetRedPen()
#define	GREENPEN		GET_SINGLE(CGameManager)->GetGreenPen()

/*
	자식 타입으로 생성하는 시스템을 만든다
	자식 클래스 입장에서
	부모클래스에서의 자식의 PROTECTED, PRIVITE에는
	접근이 안되기 때문에 접근할 수 있는 시스템이 필요하다.
*/
#define	DECLARE_OBJECT	friend class CObj;