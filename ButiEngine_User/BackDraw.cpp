#include "stdafx_u.h"
#include "BackDraw.h"
#include"GameSettings.h"

void ButiEngine::BackDraw::OnUpdate()
{
	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	Vector3 clonePosition = Vector3::Zero;
	Vector3 scale = gameObject.lock()->transform->GetWorldScale();
	Matrix4x4 rotation = gameObject.lock()->transform->GetWorldRotation();
	clonePosition.x = position.x + GameSettings::windowWidth * 0.5f;
	clonePosition.y = -position.y;
	clonePosition.z = position.z;
	wkp_right.lock()->transform->SetWorldPosition(clonePosition);
	wkp_right.lock()->transform->SetLocalScale(scale);
	wkp_right.lock()->transform->SetWorldRotation(rotation);
	wkp_right.lock()->transform->RollLocalRotationX_Degrees(180.0f);
	clonePosition.x = position.x - GameSettings::windowWidth * 0.5f;
	wkp_left.lock()->transform->SetWorldPosition(clonePosition);
	wkp_left.lock()->transform->SetLocalScale(scale);
	wkp_left.lock()->transform->SetWorldRotation(rotation);
	wkp_left.lock()->transform->RollLocalRotationX_Degrees(180.0f);
}

void ButiEngine::BackDraw::OnSet()
{
}

void ButiEngine::BackDraw::Start()
{
	std::string name = gameObject.lock()->GetGameObjectName();
	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	Vector3 clonePosition = Vector3::Zero;
	Vector3 scale = gameObject.lock()->transform->GetWorldScale();
	Matrix4x4 rotation = gameObject.lock()->transform->GetWorldRotation();
	auto tag = GetTagManager()->GetObjectTag("MapChip");
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	if (!meshDraw)
	{
		meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent_Static>();
	}

	wkp_right = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Back_Right");
	clonePosition.x = position.x + GameSettings::windowWidth * 0.5f;
	clonePosition.y = -position.y;
	clonePosition.z = position.z;
	wkp_right.lock()->transform->SetWorldPosition(clonePosition);
	wkp_right.lock()->transform->SetLocalScale(scale);
	wkp_right.lock()->transform->SetWorldRotation(rotation);
	wkp_right.lock()->transform->RollLocalRotationX_Degrees(180.0f);
	wkp_right.lock()->SetGameObjectTag(tag);
	if (meshDraw)
	{
		wkp_right.lock()->AddGameComponent(meshDraw->Clone());
	}

	wkp_left = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Back_Left");
	clonePosition.x = position.x - GameSettings::windowWidth * 0.5f;
	wkp_left.lock()->transform->SetWorldPosition(clonePosition);
	wkp_left.lock()->transform->SetLocalScale(scale);
	wkp_left.lock()->transform->SetWorldRotation(rotation);
	wkp_left.lock()->transform->RollLocalRotationX_Degrees(180.0f);
	wkp_left.lock()->SetGameObjectTag(tag);
	if (meshDraw)
	{
		wkp_left.lock()->AddGameComponent(meshDraw->Clone());
	}
}

void ButiEngine::BackDraw::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::BackDraw::Clone()
{
	return ObjectFactory::Create<BackDraw>();
}
