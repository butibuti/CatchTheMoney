#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ParentDaikokuten :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ParentDaikokuten";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void TalkAppear();
		void Appear();
		void Disappear();
		void Reaction();
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		bool isChange;
		bool isOneLoop;
		int animationCount;
		Vector3 currentPos;
		Vector3 previousPos;

		void StayUpdate();
		void AppearUpdate();
		void ReactionUpdate();
		void RHandUpdate();
		void LHandUpdate();
		void RAppearUpdate();
		void LAppearUpdate();
		void ChangeTimer(const float arg_startPos, const float arg_endPos);
	};

}

BUTI_REGIST_GAMECOMPONENT(ParentDaikokuten);

