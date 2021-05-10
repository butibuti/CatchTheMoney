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
		void SetNumber(int arg_num);
		void SetDigit(int arg_digit);
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		std::vector<std::shared_ptr<GameObject>> childNumbers;

		int number;
		int previousNumber;
		unsigned int digit;
		bool isOnce;

		unsigned int GetDigit(unsigned int arg_number);
		void CreateChildNum();
		void SetChildNum();
	};

}

BUTI_REGIST_GAMECOMPONENT(NumberComponent);