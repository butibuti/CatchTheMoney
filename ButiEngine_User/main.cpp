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


	//auto app = CreateDefaultApplicationInstance("CollisionTest", WindowPopType::max, 1080, 700, false);
	auto app = CreateEditorApplicationInstance("Editor", WindowPopType::max,1080,700, false);
	GameDevice::Initialize();
	GameDevice::GetInput()->Initialize(app);
	//ModelFileConverter::FBXtoB3M("simpleGround.fbx", "simpleGround.b3m", "Model/Stage/");
	app->InitLoadResources();
	app->GetSceneManager()->LoadScene_Init("Title");

	app->GetGraphicDevice()->SetClearColor(Vector4(0.5,0.5,0.8, 1));
	int returnCode = app->Run();


	app->Exit();

	return returnCode;
}