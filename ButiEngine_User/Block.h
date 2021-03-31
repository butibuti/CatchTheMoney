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
		void OnCollisionEnter(std::weak_ptr<GameObject> arg_other)override;
		void OnCollisionEnd(std::weak_ptr<GameObject> arg_other)override;
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
		}

		void SetStartPoint(Vector2 arg_startPoint) { startPoint = arg_startPoint; }
		void SetEndPoint(Vector2 arg_endPoint) { endPoint = arg_endPoint; }
		void FinishCreate();
		void SetClone(bool arg_clone) { clone = arg_clone; }
	private:
		void Create();
		void Correction();
		void OnOutScreen();
		void CreateClone();

		bool clone;
		std::weak_ptr<GameObject> wkp_clone;

		bool createable;
		bool createFinished;
		Vector2 startPoint;
		Vector2 endPoint;
		Vector2 drawStartPoint;
		Vector2 drawEndPoint;
	};

}

BUTI_REGIST_GAMECOMPONENT(Block);