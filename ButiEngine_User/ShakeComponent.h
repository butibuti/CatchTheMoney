#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ShakeComponent :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ShakeComponent";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void ShakeStart(float arg_amplitude);
		void ShakeStart(float arg_amplitude, int arg_frame);
		void ShakeStop();
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		bool start;
		float amplitude;
		float subAmplitude;
		Vector3 defaultRotate;
	};

}

BUTI_REGIST_GAMECOMPONENT(ShakeComponent);