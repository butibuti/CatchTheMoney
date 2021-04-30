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

		void SwitchPause() { pause = !pause; }
	private:
		bool pause;
	};

}

BUTI_REGIST_GAMECOMPONENT(PauseManager);