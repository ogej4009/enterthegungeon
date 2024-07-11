#include "GameManager.h"


union test
{
	int	a;
	int	b;
	int	c;
};

// Data Sound Texture

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	/*int	iSize = sizeof(test);
	test t;
	t.a = 10;*/

	if (!GET_SINGLE(CGameManager)->Init(hInstance))
	{
		DESTROY_SINGLE(CGameManager);
		return 0;
	}

	int	iRet = GET_SINGLE(CGameManager)->Run();

	DESTROY_SINGLE(CGameManager);

	return iRet;
}

// Render