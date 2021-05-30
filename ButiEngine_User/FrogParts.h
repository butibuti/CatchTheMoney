#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class FrogParts :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "FrogParts";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnShowUI()override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
		void SetFrog(std::weak_ptr<GameObject> arg_frog) { wkp_frog = arg_frog; }
		void Explosion(const Vector3& arg_position);
	private:
		std::weak_ptr<GameObject> wkp_frog;

		Vector3 velocity;
		float gravity;
		int life;

		void Move();
		void Rotation();
	};

}

BUTI_REGIST_GAMECOMPONENT(FrogParts);