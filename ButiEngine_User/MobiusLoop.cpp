#include "stdafx_u.h"
#include "MobiusLoop.h"
#include"GameSettings.h"

void ButiEngine::MobiusLoop::OnUpdate()
{
	float x = gameObject.lock()->transform->GetWorldPosition().x;
	if (x > GameSettings::windowWidth / 2.0f)
	{
		gameObject.lock()->transform->SetWorldPosition(left.lock()->transform->GetWorldPosition());
	}
	else if (x < -GameSettings::windowWidth / 2.0f - GameSettings::blockSize)
	{
		gameObject.lock()->transform->SetWorldPosition(right.lock()->transform->GetWorldPosition());
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

	left = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Left");
	left.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	localPosition.x = -GameSettings::windowWidth / GameSettings::blockSize;
	left.lock()->transform->SetLocalPosition(localPosition);
	left.lock()->SetGameObjectTag(tag);
	left.lock()->AddGameComponent(meshDraw->Clone());

	right = GetManager().lock()->AddObject(std::make_shared<Transform>(), name + "_Right");
	right.lock()->transform->SetBaseTransform(gameObject.lock()->transform);
	localPosition.x = GameSettings::windowWidth / GameSettings::blockSize;
	right.lock()->transform->SetLocalPosition(localPosition);
	right.lock()->SetGameObjectTag(tag);
	right.lock()->AddGameComponent(meshDraw->Clone());
}

void ButiEngine::MobiusLoop::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::MobiusLoop::Clone()
{
	return ObjectFactory::Create<MobiusLoop>();
}
