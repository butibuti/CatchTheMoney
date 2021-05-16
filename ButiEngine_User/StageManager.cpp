#include "stdafx_u.h"
#include "StageManager.h"
#include"Map.h"
#include"PauseManager.h"
#include"CameraController.h"
#include"ScrollManager.h"
#include"InputManager.h"
#include"PanelManager.h"
#include"StageSelect.h"
#include "Player.h"
#include "ShakeComponent.h"
#include "ClearBand.h"
#include "NumberComponent.h"
#include"SquareParticleEmitter.h"
#include"GameSettings.h"
#include "SceneChangeAnimation.h"
#include"FollowPanel.h"
#include"Panel.h"
#include"ClearButton.h"
#include"ControlUI.h"
#include"ShakeComponent.h"

void ButiEngine::StageManager::OnUpdate()
{
	if (!wkp_player.lock())
	{
		wkp_player = GetManager().lock()->GetGameObject("Player");
		return;
	}

	if (wkp_player.lock()->GetGameComponent<Player>()->IsClear())
	{
		if (clearAnimationFrame >= CLEAR_FRAME)
		{
			GetManager().lock()->GetGameObject("Control2").lock()->GetGameComponent<ControlUI>()->Stop();
			GetManager().lock()->AddObjectFromCereal("ClearFlash", ObjectFactory::Create<Transform>(Vector3(0.0f,0.0f,1000.0f)));
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_clear, 0.1f);
		}
		else if (clearAnimationFrame == CLEAR_FRAME - 10)
		{
			GetManager().lock()->AddObjectFromCereal("ClearBand");
		}
		else if (clearAnimationFrame == CLEAR_FRAME - 30)
		{
			GetManager().lock()->AddObjectFromCereal("ClearText");
		}
		clearAnimationFrame--;
	}

	ResetStage();
	OnGoal();
	ModeChange();
	ChangeUIAlpha();

	GetManager().lock()->GetApplication().lock()->GetGUIController()->SetGUIObject(GetThis<StageManager>());

	if (mode == GameMode::Edit) {
		auto swing = shp_scrollManager->GetCurrentScrollSwing()*360;

		shp_particleEmitter->SetRotation(particleScrollOffset+swing);
	}
}

void ButiEngine::StageManager::OnSet()
{
}

void ButiEngine::StageManager::Start()
{
	fadeCount = 0;
	isNext = false;

	shp_map = GetManager().lock()->GetGameObject("Map").lock()->GetGameComponent<Map>();
	shp_pauseManager = GetManager().lock()->GetGameObject("PauseManager").lock()->GetGameComponent<PauseManager>();
	shp_panelManager = GetManager().lock()->GetGameObject("PanelManager").lock()->GetGameComponent<PanelManager>();
	shp_scrollManager = GetManager().lock()->GetGameObject("Screen").lock()->GetGameComponent<ScrollManager>();
	shp_cameraController = GetManager().lock()->GetGameObject("Camera").lock()->GetGameComponent<CameraController>();

	CreateUI();

	wkp_stageNumber = GetManager().lock()->AddObjectFromCereal("ParentNumber", ObjectFactory::Create<Transform>(Vector3(-550, 350, 0), Vector3::Zero, Vector3(80, 80, 0)));
	auto numComponent = wkp_stageNumber.lock()->GetGameComponent<NumberComponent>();
	numComponent->SetNumber(StageSelect::GetStageNum() + 1);

	if (StageSelect::GetStageNum() == 0)
	{
		wkp_talkText = GetManager().lock()->AddObjectFromCereal("TalkText", ObjectFactory::Create<Transform>(Vector3(0, -330, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		wkp_textWindow = GetManager().lock()->AddObjectFromCereal("TextWindow", ObjectFactory::Create<Transform>(Vector3(0, -330, -0.12f), Vector3::Zero, Vector3(1920, 640, 1)));
	}


	wkp_fadeObject = GetManager().lock()->AddObjectFromCereal("FadeObject2", ObjectFactory::Create<Transform>(Vector3(0, 0, -0.5f), Vector3::Zero, Vector3(1920, 1080, 1)));

	shp_map->PutTile();

	clearAnimationFrame = CLEAR_FRAME;

	mode = GameMode::Normal;
	shp_particleEmitter= GetManager().lock()->GetGameObject("SquareParticleEmitter").lock()->GetGameComponent<SquareParticleEmitter>();

	modeUIPosition = Vector3(750.0f, -410.0f, -0.1f);

	clearButtonAnimation = false;
	selectedNext = true;

	for (int i = 0; i < StageSelect::GetMaxStage(); i++)
	{
		std::string stageName = "Stage" + std::to_string(i);
		if (stageName == StageSelect::GetRemoveStageName())
		{
			auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
			sceneManager->RemoveScene(stageName);
		}
	}

	bgm = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/BGM.wav");
	se_clear = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Clear.wav");

	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlayBGM(bgm, 0.1f);

	nextSceneName = "Stage" + std::to_string(StageSelect::GetStageNum() + 1);
}

void ButiEngine::StageManager::ShowGUI()
{
}

void ButiEngine::StageManager::OnCollision(std::weak_ptr<GameObject> arg_other)
{
}

std::shared_ptr<ButiEngine::GameComponent> ButiEngine::StageManager::Clone()
{
	return ObjectFactory::Create<StageManager>();
}

void ButiEngine::StageManager::ResetStage()
{
	if (InputManager::OnTriggerOpenMenuKey())
	{
		shp_map->DestoryBlock();
		auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
		sceneManager->ReloadScene();
	}
}

void ButiEngine::StageManager::OnGoal()
{
	if (clearAnimationFrame == 0)
	{
		shp_buttonNext->Appear();
		if (StageSelect::GetStageNum() != StageSelect::GetMaxStage())
		{
			shp_buttonSelect->Appear();
		}
	}
	else if (!clearButtonAnimation && clearAnimationFrame < 0 && 
		!wkp_buttonNext.lock()->GetGameComponent<TransformAnimation>())
	{
		clearButtonAnimation = true;
		shp_buttonNext->OnSelected();
	}
	ClearButtonUpdate();
	if (isNext)
	{
		fadeCount++;
	}
	if (fadeCount == 1)
	{
		GetManager().lock()->AddObjectFromCereal("FadeObject2", ObjectFactory::Create<Transform>(Vector3(0, 1080, -0.5f), Vector3::Zero, Vector3(1920, 1080, 1)));
	}
	if (fadeCount > 30)
	{
		shp_map->DestoryBlock();
		//ChangeScene("StageSelect");
		//int nextStageNum = StageSelect::GetStageNum() + 1;
		//StageSelect::SetStageNum(nextStageNum);
		
		int nextStageNum = StageSelect::GetStageNum() + 1;
		if (nextStageNum > 11)
		{
			StageSelect::SetRemoveStageName("none");
			std::string sceneName = "StageSelect";
			ChangeScene(sceneName);
		}
		else
		{
			std::string sceneName = "Stage" + std::to_string(StageSelect::GetStageNum());
			if (nextSceneName == "StageSelect")
			{
				sceneName = "none";
			}
			StageSelect::SetRemoveStageName(sceneName);
			ChangeScene(nextSceneName);
		}

		StageSelect::SetStageNum(nextStageNum);
	}
	if (GameDevice::GetInput()->TriggerKey(Keys::C))
	{
		StageSelect::SetRemoveStageName("none");
		shp_map->DestoryBlock();
		ChangeScene("StageSelect");
	}
}

void ButiEngine::StageManager::ChangeScene(const std::string& arg_sceneName)
{
	auto sceneManager = gameObject.lock()->GetApplication().lock()->GetSceneManager();
	sceneManager->RemoveScene(arg_sceneName);
	sceneManager->LoadScene(arg_sceneName);
	sceneManager->ChangeScene(arg_sceneName);
}

void ButiEngine::StageManager::ModeChange()
{
	if (InputManager::OnTriggerModeChangeKey())
	{
		if (shp_cameraController->IsAnimation()) { return; }
		if (shp_panelManager->IsAnimation()) { return; }
		if (StageSelect::GetStageNum() == 0) { return; }
		if (wkp_player.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel().lock()->GetGameComponent<Panel>()->IsLock()) { return; }

		shp_pauseManager->SwitchPause();
		shp_scrollManager->ResetScroll();
		
		if (mode == GameMode::Normal)
		{
			mode = GameMode::Edit;

			wkp_edit.lock()->transform->TranslateZ(1000);
			wkp_chara.lock()->transform->SetWorldPosition(modeUIPosition);

			shp_cameraController->ZoomOut();
			shp_panelManager->ResetMoveNum();
			shp_panelManager->ResetMoveHistories();

			shp_particleEmitter->SetIsEdit(true);

			auto nowPosX = wkp_player.lock()->transform->GetWorldPosition();
			particleScrollOffset = nowPosX.x-shp_panelManager->GetClosestPanel(nowPosX.x).lock()->transform->GetWorldPosition().x;

			particleScrollOffset = 360 * (particleScrollOffset / (float)GameSettings::windowWidth);

		}
		else
		{
			mode = GameMode::Normal;
			shp_cameraController->ZoomIn(); 

			shp_particleEmitter->SetIsEdit(false);

			wkp_chara.lock()->transform->TranslateZ(1000);
			wkp_edit.lock()->transform->SetWorldPosition(modeUIPosition);

			shp_cameraController->ZoomIn();
		}
	}
}

void ButiEngine::StageManager::CreateUI()
{
	int stageNum = StageSelect::GetStageNum();

	if (stageNum == 0)
	{
		GetManager().lock()->AddObjectFromCereal("Control");
	}
	else
	{
		wkp_x = GetManager().lock()->AddObjectFromCereal("X");
		wkp_edit = GetManager().lock()->AddObjectFromCereal("Edit");
		wkp_chara = GetManager().lock()->AddObjectFromCereal("Chara");

		shp_XMesh = wkp_x.lock()->GetGameComponent<MeshDrawComponent>();
		shp_EditMesh = wkp_edit.lock()->GetGameComponent<MeshDrawComponent>();
		shp_CharaMesh = wkp_chara.lock()->GetGameComponent<MeshDrawComponent>();

		wkp_chara.lock()->transform->TranslateZ(1000);
	}

	wkp_buttonNext = GetManager().lock()->AddObjectFromCereal("Button_Next");
	wkp_buttonSelect = GetManager().lock()->AddObjectFromCereal("Button_Select");
	shp_buttonNext = wkp_buttonNext.lock()->GetGameComponent<ClearButton>();
	shp_buttonSelect = wkp_buttonSelect.lock()->GetGameComponent<ClearButton>();

	if (stageNum >= 9)
	{
		wkp_grab = GetManager().lock()->AddObjectFromCereal("BTextWindow");
	}
}

void ButiEngine::StageManager::ChangeUIAlpha()
{
	bool isLock = wkp_player.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel().lock()->GetGameComponent<Panel>()->IsLock();

	float alpha = 1.0f;
	if (isLock)
	{
		alpha = 0.5f;
	}
	if (shp_XMesh && shp_EditMesh && shp_CharaMesh)
	{
		shp_XMesh->GetCBuffer<LightVariable>("LightBuffer")->Get().lightDir.w = alpha;
		shp_EditMesh->GetCBuffer<LightVariable>("LightBuffer")->Get().lightDir.w = alpha;
		shp_CharaMesh->GetCBuffer<LightVariable>("LightBuffer")->Get().lightDir.w = alpha;
	}
}

void ButiEngine::StageManager::ClearButtonUpdate()
{
	if (!clearButtonAnimation) { return; }

	if ((InputManager::OnTriggerRightKey() || InputManager::OnTriggerLeftKey()) &&
		StageSelect::GetStageNum() != StageSelect::GetMaxStage())
	{
		selectedNext = !selectedNext;
		if (selectedNext)
		{
			shp_buttonNext->OnSelected();
			shp_buttonSelect->OnEndSelect();
			nextSceneName = "Stage" + std::to_string(StageSelect::GetStageNum() + 1);
		}
		else
		{
			shp_buttonNext->OnEndSelect();
			shp_buttonSelect->OnSelected();
			nextSceneName = "StageSelect";
		}
	}

	if (InputManager::OnTriggerDecisionKey())
	{
		isNext = true;
		if (selectedNext)
		{
			wkp_buttonNext.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(10.0f);
		}
		else
		{
			wkp_buttonSelect.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(10.0f);
		}
	}
}
