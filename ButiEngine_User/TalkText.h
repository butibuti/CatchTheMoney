#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpliteAnimationComponent;

	class TalkText :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "TalkText";
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

		static bool IsDelete() { return isDelete; }
	private:
		std::shared_ptr<SpliteAnimationComponent> shp_spriteAnimation;
		
		int textCount;
		static bool isDelete;

		void Stage0Text();
	};

}

BUTI_REGIST_GAMECOMPONENT(TalkText);