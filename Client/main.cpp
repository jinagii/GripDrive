#include "framework.h"
#include "GameManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: ���⿡ �ڵ带 �Է��մϴ�.
	GameManager* gm = new GameManager();
	gm->Initialize(hInstance);
	gm->Looping();
	gm->Finish();
	/// ������� ���� ����� ���⼭ �ٷ� ����

	return 0;
}