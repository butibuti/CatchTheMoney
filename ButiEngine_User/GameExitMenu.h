#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class SquareParticleEmitter;
	class GameExitMenu :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "GameExitMenu";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		static bool IsOpenMenu() { return open; }
	private:
		const int ANIMATION_FRAME = 20;
		const int EXIT_WAIT_FRAME = 60;

		std::shared_ptr<PauseManager> shp_pauseManager;

		std::weak_ptr<GameObject> wkp_player;
		std::weak_ptr<GameObject> wkp_text;
		std::weak_ptr<GameObject> wkp_background;
		std::weak_ptr<GameObject> wkp_button_no;
		std::weak_ptr<GameObject> wkp_button_yes;

		static bool open;
		bool open_;
		bool disappear;
		bool pushMenuKey;
		int progress;
		bool selectYesButton;
		bool exit;
		int exitProgressFrame;

		Vector3 initTextPos;
		Vector3 defaultTextPos;
		Vector3 initBGScale;
		Vector3 defaultBGScale;

		SoundTag se_enter;
		SoundTag se_select;
		SoundTag se_pick;

		std::shared_ptr<SquareParticleEmitter> shp_particleEmitter;
		void SwitchPause();
		void ButtonAnimation();
		void SelectButton();
		void OnDecide();
		void OnDecideNo();
		void OnDecideYes();
		void AppearUI();
		void DisappearUI();
		void AddPositionAnimation(std::weak_ptr<GameObject> arg_object, const Vector3& arg_targetPosition, int frame, Easing::EasingType easingType);
		void AddScaleAnimation(std::weak_ptr<GameObject> arg_object, const Vector3& arg_targetScale, int frame, Easing::EasingType easingType);
		void StorePlayer();
		void Exit();
	};

}

BUTI_REGIST_GAMECOMPONENT(GameExitMenu);