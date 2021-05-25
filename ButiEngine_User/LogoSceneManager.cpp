#include "stdafx_u.h"
#include "LogoSceneManager.h"

void ButiEngine::LogoSceneManager::OnUpdate()
{
    gameObject.lock()->transform->SetLocalScale(gameObject.lock()->transform->GetLocalScale()*1.001f) ;
    logoTime += logoApperanceSpeed;
    shp_logoBuffer->Get().lightDir.w = Easing::Parabola(logoTime);
    if (shp_timer->Update()) {
        GetManager().lock()->AddObjectFromCereal("LogoSceneScreenObject", ObjectFactory::Create<Transform>(Vector3(0, 1080, 0.1), Vector3::Zero, Vector3(1920, 1080, 1)));
        

        shp_transTimer->Start();
    }
    if (shp_transTimer->Update()) {

        auto app = GetManager().lock()->GetApplication().lock();
        app->InitLoadResources();
        app->GetSceneManager()->LoadScene("Title");
        app->GetSceneManager()->ChangeScene("Title");
    }
}

void ButiEngine::LogoSceneManager::OnSet()
{
    if (!shp_timer) {
        shp_timer = ObjectFactory::Create<RelativeTimer>(60.0f);
    }
    if (!shp_transTimer) {
        shp_transTimer= ObjectFactory::Create<RelativeTimer>(60.0f);
    }
}

void ButiEngine::LogoSceneManager::Start()
{
    shp_logoBuffer = gameObject.lock()->GetGameComponent<MeshDrawComponent>()->GetCBuffer<LightVariable>("LightBuffer");
    shp_timer->Start();
}

void ButiEngine::LogoSceneManager::OnShowUI()
{
    if (GUI::TreeNode("Timer")) {
        shp_timer->ShowGUI();
        GUI::TreePop();
    }
    if (GUI::TreeNode("TransitionTimer")) {
        shp_transTimer->ShowGUI();
        GUI::TreePop();
    }
    GUI::BulletText("logoSpeed");
    GUI::DragFloat("##logoapp", logoApperanceSpeed, 0.001f);
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::LogoSceneManager::Clone()
{
    auto output = ObjectFactory::Create<LogoSceneManager>();
    if (shp_timer) {
        output->shp_timer = shp_timer->Clone();
    }if (shp_transTimer) {
        output->shp_transTimer = shp_transTimer->Clone();
    }
    return output;
}