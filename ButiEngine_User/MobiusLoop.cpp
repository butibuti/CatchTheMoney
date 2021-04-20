#include "stdafx_u.h"
#include "MobiusLoop.h"
#include"GameSettings.h"

void ButiEngine::MobiusLoop::OnUpdate()
{
	float x = gameObject.lock()->transform->GetWorldPosition().x;
	if (x > GameSettings::windowWidth / 2.0f)
	{
		gameObject.lock()->transform->SetWorldPosition(wkp_left.lock()->transform->GetWorldPosition());
	}
	else if (x < -GameSettings::windowWidth / 2.0f)
	{
		gameObject.lock()->transform->SetWorldPosition(wkp_right.lock()->transform->GetWorldPosition());
	}
}

void ButiEngine::MobiusLoop::OnSet()
{
}

void ButiEngine::MobiusLoop::Start()
{
	std::string name = gameObject.lock()->GetGameObjectName();
	Vector3 localPosition = Vector3::Zero;
	auto tag = GetTagManager()->GetObjectTag("MapChip");
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();

	wkp_right = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Right");
	wkp_right.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	localPosition.x = GameSettings::windowWidth / GameSettings::blockSize;
	wkp_right.lock()->transform->SetLocalPosition(localPosition);
	wkp_right.lock()->SetGameObjectTag(tag);
	wkp_right.lock()->AddGameComponent(meshDraw->Clone());

	wkp_left = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Left");
	wkp_left.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	localPosition.x = -GameSettings::windowWidth / GameSettings::blockSize;
	wkp_left.lock()->transform->SetLocalPosition(localPosition);
	wkp_left.lock()->SetGameObjectTag(tag);
	wkp_left.lock()->AddGameComponent(meshDraw->Clone());
}

void ButiEngine::MobiusLoop::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::MobiusLoop::Clone()
{
	return ObjectFactory::Create<MobiusLoop>();
}
