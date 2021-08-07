#include "stdafx_u.h"
#include "HitChecker.h"

//BUTI_REGIST_GAMECOMPONENT(HitChecker)
void ButiEngine::HitChecker::OnUpdate()
{

}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::HitChecker::Clone()
{
    return ObjectFactory::Create<HitChecker>();
}

void ButiEngine::HitChecker::OnCollision(std::weak_ptr<GameObject> arg_other)
{
    hitObjName = arg_other.lock()->GetGameObjectName();
}

