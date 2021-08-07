#include "stdafx_u.h"
#include "BackGroundMobius.h"
#include"PauseManager.h"

void ButiEngine::BackGroundMobius::OnUpdate()
{
    if (PauseManager::IsPause()) { return; }
    time += timePase;
    zAxisRotate += 0.01f;
    auto t =Easing::GetEase(time, rotateEaseType) ;
    shp_parent->SetLocalRotation(MathHelper::LearpQuat(initRotate,targetRotate,t).ToMatrix());

    gameObject.lock()->transform->RollLocalRotationZ_Degrees(1.0f);
    if (time >1.0f) {
        time = 0.0f;
        initRotate = targetRotate;
        targetRotate =   initRotate* rotate;
    }

}

void ButiEngine::BackGroundMobius::OnSet()
{
}

void ButiEngine::BackGroundMobius::Start()
{
    rotate =   Quat().CreateFromEular_local_deg(euler);
    initRotate = gameObject.lock()->transform->GetLocalRotation().ToQuat();
    targetRotate = initRotate ;
    time = 0.0f;
    shp_parent = ObjectFactory::Create<Transform>();
    shp_parent->SetWorldPosition(gameObject.lock()->transform->GetWorldPosition());

    gameObject.lock()->transform->SetBaseTransform(shp_parent, true);
    gameObject.lock()->transform->SetLocalPosition(Vector3(0, 0, 0));
}

void ButiEngine::BackGroundMobius::OnShowUI()
{
    GUI::DragFloat3("Euler", euler, 0.01f, -180.0f, 180.0f);
    GUI::DragFloat("timepase", timePase, 0.01f, 0, 1.0f);

    if (GUI::TreeNode("EaseType1"))
    {
        GUI::Text(Easing::EasingTypeNames[(int)rotateEaseType]);

        if (GUI::ArrowButton("##EaseType_UP", GUI::GuiDir_Up)) {
            rotateEaseType = ((Easing::EasingType)(((int)rotateEaseType) + 1));
            if ((int)rotateEaseType > (int)Easing::EasingType::CubeRotate90) {
                rotateEaseType = Easing::EasingType::CubeRotate90;
            }
        }
        GUI::SameLine();
        if (GUI::ArrowButton("##EaseType_Down", GUI::GuiDir_Down)) {

            rotateEaseType = ((Easing::EasingType)(((int)rotateEaseType) - 1));
            if ((int)rotateEaseType < 0) {
                rotateEaseType = Easing::EasingType::EaseIn;
            }
        }
        GUI::TreePop();
    } 

}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::BackGroundMobius::Clone()
{
    auto output = ObjectFactory::Create<BackGroundMobius>();

    output->euler = euler;
    output->scale = scale;
    output->translate = translate;

    output->timePase = timePase;
    output->rotateEaseType = rotateEaseType;
    output->scaleEaseType = scaleEaseType;

    return output;
}
