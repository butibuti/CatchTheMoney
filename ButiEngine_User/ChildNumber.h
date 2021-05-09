#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class SpliteAnimationComponent;

	class ChildNumber :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ChildNumber";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		std::shared_ptr<GameComponent> Clone()override;
		void SetNumber(int arg_number);
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::shared_ptr<SpliteAnimationComponent> shp_spriteAnimation;

		int number;
		int previousNumber;
	};

}

BUTI_REGIST_GAMECOMPONENT(ChildNumber);