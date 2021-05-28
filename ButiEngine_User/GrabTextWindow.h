#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	class Player;
	class GrabTextWindow :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "GrabTextWindow";
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
	private:
		std::weak_ptr<GameObject> wkp_target;
		std::weak_ptr<GameObject> wkp_b;
		std::shared_ptr<Player> shp_player;
		
		std::shared_ptr<ICamera> shp_camera;
		std::weak_ptr<GameObject> wkp_screen;
		float currentScale;
		float previousScale;

		Vector3 grabObjectPos;

		Vector3 initPos;
		Matrix4x4 vpMatrix;
		

		void StorePlayer();
	};

}

BUTI_REGIST_GAMECOMPONENT(GrabTextWindow);