#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class MobiusMover :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "MobiusMover";
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

		void SetProgress(float arg_progress) { progress = arg_progress; }
	private:
		float progress;
	};

}

BUTI_REGIST_GAMECOMPONENT(MobiusMover);