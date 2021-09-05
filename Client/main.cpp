#include "framework.h"
#include "GameManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.
	GameManager* gm = new GameManager();
	gm->Initialize(hInstance);
	gm->Looping();
	gm->Finish();
	/// 디버깅을 위해 현재는 여기서 바로 실행

	return 0;
}