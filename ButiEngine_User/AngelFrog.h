#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class AngelFrog :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "AngelFrog";
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
		void SetBackFrog(std::weak_ptr<GameObject> arg_backFrog) { wkp_backFrog = arg_backFrog; }
		void SetDefaultGravity(bool arg_top);
		void SetUp(float arg_scaleY);
	private:
		std::weak_ptr<GameObject> wkp_backFrog;
		std::shared_ptr<CBuffer<ObjectInformation >>shp_alpha;
		int life;
		float accel,alphaTime;
		Vector3 velocity;

		void Move();
	};

}

BUTI_REGIST_GAMECOMPONENT(AngelFrog);