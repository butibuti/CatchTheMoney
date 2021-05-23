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
#include "TalkText.h"

//#define OUTPUT_STAGERENDERTARGET
#ifdef DEBUG

static int f = 0;
#endif // DEBUG

ButiEngine::GameMode ButiEngine::StageManager::mode;

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
			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_clear, GameSettings::masterVolume);
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

	OnGoal();
	ModeChange();
	ChangeUIAlpha();

	GetManager().lock()->GetApplication().lock()->GetGUIController()->SetGUIObject(GetThis<StageManager>());

	if (mode == GameMode::Edit) {
		auto swing = shp_scrollManager->GetCurrentScrollSwing()*360;

		shp_particleEmitter->SetRotation(particleScrollOffset+swing);
	}

#ifdef OUTPUT_STAGERENDERTARGET 
#ifdef DEBUG

	if (f==3) {
		const int stageNum = StageSelect::GetStageNum();

		std::string filePath = "Texture/StageSelect/stage_";


		if (stageNum < 10) {
			filePath += "0";
		}

		filePath += std::to_string(stageNum) + ".png";
		TextureTag output = TextureTag(":/wideScreen/1280/144");
		auto texture = GetManager().lock()->GetApplication().lock()->GetResourceContainer()->GetTexture(output).lock();
		if (texture && !GameSettings::isTitle) {
			texture->ToPNG(GlobalSettings::GetResourceDirectory() + filePath);
		}
		f = 0;
	}
	if (f > 0) {
		f++;
	}
#endif // DEBUG
#endif // OUTPUT_STAGERENDERTARGET
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

	if (!GameSettings::isTitle)
	{
		wkp_stageNumber = GetManager().lock()->AddObjectFromCereal("ParentNumber", ObjectFactory::Create<Transform>(Vector3(-550, 350, 0), Vector3::Zero, Vector3(80, 80, 0)));
		auto numComponent = wkp_stageNumber.lock()->GetGameComponent<NumberComponent>();
		numComponent->SetNumber(StageSelect::GetStageNum() + 1);
	}

	if (StageSelect::GetStageNum() == 0 && !GameSettings::isTitle)
	{
		wkp_talkText = GetManager().lock()->AddObjectFromCereal("FirstTalkText", ObjectFactory::Create<Transform>(Vector3(0, -330, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		CommonTextObject();
	}
	else if (StageSelect::GetStageNum() == 3 && !GameSettings::isTitle)
	{
		wkp_talkText = GetManager().lock()->AddObjectFromCereal("ReverseTalkText", ObjectFactory::Create<Transform>(Vector3(0, -330, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		CommonTextObject();
	}
	else if (StageSelect::GetStageNum() == 6 && !GameSettings::isTitle)
	{
		wkp_talkText = GetManager().lock()->AddObjectFromCereal("GravityTalkText", ObjectFactory::Create<Transform>(Vector3(0, -330, -0.14f), Vector3::Zero, Vector3(1808, 315, 1)));
		CommonTextObject();
	}
	else
	{
		TalkText::Delete();
	}

	wkp_fadeObject = GetManager().lock()->AddObjectFromCereal("FadeObject2", ObjectFactory::Create<Transform>(Vector3(0, 0, -0.7f), Vector3::Zero, Vector3(1920, 1080, 1)));

	shp_map->PutTile();

	clearAnimationFrame = CLEAR_FRAME;

	mode = GameMode::Chara;
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
	se_enter = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Enter.wav");
	se_select = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Select-Click.wav");
	se_panelMode = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Panel_Pick.wav");
	//se_charaMode = gameObject.lock()->GetResourceContainer()->GetSoundTag("Sound/Panel_Pick.wav");

	GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlayBGM(bgm, 0.1f);
	const int stageNum = StageSelect::GetStageNum();
	nextSceneName = "Stage" + std::to_string(stageNum + 1);

#ifdef DEBUG
	f = 1;
#endif // DEBUG


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

void ButiEngine::StageManager::OnGoal()
{
	if (clearAnimationFrame == 30)
	{
		shp_buttonNext->Appear();
		if (StageSelect::GetStageNum() != StageSelect::GetMaxStage())
		{
			shp_buttonSelect->Appear();
		}
	}
	else if (!clearButtonAnimation && clearAnimationFrame < 0)
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
		GetManager().lock()->AddObjectFromCereal("FadeObject2", ObjectFactory::Create<Transform>(Vector3(0, 1080, -0.7f), Vector3::Zero, Vector3(1920, 1080, 1)));
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
#ifdef _DEBUG
	if (GameDevice::GetInput()->TriggerKey(Keys::C))
	{
		StageSelect::SetRemoveStageName("none");
		shp_map->DestoryBlock();
		ChangeScene("StageSelect");
	}
#endif
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
		if (shp_pauseManager->IsPause()) { return; }
		if (StageSelect::GetStageNum() == 0) { return; }
		if (wkp_player.lock()->GetGameComponent<FollowPanel>()->GetClosestPanel().lock()->GetGameComponent<Panel>()->IsLock()) { return; }
		if (wkp_player.lock()->GetGameComponent<Player>()->IsClear()) { return; }
		if (wkp_player.lock()->GetGameComponent<Player>()->IsFreeze()) { return; }

		shp_scrollManager->ResetScroll();
		
		if (mode == GameMode::Chara)
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

			GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_panelMode, GameSettings::masterVolume * 3.0f);
		}
		else
		{
			mode = GameMode::Chara;
			shp_cameraController->ZoomIn(); 

			shp_particleEmitter->SetIsEdit(false);

			wkp_chara.lock()->transform->TranslateZ(1000);
			wkp_edit.lock()->transform->SetWorldPosition(modeUIPosition);

			shp_cameraController->ZoomIn();

			//GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_charaMode, GameSettings::masterVolume);
		}
	}
}

void ButiEngine::StageManager::CreateUI()
{
	if (GameSettings::isTitle) return;
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

	if (stageNum >= 6)
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

	if (StageSelect::GetStageNum() != StageSelect::GetMaxStage() && !isNext)
	{
		if (InputManager::OnTriggerRightKey())
		{
			if (!selectedNext)
			{
				GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);
			}
			selectedNext = true;
			shp_buttonNext->OnSelected();
			shp_buttonSelect->OnEndSelect();
			nextSceneName = "Stage" + std::to_string(StageSelect::GetStageNum() + 1);
		}
		else if (InputManager::OnTriggerLeftKey())
		{
			if (selectedNext)
			{
				GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_select, GameSettings::masterVolume);
			}
			selectedNext = false;
			shp_buttonNext->OnEndSelect();
			shp_buttonSelect->OnSelected();
			nextSceneName = "StageSelect";
		}
	}

	if (InputManager::OnTriggerDecisionKey() && !isNext)
	{
		isNext = true;
		GetManager().lock()->GetApplication().lock()->GetSoundManager()->PlaySE(se_enter, GameSettings::masterVolume);
		if (selectedNext)
		{
			wkp_buttonNext.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
		}
		else
		{
			wkp_buttonSelect.lock()->GetGameComponent<ShakeComponent>()->ShakeStart(20.0f);
		}
	}
}

void ButiEngine::StageManager::CommonTextObject()
{
	wkp_textWindow = GetManager().lock()->AddObjectFromCereal("TextWindow", ObjectFactory::Create<Transform>(Vector3(0, -330, -0.12f), Vector3::Zero, Vector3(1920, 640, 1)));
	GetManager().lock()->AddObjectFromCereal("Abutton", ObjectFactory::Create<Transform>(Vector3(790, -390, -0.16f), Vector3::Zero, Vector3(180, 180, 1)));
	GetManager().lock()->AddObjectFromCereal("SkipYbutton", ObjectFactory::Create<Transform>(Vector3(675, -220, -0.16f), Vector3::Zero, Vector3(80, 80, 1)));
	GetManager().lock()->AddObjectFromCereal("SkipText", ObjectFactory::Create<Transform>(Vector3(780, -220, -0.16f), Vector3::Zero, Vector3(160, 80, 1)));
}
