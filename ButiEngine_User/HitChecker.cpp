#include "stdafx_u.h"
#include "HitChecker.h"

//BUTI_REGIST_GAMECOMPONENT(HitChecker)
void ButiEngine::HitChecker::OnUpdate()
{
    GetManager().lock()->GetApplication().lock()->GetGUIController()->vec_shp_GUIObjects.push_back(GetThis<IObject>());
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::HitChecker::Clone()
{
    return ObjectFactory::Create<HitChecker>();
}

void ButiEngine::HitChecker::OnCollision(std::weak_ptr<GameObject> arg_other)
{
    hitObjName = arg_other.lock()->GetGameObjectName();
}

void ButiEngine::HitChecker::ShowGUI()
{
    GUI::Begin("HitChecker");
    if (hitObjName.size() > 0) {

        GUI::BulletText("Hit:"+ hitObjName);
        hitObjName.clear();
    }
    GUI::End();
}
