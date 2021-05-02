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
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void ResetScroll();
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;

		std::weak_ptr<GameObject> wkp_player;
		std::weak_ptr<CBuffer<LightVariable>> wkp_screenScroll;
		float scrollSpeed;
		float previousScroll;
		float currentScroll;
		Vector3 scrollPosition;

		void MoveScroll();
		float* LoopDistance(float arg_prev, float arg_curr);
	};

}

BUTI_REGIST_GAMECOMPONENT(ScrollManager);