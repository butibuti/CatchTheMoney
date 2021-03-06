#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class SquareParticleEmitter;
	class PauseManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "PauseManager";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void OnDecideReset();
		void SetPause(bool arg_flag) { pause = arg_flag; }
		static bool IsPause() { return pause; }
	private:
		const int ANIMATION_FRAME = 20;
		const int BACK = 0;
		const int RESET = 1;
		const int SELECT = 2;

		std::weak_ptr<GameObject> wkp_player;
		std::weak_ptr<GameObject> wkp_text;
		std::weak_ptr<GameObject> wkp_background;
		std::weak_ptr<GameObject> wkp_button_back;
		std::weak_ptr<GameObject> wkp_button_reset;
		std::weak_ptr<GameObject> wkp_button_select;

		std::shared_ptr<SquareParticleEmitter> shp_particleEmitter;

		bool isNext;
		static bool pause;
		bool pause_;
		bool disappear;
		bool pushPauseKey;
		int progress;
		int selectedButton;
		int fadeCount;
		bool reset;

		Vector3 initTextPos;
		Vector3 defaultTextPos;
		Vector3 initBGScale;
		Vector3 defaultBGScale;

		SoundTag se_enter;
		SoundTag se_select;
		SoundTag se_pick;

		void SwitchPause();
		void ButtonAnimation();
		void SelectButton();
		void OnDecide();
		void OnDecideBack();
		void OnDecideSelect();
		void AppearUI();
		void DisappearUI();
		void FadeUpdate();
		void ChangeScene(const std::string& arg_sceneName);
		void AddPositionAnimation(std::weak_ptr<GameObject> arg_object, const Vector3& arg_targetPosition, int frame, Easing::EasingType easingType);
		void AddScaleAnimation(std::weak_ptr<GameObject> arg_object, const Vector3& arg_targetScale, int frame, Easing::EasingType easingType);
		void StorePlayer();
	};

}

BUTI_REGIST_GAMECOMPONENT(PauseManager);