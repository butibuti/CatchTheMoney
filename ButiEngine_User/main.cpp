#include"stdafx_u.h"
#include"Header/ApplicationCreater.h"
#include"Header/GameObjects/DefaultGameComponent/ModelDrawComponent.h"
#pragma comment(lib,"ButiEngine.lib")
using namespace::ButiEngine;

#ifdef DEBUG



int main() {
	std::cout << "debug" << std::endl;
#else

int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
//int main()
{
#endif


	int returnCode = -1;

	while (returnCode == -1)
	{
		ApplicationInitData init = InputApplicationInitData();
		auto app = CreateEditorApplicationInstance(init);
		//auto app = CreateDefaultApplicationInstance(init);

		GameDevice::Initialize();
		//GameDevice::GetInput()->SetCursorHide(true);
		GameDevice::GetInput()->Initialize(app);
		app->PreLoadResources();
#ifdef DEBUG

		app->InitLoadResources();
		app->GetSceneManager()->LoadScene_Init(init.initSceneName);
		app->GetGraphicDevice()->SetClearColor(Vector4((255.0f / 255.0f), (254.0f / 255.0f), (250.0f / 255.0f), 1.0f));
#else
		GameDevice::GetInput()->SetCursorHide(true);
		app->GetSceneManager()->LoadScene_Init("Logo");
		app->GetGraphicDevice()->SetClearColor(Vector4(0, 0, 0, 1.0f));
#endif
		returnCode = app->Run();
		app->Exit();
	}



	return returnCode;
}