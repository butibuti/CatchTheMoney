#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ControlUI :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ControlUI";
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
		Vector3 initScale;
		float progress;
		bool animation;

		void Animation();
		void OnNoPush();
		void OnPushAnyKey();
	};

}

BUTI_REGIST_GAMECOMPONENT(ControlUI);