#include "stdafx_u.h"
#include "ParentSelectPanel.h"
#include "StageSelect.h"

void ButiEngine::ParentSelectPanel::OnUpdate()
{
}

void ButiEngine::ParentSelectPanel::OnSet()
{
}

void ButiEngine::ParentSelectPanel::Start()
{
	rotation = Vector3(0, 270, 0);
	int stageCount = (StageSelect::GetMaxStage() + 1) / 2; //LastStageNum * 0.5  "rewrite to StageSelect::maxStageNum"

	const float M_PI = 3.1415f;
	float angle = 360.0f / (float)stageCount;
	float mobiusAngle = 180.0f / (float)stageCount;
	auto transform = gameObject.lock()->transform; 

	for (unsigned int i = 0; i < stageCount; i++)
	{
		float radian = rotation.y * (M_PI / 180.0f);
		Vector3 result = Vector3(cos(radian), 0, sin(radian));
		result.Normalize();

		Vector3 position = result * 8.5f;
		Vector3 scale = Vector3(1, 1, 0.1f);
		Vector3 childRotation = Vector3(mobiusAngle * (float)i, -angle * (float)i, 0);
		
		childPanels.push_back(GetManager().lock()->AddObjectFromCereal("SelectPanel", ObjectFactory::Create<Transform>(position, childRotation, scale)).lock());
		
		rotation.y += angle;
	}
}

void ButiEngine::ParentSelectPanel::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::ParentSelectPanel::Clone()
{
	return ObjectFactory::Create<ParentSelectPanel>();
}

void ButiEngine::ParentSelectPanel::ChildRotation(float arg_rotateX)
{
	auto end = childPanels.end();
	for (auto itr = childPanels.begin(); itr != end; ++itr)
	{
		auto anim = (*itr)->GetGameComponent<TransformAnimation>();
		if (!anim)
		{
			anim = (*itr)->AddGameComponent<TransformAnimation>();
			anim->SetTargetTransform((*itr)->transform->Clone());
			anim->GetTargetTransform()->RollLocalRotationX_Degrees(arg_rotateX);
			anim->SetSpeed(0.15f);
			anim->SetEaseType(Easing::EasingType::EaseInOut);
		}
	}
}
