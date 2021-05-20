#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

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
		bool GetPause() { return pause; }
	private:
		const int ANIMATION_FRAME = 30;
		const int BACK = 0;
		const int RESET = 1;
		const int SELECT = 2;

		std::weak_ptr<GameObject> wkp_text;
		std::weak_ptr<GameObject> wkp_background;
		std::weak_ptr<GameObject> wkp_button_back;
		std::weak_ptr<GameObject> wkp_button_reset;
		std::weak_ptr<GameObject> wkp_button_select;

		bool isNext;
		bool pause;
		bool pause_;
		bool pushPauseKey;
		int progress;
		int selectedButton;
		int fadeCount;
		bool reset;

		Vector3 initTextPos;
		Vector3 defaultTextPos;
		Vector3 textScale;
		Vector3 initBGScale;
		Vector3 defaultBGScale;
		Vector3 BGPos;

		void SwitchPause();
		void ButtonAnimation();
		void SelectButton();
		void OnDecide();
		void OnDecideBack();
		void OnDecideReset();
		void OnDecideSelect();
		void AppearUI();
		void DisappearUI();
		void FadeUpdate();
		void ChangeScene(const std::string& arg_sceneName);
		void AddAnimation(std::weak_ptr<GameObject> arg_object, const Vector3& arg_targetPosition, const Vector3& arg_targetScale, int frame, Easing::EasingType easingType);
	};

}

BUTI_REGIST_GAMECOMPONENT(PauseManager);