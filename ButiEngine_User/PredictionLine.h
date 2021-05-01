#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PredictionLine :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "PredictionLine";
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

	};

}

BUTI_REGIST_GAMECOMPONENT(PredictionLine);