#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Daikokuten :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "Daikokuten";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void Disappear();
		void Appear();
		void TalkDisappear();
		void TalkAppear();
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		bool isChange;
		bool isAppear;
		int moveCount;
		float initPos;
		float stayPos;
		float animationPos;
		float currentPos;
		float previousPos;
		float animationTime;

		void AppearAnimation();
		void AlwaysAnimation();
	};
}

BUTI_REGIST_GAMECOMPONENT(Daikokuten);