#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class NumberComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "NumberComponent";
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
		int number;
		int digit;

		unsigned int GetDigit(unsigned int arg_number);
	};

}

BUTI_REGIST_GAMECOMPONENT(NumberComponent);