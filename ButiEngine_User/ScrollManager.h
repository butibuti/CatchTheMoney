#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Player;

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
	private:
		void MoveScroll();

		std::weak_ptr<GameObject> weak_player;
		std::weak_ptr<CBuffer<LightVariable>> wkp_screenScroll;
		bool mode;
		float scrollSpeed;
		Vector3 scrollPosition;
	};

}

BUTI_REGIST_GAMECOMPONENT(ScrollManager);