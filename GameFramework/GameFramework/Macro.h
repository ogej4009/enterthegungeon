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

// �ϳ��� �����ϰ�, �̹� ������ ��ü�� �������� ������ ��� ����. 
#define	WINDOWINSTANCE	GET_SINGLE(CGameManager)->GetWindowInstance()
#define	WINDOWHANDLE	GET_SINGLE(CGameManager)->GetWindowHandle()
#define	WINDOWDC		GET_SINGLE(CGameManager)->GetWindowDC()
#define	_RESOLUTION		GET_SINGLE(CGameManager)->GetWindowResolution()
#define	REDBRUSH		GET_SINGLE(CGameManager)->GetRedBrush()
#define	GREENBRUSH		GET_SINGLE(CGameManager)->GetGreenBrush()
#define	REDPEN			GET_SINGLE(CGameManager)->GetRedPen()
#define	GREENPEN		GET_SINGLE(CGameManager)->GetGreenPen()

/*
	�ڽ� Ÿ������ �����ϴ� �ý����� �����
	�ڽ� Ŭ���� ���忡��
	�θ�Ŭ���������� �ڽ��� PROTECTED, PRIVITE����
	������ �ȵǱ� ������ ������ �� �ִ� �ý����� �ʿ��ϴ�.
*/
#define	DECLARE_OBJECT	friend class CObj;