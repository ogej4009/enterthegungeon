#pragma once
#include "Game.h"

class CGameManager
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	HDC			m_hDC;
	Resolution	m_tRS;
	class CTimer* m_pTimer;
	float		m_fTimeScale;
	class CTexture* m_pBackBuffer; // ＠ 백버퍼는여기
	HBRUSH		m_hRedBrush;
	HBRUSH		m_hGreenBrush;
	HPEN		m_hRedPen;
	HPEN		m_hGreenPen;

private:
	static bool	m_bLoop; // ＠ static변수를 사용 

public:
	HINSTANCE GetWindowInstance()	const;
	HWND GetWindowHandle()			const;
	HDC GetWindowDC()				const;
	Resolution GetWindowResolution()const;
	float GetTimeScale()			const;
	HBRUSH GetRedBrush()	const
	{
		return m_hRedBrush;
	}

	HBRUSH GetGreenBrush()	const
	{
		return m_hGreenBrush;
	}

	HPEN GetRedPen()	const
	{
		return m_hRedPen;
	}

	HPEN GetGreenPen()	const
	{
		return m_hGreenPen;
	}

	float GetDeltaTime()	const;

public:
	void SetTimeScale(float fScale);

public:
	bool Init(HINSTANCE hInst);
	int Run();

private:
	void Logic();
	int Update(float fTime);
	int LateUpdate(float fTime);
	int Collision(float fTime);
	void Render(float fTime);

private:
	void TestRender(float fTime);

private:
	void AddInput();

private:
	ATOM RegisterWindowClass(const TCHAR* pClass);
	bool Create(const TCHAR* pClass, const TCHAR* pTitle);

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLE(CGameManager)

};
