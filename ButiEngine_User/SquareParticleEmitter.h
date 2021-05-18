#pragma once
#include"Header/GameComponentHeader.h"
namespace ButiEngine {
	
	class SquareParticleEmitter :public GameComponent
	{
		enum MoveMode {
			Move_TopLineFront,
			Move_TopLineBack,
			Move_LeftLine,
			Move_BottomLine,
			Move_RightLine,
		};
		struct FramePart {
			std::shared_ptr<GameObject> shp_object;
			float size;
			float time;
			MoveMode mode;
		};
	public:
		std::string GetGameComponentName()override {
			return "SquareParticleEmitter";
		}
		void OnUpdate()override;
		void OnSet()override;
		void Roll(const float arg_degrees);
		void SetRotation(const float arg_degrees);
		void Start()override;
		void OnShowUI()override;
		void SetIsEdit(const bool arg_isEdit);
		void SetIsPause(const bool arg_isPause);
		void EditUpdate();
		std::shared_ptr<GameComponent> Clone()override;
		template<class Archive>
		void serialize(Archive& archive)
		{
			archive(isActive);
			archive(thickness);
			archive(sizeMin);
			archive(sizeMax);
			archive(speedMin);
			archive(increase);
			archive(rotation);
			archive(radius);
		}
	private:
		void ParticleInformationSet(Particle3D& arg_refParticle);
		Vector3  center;
		float	thickness, sizeMin, sizeMax, speedMin,radius;
		Vector3 leftUpEdge, leftDownEdge, rightUpEdge, rightDownEdge;
		int increase = 0;
		float rotation;
		bool isEdit=false,isPause=false;
		std::vector <FramePart >vec_frameParts;
		std::vector<Vector3>vec_controlPoints;
	};

}

BUTI_REGIST_GAMECOMPONENT(SquareParticleEmitter);