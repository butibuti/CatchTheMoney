#pragma once
#include"stdafx.h"
#include"../Resources/WaveDatas.h"
namespace ButiEngine {
	class SoundManager:public ISoundManager
	{
	public:
		SoundManager(std::weak_ptr<IApplication> arg_wkp_app);
		~SoundManager();
		void Initialize()override;
		void StopCheck()override;
		void Update()override;
		void ClearCheck()override;
		void PlaySE(const SoundTag tag, const  float volume)override;
		void PlayControllableSE(const SoundTag tag, const UINT index, const float volume, const bool isLoop)override;
		void SetControllableSEVolume(const UINT index, const float volume)override;
		void ExitControllableSELoop(const UINT index)override;
		void DestroyControllableSE(const UINT index)override;
		void StartontrollableSE(const UINT index)override;
		void StopControllableSE(const UINT index)override;
		void PlayBGM(const SoundTag tag, const float volume)override;
		void Release()override;
		void StopSE() override;
		void StopBGM() override;
		void RestartSE() override;
		void RestartBGM() override;
		void DestroySE() override;
		void DestroyBGM() override;
		void SetBGMVolume(const float volume) override;
		SoundTag GetNowPlayBGM()const override;
		float GetBGMVolume()const ;
		float GetControllableSEVolume(const int index)const;
	private:
		SoundTag nowBGM;
		IXAudio2SourceVoice* cmp_bgm;
		IXAudio2MasteringVoice* p_masterVoice;
		Microsoft::WRL::ComPtr< IXAudio2> cmp_pXAudio2;
		std::vector<IXAudio2SourceVoice*> vec_seVoices;
		std::vector<IXAudio2SourceVoice*> vec_controlleableSEVoices;
		std::shared_ptr<Timer> shp_clearCheckTimer; 
		std::weak_ptr<IApplication> wkp_app;
	};
}

