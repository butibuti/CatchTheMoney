#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Map;
	class PauseManager;
	class PanelManager;
	class ScrollManager;
	class CameraController;

	enum GameMode
	{
		Normal,
		Edit,
	};

	class StageManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "StageManager";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		GameMode GetMode() { return mode; }
	private:
		std::shared_ptr<Map> shp_map;
		std::shared_ptr<PauseManager> shp_pauseManager;
		std::shared_ptr<PanelManager> shp_panelManager;
		std::shared_ptr<ScrollManager> shp_scrollManager;
		std::shared_ptr<CameraController> shp_cameraController;

		std::weak_ptr<GameObject> wkp_player;

		GameMode mode;

		void ResetStage();
		void OnGoal();
		void ChangeScene(const std::string& arg_sceneName);
		void ModeChange();
	};

}

BUTI_REGIST_GAMECOMPONENT(StageManager);