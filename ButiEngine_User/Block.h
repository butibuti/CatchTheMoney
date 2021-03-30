#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {

	class Block :public GameComponent
	{
	public:
		std::string GetGameComponentName()override {
			return "Block";
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

		void SetStartPoint(Vector2 arg_startPoint) { startPoint = arg_startPoint; }
		void SetEndPoint(Vector2 arg_endPoint) { endPoint = arg_endPoint; }
		void FinishCreate() { createFinished = true; }
	private:
		void Create();
		void Correction();

		bool createFinished;
		Vector2 startPoint;
		Vector2 endPoint;
		Vector2 drawStartPoint;
		Vector2 drawEndPoint;
	};

}

BUTI_REGIST_GAMECOMPONENT(Block);