#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class ParentSelectPanel :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "ParentSelectPanel";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Start()override;
		void OnCollision(std::weak_ptr<GameObject> arg_other)override;
		void ChildRotation(float rotate);
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}
	private:
		Vector3 rotation;
		std::vector<std::shared_ptr<GameObject>> childPanels;
	};

}

BUTI_REGIST_GAMECOMPONENT(ParentSelectPanel);