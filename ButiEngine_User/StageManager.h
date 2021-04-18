#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Map;

	class StageManager :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "StageManager";
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
		std::shared_ptr<Map> shp_map;
	};

}

BUTI_REGIST_GAMECOMPONENT(StageManager);