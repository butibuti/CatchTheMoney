#include "stdafx_u.h"
#include "ParentSelectPanel.h"
#include "StageSelect.h"
#include"Header/GameObjects/DefaultGameComponent/SpliteAnimationComponent.h"
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

	Vector3 scale = Vector3(1.5f, 1.5f, 0.1f);
	for (unsigned int i = 0; i < stageCount; i++)
	{
		float radian = rotation.y * (M_PI / 180.0f);
		Vector3 result = Vector3(cos(radian), 0, sin(radian));
		result.Normalize();

		Vector3 position = Vector3(0, 0, -8.5f);
		float yRotate = -angle * (float)i;
		Vector3 childRotation = Vector3(mobiusAngle * (float)i,0, 0);
		
		auto panel = GetManager().lock()->AddObjectFromCereal("SelectPanel", ObjectFactory::Create<Transform>(position, childRotation, scale)).lock();
		auto yAxis = GetManager().lock()->AddObject( ObjectFactory::Create<Transform>(0, 0, Vector3(1,1,1)), "yAxis").lock();

		panel->transform->SetBaseTransform(yAxis->transform,true);
		yAxis->transform->SetBaseTransform(gameObject.lock()->transform);
		yAxis->transform->RollLocalRotationY_Degrees(yRotate);

		childPanels.push_back(panel);
		auto front = GetManager().lock()->AddObjectFromCereal("SelectNumberPlane", ObjectFactory::Create<Transform>(Vector3(0, 0,- 0.5f))).lock();
		auto back = GetManager().lock()->AddObjectFromCereal("SelectNumberPlane", ObjectFactory::Create<Transform>(Vector3(0,0,0.5f),Vector3(180.0f,0.0f,0.0f),Vector3(1.0f))).lock();

		auto frontAnim = front->GetGameComponent<SpliteAnimationComponent>();
		auto backAnim= back->GetGameComponent<SpliteAnimationComponent>();
		frontAnim->SetHorizontalAnim(i % 10);
		frontAnim->SetVarticalAnim(i / 10);
		backAnim->SetHorizontalAnim((i+stageCount) % 10);
		backAnim->SetVarticalAnim((i + stageCount) / 10);
		front->transform->SetBaseTransform(panel->transform,true);
		back->transform->SetBaseTransform(panel->transform,true);

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

void ButiEngine::ParentSelectPanel::ChildRotation(const float arg_rotateX, const int arg_selectNum)
{
	auto end = childPanels.end();
	int i = 0;
	const int panelNum  = (StageSelect::GetMaxStage() + 1) / 2;;
	for (auto itr = childPanels.begin(); itr != end; ++itr,i++)
	{
		auto anim = (*itr)->GetGameComponent<TransformAnimation>();
		if (!anim)
		{
			anim = (*itr)->AddGameComponent<TransformAnimation>();
			anim->SetTargetTransform((*itr)->transform->Clone());
			anim->GetTargetTransform()->RollLocalRotationX_Degrees(arg_rotateX);
			if (arg_selectNum == i||arg_selectNum-panelNum==i) {
				anim->GetTargetTransform()->SetLocalScale(Vector3( 2.0f, 2.0f,0.5f));
			}
			else {
				anim->GetTargetTransform()->SetLocalScale(Vector3(1.5f,1.5f,0.1f));
			}

			anim->SetSpeed(0.15f);
			anim->SetEaseType(Easing::EasingType::EaseInOut);
		}
	}
}
