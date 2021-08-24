#pragma once
#ifndef _SceneManager_Edit_H_
#define _SceneManager_Edit_H_

#include"SceneManager.h"
#include<thread>
namespace ButiEngine {
	class IApplication;
	struct SceneInformation;
	class SceneManager_Edit :public SceneManager
	{
	public:
		SceneManager_Edit(std::weak_ptr<IApplication> arg_wkp_app);
		void Initialize()override;
		void Update()override;
		void ShowGUI();
		void RenewalScene()override;
		void ChangeScene(const std::string& arg_sceneName, float sceneChangeDalay = 0)override;
		void LoadScene(const std::string& arg_sceneName, std::shared_ptr<SceneInformation> shp_scene = nullptr) override;
		void LoadScene_Init(const std::string& arg_sceneName, std::shared_ptr<SceneInformation> shp_scene = nullptr) override;
		void Release()override;
		void Reload();
	private:
		bool isActive = false;
		bool isPlaying = false;
		std::shared_ptr<IResource_Texture>shp_texture;
		std::string reloadSceneName;
		UINT startCount = 0;
		TextureTag screenTextureTag;

		std::timespec befUpdateTs;
		std::timespec nowUpdateTs;
		long long int befUpdateTime;
		std::thread updateThread;
		std::timespec befDrawTs;
		std::timespec nowDrawTs;
		long long int befDrawTime;
	};
}
#endif // !_SceneManager_Edit_H_