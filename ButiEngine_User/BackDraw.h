#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseManager;

	class BackDraw :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "BackDraw";
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
		void SwitchGravityCore(bool arg_scroll);
	private:
		std::shared_ptr<PauseManager> shp_pauseManager;

		std::weak_ptr<GameObject> wkp_player;
		std::weak_ptr<GameObject> wkp_right;
		std::weak_ptr<GameObject> wkp_left;

		void StorePlayer();
		void Correction();
	};

}

BUTI_REGIST_GAMECOMPONENT(BackDraw);