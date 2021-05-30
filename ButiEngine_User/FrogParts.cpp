#include "stdafx_u.h"
#include "FrogParts.h"
#include"Frog.h"

void ButiEngine::FrogParts::OnUpdate()
{
	if (life > 0)
	{
		life--;
	}
	else
	{
		//ƒ‰ƒCƒt‚ª‚È‚­‚È‚Á‚½‚ç”ò‚Î‚·
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, -3000, 0));
		return;
	}

	Move();
	Rotation();
}

void ButiEngine::FrogParts::OnSet()
{
}

void ButiEngine::FrogParts::Start()
{
	velocity = Vector3::Zero;
	gravity = 0.0f;
	life = 0;
}

void ButiEngine::FrogParts::OnShowUI()
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::FrogParts::Clone()
{
	return ObjectFactory::Create<FrogParts>();
}

void ButiEngine::FrogParts::Explosion(const Vector3& arg_position)
{
	life = 180;
	velocity.x = ButiRandom::GetRandom(-2.0f, 2.0f, 100);
	velocity.y = ButiRandom::GetRandom(-7.0f, 7.0f, 100);
	velocity.z = 0;
	gravity = wkp_frog.lock()->GetGameComponent<Frog>()->GetGravity() * 1.2f;
	gameObject.lock()->transform->SetWorldPosition(arg_position);
}

void ButiEngine::FrogParts::Move()
{
	velocity.y += gravity;

	gameObject.lock()->transform->Translate(velocity * 1.75f);
}

void ButiEngine::FrogParts::Rotation()
{
	gameObject.lock()->transform->RollLocalRotationZ_Degrees(velocity.x * 3.0f);
}
