#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Title :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "Title";
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
	private:
		std::weak_ptr<GameObject> wkp_fadeObject;
		bool nextFlag;
		int nextSceneCount;
	};

}

BUTI_REGIST_GAMECOMPONENT(Title);