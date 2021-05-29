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
		void Reaction(bool arg_isReactionScale);
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		bool isChange;
		bool isOneLoop;
		bool isReactionScale;
		bool isActiveChange;
		int animationCount;
		float movePos;
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
		void LoopAnimation(const float arg_startPos, const float arg_endPos, bool arg_isX);
	};

}

BUTI_REGIST_GAMECOMPONENT(ParentDaikokuten);

