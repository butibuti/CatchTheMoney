#include "stdafx_u.h"
#include "AngelFrog.h"

void ButiEngine::AngelFrog::OnUpdate()
{
	if (life > 0)
	{
		life--;
	}
	else
	{
		//ƒ‰ƒCƒt‚ª‚È‚­‚È‚Á‚½‚ç”ò‚Î‚·
		gameObject.lock()->transform->SetLocalPosition(Vector3(0, 0, -3000));
		return;
	}
	
	Move();

}

void ButiEngine::AngelFrog::OnSet()
{
}

void ButiEngine::AngelFrog::Start()
{
	life = 60;
	gameObject.lock()->transform->SetLocalScale(Vector3(32, 32, 2));
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::AngelFrog::Clone()
{
	return ObjectFactory::Create<AngelFrog>();
}

void ButiEngine::AngelFrog::Move()
{


}
