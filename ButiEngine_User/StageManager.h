#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Map;
	class PauseManager;
	class PanelManager;
	class ScrollManager;
	class CameraController;
	class ClearBand;
	class SquareParticleEmitter;
	class ClearButton;
	enum GameMode
	{
		Chara,
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
		static GameMode GetMode() { return mode; }
	private:
		std::shared_ptr<Map> shp_map;
		std::shared_ptr<PauseManager> shp_pauseManager;
		std::shared_ptr<PanelManager> shp_panelManager;
		std::shared_ptr<ScrollManager> shp_scrollManager;
		std::shared_ptr<CameraController> shp_cameraController;
		std::shared_ptr<MeshDrawComponent> shp_XMesh;
		std::shared_ptr<MeshDrawComponent> shp_switchDaikokutenMesh;
		std::shared_ptr<MeshDrawComponent> shp_switchNezumiMesh;
		std::shared_ptr<ClearButton> shp_buttonNext;
		std::shared_ptr<ClearButton> shp_buttonSelect;
		
		std::shared_ptr< SquareParticleEmitter> shp_particleEmitter;

		std::weak_ptr<GameObject> wkp_stageNumber;
		std::weak_ptr<GameObject> wkp_player;
		std::weak_ptr<GameObject> wkp_frog;
		std::weak_ptr<GameObject> wkp_x;
		std::weak_ptr<GameObject> wkp_switchDaikokuten;
		std::weak_ptr<GameObject> wkp_switchNezumi;
		std::weak_ptr<GameObject> wkp_grab;
		std::weak_ptr<GameObject> wkp_buttonNext;
		std::weak_ptr<GameObject> wkp_buttonSelect;
		std::weak_ptr<GameObject> wkp_textWindow;
		std::weak_ptr<GameObject> wkp_RotateButtonUI;

		std::weak_ptr<GameObject> wkp_daikokutenHead;
		std::weak_ptr<GameObject> wkp_daikokutenHandLeft;
		std::weak_ptr<GameObject> wkp_daikokutenHandRight;

		std::weak_ptr<GameObject> wkp_daikokutenStay;
		std::weak_ptr<GameObject> wkp_daikokutenAppear;
		std::weak_ptr<GameObject> wkp_daikokutenReaction;
		std::weak_ptr<GameObject> wkp_daikokutenRHand;
		std::weak_ptr<GameObject> wkp_daikokutenLHand;
		std::weak_ptr<GameObject> wkp_daikokutenRAppear;
		std::weak_ptr<GameObject> wkp_daikokutenLAppear;

		std::weak_ptr<GameObject> wkp_tutorialNo;
		std::weak_ptr<GameObject> wkp_tutorialYes;
		std::weak_ptr<GameObject> wkp_tutorialTextWindow;
		std::weak_ptr<GameObject> wkp_reverseCheckText;

		SoundTag bgm;
		SoundTag se_clear;
		SoundTag se_enter;
		SoundTag se_select;
		SoundTag se_panelMode;
		SoundTag se_charaMode;

		static GameMode mode;

		int clearAnimationFrame;
		int fadeCount;
		int frogEatAnimationCount;
		bool isNext;
		bool isOnce;
		bool isAddTutorial;
		bool isTutorialSelect;
		const int CLEAR_FRAME = 90;
		float particleScrollOffset;
		Vector3 defaultModeUIScale;
		bool clearButtonAnimation;
		bool selectedNext;
		std::string nextSceneName;

		void OnGoal();
		void ChangeScene(const std::string& arg_sceneName);
		void ModeChange();
		void CreateUI();
		void ChangeUIAlpha();
		void ClearButtonUpdate();
		void CommonTextObject();
		void FrogEatAnimation();
		void StorePlayer();
		void StoreFrog();
		void TutorialMode();
		void AddTutorialUI();
		void AddClearAnimation();
		void AddTalkText();
		void SetEditMode();
		void SetCharaMode();
	};

}

BUTI_REGIST_GAMECOMPONENT(StageManager);