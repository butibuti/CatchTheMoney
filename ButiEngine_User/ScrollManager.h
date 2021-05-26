#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class ScrollManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ScrollManager";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI() override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void ResetScroll();
		float GetCurrentScrollSwing()const {
			return previousScroll- currentScroll;
		}
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;

		std::weak_ptr<GameObject> wkp_player;
		std::weak_ptr<CBuffer<LightVariable>> wkp_screenScroll;
		std::weak_ptr<CBuffer<LightVariable>> wkp_sideScroll;
		float scrollSpeed;
		float previousScroll;
		float currentScroll;
		float currentScrollRight;
		float currentScrollLeft;
		bool isPreviousEdit;
		bool isCurrentEdit;
		Vector3 scrollPosition;

		void MoveScroll();
		void BackScroll();
		float* LoopDistance(const float arg_prev, const float arg_curr);
	};

}

BUTI_REGIST_GAMECOMPONENT(ScrollManager);