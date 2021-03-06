#pragma once
#include"stdafx.h"
namespace ButiEngine {

class ISceneManager;
class Window;
class ResourceContainer;



class Application :public IApplication
{
public:
	Application() {};
	void Initialize()override {}
	void PreInitialize()override {}
	void CreateInstances(const std::string windowName = "Buti", const WindowPopType arg_windowPopType = WindowPopType::normal, const UINT windowWidth = 720, const UINT windowHeight = 480, const bool isFullScreen = false,const bool isEditor=false);
	std::unique_ptr<IWindow>& GetWindow()override;
	std::shared_ptr<ISceneManager> GetSceneManager()override;
	std::shared_ptr<GraphicDevice> GetGraphicDevice()override;
	std::shared_ptr<IResourceContainer> GetResourceContainer()override;
	std::shared_ptr<GameObjectTagManager> GetGameObjectTagManager()override;
	std::shared_ptr<ISoundManager> GetSoundManager()override;
	bool Update()override;
	int Run()override;
	void InitLoadResources()override;
	void PreLoadResources()override;
	void Exit()override;
	long long int GetBefFrame()const override;
private:
	std::shared_ptr<GraphicDevice>shp_graphicDevice;
	std::unique_ptr<IWindow> unq_window;
	std::shared_ptr< ISceneManager> shp_sceneManager;
	std::shared_ptr<IResourceContainer>shp_resourceContainer;
	std::shared_ptr<ISoundManager>shp_soundManager;
	std::shared_ptr<GameObjectTagManager> shp_gameObjTagManager;
	std::timespec befTs;
	std::timespec nowTs;
	long long int befFrameTime;
};

}