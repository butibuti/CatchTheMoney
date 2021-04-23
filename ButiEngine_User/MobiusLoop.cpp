#include "stdafx_u.h"
#include "MobiusLoop.h"
#include"GameSettings.h"
#include"Player.h"

void ButiEngine::MobiusLoop::OnUpdate()
{
	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	Vector3 positionRight, positionLeft;
	positionRight.x = position.x + GameSettings::windowWidth * 0.5f;
	positionRight.y = -position.y;
	positionRight.z = position.z;
	positionLeft.x = position.x - GameSettings::windowWidth * 0.5f;
	positionLeft.y = -position.y;
	positionLeft.z = position.z;

	wkp_right.lock()->transform->SetWorldPosition(positionRight);
	wkp_left.lock()->transform->SetWorldPosition(positionLeft);

	if (position.x < -GameSettings::windowWidth * 0.25f)
	{
		gameObject.lock()->transform->SetWorldPosition(wkp_right.lock()->transform->GetWorldPosition());

		shp_player->ReverseGravity();
		wkp_right.lock()->transform->RollLocalRotationZ_Degrees(180.0f);
		wkp_left.lock()->transform->RollLocalRotationZ_Degrees(180.0f);
	}
	else if (position.x > GameSettings::windowWidth * 0.25f)
	{
		gameObject.lock()->transform->SetWorldPosition(wkp_left.lock()->transform->GetWorldPosition());

		shp_player->ReverseGravity();
		wkp_right.lock()->transform->RollLocalRotationZ_Degrees(180.0f);
		wkp_left.lock()->transform->RollLocalRotationZ_Degrees(180.0f);
	}
}

void ButiEngine::MobiusLoop::OnSet()
{
}

void ButiEngine::MobiusLoop::Start()
{
	shp_player = gameObject.lock()->GetGameComponent<Player>();

	auto tag = GetTagManager()->GetObjectTag("MapChip");
	auto meshDraw = gameObject.lock()->GetGameComponent<MeshDrawComponent>();
	std::string name = gameObject.lock()->GetGameObjectName();
	Vector3 position = gameObject.lock()->transform->GetWorldPosition();
	Vector3 scale = gameObject.lock()->transform->GetWorldScale();

	wkp_right = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(position, Vector3(0.0f, 0.0f, 180.0f), scale), name + "_Clone_Right");
	wkp_right.lock()->SetGameObjectTag(tag);
	wkp_right.lock()->AddGameComponent(meshDraw->Clone());

	wkp_left = GetManager().lock()->AddObject(ObjectFactory::Create<Transform>(position, Vector3(0.0f, 0.0f, 180.0f), scale), name + "_Clone_Left");
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
