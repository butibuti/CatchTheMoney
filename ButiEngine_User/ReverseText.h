#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ReverseText :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ReverseText";
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
		void PlayAnimation() 
		{
			play = true; 
			progress = 0.0f;
			progressFrame = 0;
		}
	private:
		const int SHOW_FRAME = 40;

		float x;
		float y;
		bool play;
		float progress;
		int progressFrame;

		void Animation();
	};

}

BUTI_REGIST_GAMECOMPONENT(ReverseText);