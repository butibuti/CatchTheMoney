#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class PauseButton :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "PauseButton";
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
		void Appear();
		void Disappear();
		void OnSelected();
		void OnEndSelect();
	private:
		Vector3 initPosition;
		Vector3 defaultPosition;
		Vector3 defaultScale;

		void AddPositionAnimation(const Vector3& arg_targetPosition, int frame, Easing::EasingType easingType);
		void AddScaleAnimation(const Vector3& arg_targetScale, int frame, Easing::EasingType easingType);
	};

}

BUTI_REGIST_GAMECOMPONENT(PauseButton);