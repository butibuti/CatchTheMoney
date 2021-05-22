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


	//auto app = CreateDefaultApplicationInstance("Choose", WindowPopType::max, 1080, 700, false);
	auto app = CreateEditorApplicationInstance("Editor", WindowPopType::max,1080,700, false);
	GameDevice::Initialize();
	GameDevice::GetInput()->Initialize(app);

	app->InitLoadResources();
	app->GetSceneManager()->LoadScene_Init("Title");

	app->GetGraphicDevice()->SetClearColor(Vector4(107.0f / 255.0f,43.0f / 255.0f,189.0f / 255.0f, 1.0f));
	int returnCode = app->Run();


	app->Exit();

	return returnCode;
}