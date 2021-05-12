#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SceneChangeAnimation :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "SceneChangeAnimation";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		Vector3 position;
		float pointPosY;
	};

}

BUTI_REGIST_GAMECOMPONENT(SceneChangeAnimation);