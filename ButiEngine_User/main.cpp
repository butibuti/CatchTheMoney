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


	//auto app = CreateDefaultApplicationInstance("Choose", WindowPopType::max, 1920, 1080, true);
	auto app = CreateEditorApplicationInstance("Editor", WindowPopType::max,1080,700, false);
	GameDevice::Initialize();
	GameDevice::GetInput()->Initialize(app);
	app->PreLoadResources();
#ifdef DEBUG

	app->InitLoadResources();
	app->GetSceneManager()->LoadScene_Init("Title");
#else
	app->GetSceneManager()->LoadScene_Init("Logo");
#endif
	

	app->GetGraphicDevice()->SetClearColor(Vector4((255.0f / 255.0f), (254.0f / 255.0f), (250.0f / 255.0f), 1.0f));
	int returnCode = app->Run();


	app->Exit();

	return returnCode;
}