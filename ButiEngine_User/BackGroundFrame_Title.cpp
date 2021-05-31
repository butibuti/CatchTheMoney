#include "stdafx_u.h"
#include "BackGroundFrame_Title.h"
#include "GameSettings.h"
#include"PauseManager.h"


void ButiEngine::BackGroundFrame_Title::OnUpdate()
{
	if (PauseManager::IsPause()) { return; }
	Move();
	Rotation();
	if (shp_sucideTimer->Update()) {
		gameObject.lock()->SetIsRemove(true);
	}
}

void ButiEngine::BackGroundFrame_Title::OnSet()
{

	SetVelocityRandom();
	SetScaleRandom();
}

void ButiEngine::BackGroundFrame_Title::Start()
{
	shp_sucideTimer = ObjectFactory::Create<RelativeTimer>(36);
	shp_sucideTimer->Start();
}

void ButiEngine::BackGroundFrame_Title::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::BackGroundFrame_Title::Clone()
{
    return ObjectFactory::Create<BackGroundFrame_Title>();
}

void ButiEngine::BackGroundFrame_Title::SetVelocityRandom()
{
	float speed = ButiRandom::GetRandom(20, 150, 1);
	if (speed == 0) {
		speed = 30;
	}

	velocity.x = ButiRandom::GetRandom(-10, 10, 1);
	velocity.y = ButiRandom::GetRandom(-10, 10, 1);
	velocity.z = 0;
	if (velocity.x == 0 && velocity.y == 0) {
		velocity.y = -1;
		velocity.x = 1.0f;
	}

	velocity.Normalize();
	velocity *= speed;
	float scaleX = gameObject.lock()->transform->GetWorldScale().x;
	const float MAX_SCALE = 960.0f;
	addAngle = velocity.x * MAX_SCALE / scaleX * -0.125f;
	Move();
}

void ButiEngine::BackGroundFrame_Title::SetScaleRandom()
{
	float scale = ButiRandom::GetRandom(120, 480, 1);

	gameObject.lock()->transform->SetLocalScale(Vector3(scale, scale, 1));
}

void ButiEngine::BackGroundFrame_Title::Move()
{
	gameObject.lock()->transform->Translate(velocity);

}

void ButiEngine::BackGroundFrame_Title::Rotation()
{
	gameObject.lock()->transform->RollWorldRotationZ_Degrees(addAngle);
}
