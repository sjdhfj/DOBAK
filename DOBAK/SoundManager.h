#pragma once
#include <string>
#include <map>
#include "Defines.h"
#include "fmod.hpp"

#define SOUND SoundManager::GetInst()
class SoundManager
{
private:
    SoundManager() = default;
    ~SoundManager() = default;
public:
    static SoundManager* GetInst()
    {
        if (m_pInst == nullptr)
            m_pInst = new SoundManager;
        return m_pInst;
    }
    static void DestroyInst()
    {
        SAFE_DELETE(m_pInst);
    }
    void Init();
    void Update();
    void Release();

    void Load(const std::string& key, const std::string& filePath);
    void Play(const std::string& key);
    void PlayBGM(const std::string& filePath);
    void StopBGM();
    void SetMute(const std::string& key, bool mute);
    bool GetMute(const std::string& key) const;
    void SetVolume(const std::string& key, float volume);
    float GetVolume(const std::string& key) const;
    void SetSFXMute(bool mute);
    bool GetSFXMute() const;
    void SetSFXVolume(float volume);
    float GetSFXVolume() const;
    void SetBGMMute(bool mute);
    bool GetBGMMute() const;
    void SetBGMVolume(float volume);
    float GetBGMVolume() const;
    void SetMasterMute(bool mute);
    bool GetMasterMute() const;
    void SetMasterVolume(float volume);
    float GetMasterVolume() const;
private:
    static SoundManager* m_pInst;
private:
    FMOD::System* m_system = nullptr;
    FMOD::Channel* m_bgmCh = nullptr;
    FMOD::Sound* m_bgm = nullptr;
    std::map<std::string, FMOD::Sound*> m_sounds;
    std::map<std::string, bool> m_muted;
    std::map<std::string, float> m_volumes;
private:
    bool m_sfxMuted = false;
    float m_sfxVolume = 1.0f;
    bool m_bgmMuted = false;
    float m_bgmVolume = 1.0f;
    bool m_masterMuted = false;
    float m_masterVolume = 1.0f;
};