#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Map;
	class PauseManager;
	class PanelManager;
	class ScrollManager;
	class CameraController;
	class ClearBand;

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
		void ShowGUI() override;
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
		std::shared_ptr<MeshDrawComponent> shp_XMesh;
		std::shared_ptr<MeshDrawComponent> shp_EditMesh;
		std::shared_ptr<MeshDrawComponent> shp_CharaMesh;
		
		std::weak_ptr<GameObject> wkp_stageNumber;
		std::weak_ptr<GameObject> wkp_player;
		std::weak_ptr<GameObject> wkp_fadeObject;
		std::weak_ptr<GameObject> wkp_x;
		std::weak_ptr<GameObject> wkp_edit;
		std::weak_ptr<GameObject> wkp_chara;

		SoundTag bgm;
		SoundTag se_clear;

		GameMode mode;

		void ResetStage();
		int clearAnimationFrame;
		int fadeCount;
		bool isNext;
		const int CLEAR_FRAME = 90;
		Vector3 modeUIPosition;

		void OnGoal();
		void ChangeScene(const std::string& arg_sceneName);
		void ModeChange();
		void CreateUI();
		void ChangeUIAlpha();
	};

}

BUTI_REGIST_GAMECOMPONENT(StageManager);