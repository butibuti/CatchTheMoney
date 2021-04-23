#include "stdafx_u.h"
#include "MobiusLoop.h"
#include"GameSettings.h"

void ButiEngine::MobiusLoop::OnUpdate()
{
	float x = gameObject.lock()->transform->GetWorldPosition().x;

	if (x < -GameSettings::windowWidth * 0.25f)
	{
		gameObject.lock()->transform->SetWorldPosition(wkp_right.lock()->transform->GetWorldPosition());
	}
	else if (x > GameSettings::windowWidth * 0.25f)
	{
		gameObject.lock()->transform->SetWorldPosition(wkp_left.lock()->transform->GetWorldPosition());
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

	wkp_right.lock()->transform->TranslateX(64.0f);

	wkp_right.lock()->transform->TranslateX(64.0f);
}

void ButiEngine::MobiusLoop::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::MobiusLoop::Clone()
{
	return ObjectFactory::Create<MobiusLoop>();
}
