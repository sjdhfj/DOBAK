#include "SoundManager.h"
#include "fmod.hpp"
#pragma comment(lib, "fmod_vc.lib") 
SoundManager* SoundManager::m_pInst = nullptr;

void SoundManager::Init()
{
    FMOD::System_Create(&m_system);
    m_system->init(32, FMOD_INIT_NORMAL, nullptr);
}

void SoundManager::Update()
{
    if (m_system)
        m_system->update();
}

void SoundManager::Release()
{
    for (auto& pair : m_sounds)
        pair.second->release();
    m_sounds.clear();

    StopBGM();

    if (m_system != nullptr)
    {
        m_system->close();
        m_system->release();
        m_system = nullptr;
    }
}

void SoundManager::Load(const std::string& key, const std::string& filePath)
{
    if (m_system == nullptr)
        return;

    FMOD::Sound* sound = nullptr;
    m_system->createSound(filePath.c_str(), FMOD_DEFAULT, nullptr, &sound);
    if (sound != nullptr)
        m_sounds[key] = sound;
}

void SoundManager::Play(const std::string& key)
{
    if (m_system == nullptr)
        return;
    if (GetMute(key))
        return;
    auto it = m_sounds.find(key);
    if (it == m_sounds.end())
        return;
    FMOD::Channel* channel = nullptr;
    m_system->playSound(it->second, nullptr, false, &channel);
    if (channel)
        channel->setVolume(GetVolume(key) * GetSFXVolume() * GetMasterVolume());
}

void SoundManager::PlayBGM(const std::string& filePath)
{
    if (m_system == nullptr)
        return;
    StopBGM();
    m_system->createStream(filePath.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, nullptr, &m_bgm);
    m_system->playSound(m_bgm, nullptr, false, &m_bgmCh);

    if (m_bgmCh)
    {
        m_bgmCh->setVolume(m_bgmVolume * m_masterVolume);
        m_bgmCh->setMute(m_bgmMuted || m_masterMuted);
    }
}
void SoundManager::StopBGM()
{
    if (m_bgmCh != nullptr)
    {
        m_bgmCh->stop();
        m_bgmCh = nullptr;
    }

    if (m_bgm != nullptr)
    {
        m_bgm->release();
        m_bgm = nullptr;
    }
}

void SoundManager::SetMute(const std::string& key, bool mute)
{
    m_muted[key] = mute;
}

bool SoundManager::GetMute(const std::string& key) const
{
    auto it = m_muted.find(key);
    return it != m_muted.end() && it->second;
}

void SoundManager::SetVolume(const std::string& key, float volume)
{
    m_volumes[key] = volume;
}

float SoundManager::GetVolume(const std::string& key) const
{
    auto it = m_volumes.find(key);
    return it != m_volumes.end() ? it->second : 1.0f;
}

void SoundManager::SetSFXMute(bool mute)
{ 
    m_sfxMuted = mute;
}
bool SoundManager::GetSFXMute() const 
{ 
    return m_sfxMuted; 
}
void SoundManager::SetSFXVolume(float volume) 
{
    m_sfxVolume = volume; 
}
float SoundManager::GetSFXVolume() const 
{ 
    return m_sfxVolume; 
}

void SoundManager::SetBGMMute(bool mute)
{
    m_bgmMuted = mute;
    if (m_bgmCh)
        m_bgmCh->setMute(mute || m_masterMuted);
}
bool SoundManager::GetBGMMute() const
{
    return m_bgmMuted;
}

void SoundManager::SetBGMVolume(float volume)
{
    m_bgmVolume = volume;
    if (m_bgmCh)
        m_bgmCh->setVolume(volume);
}
float SoundManager::GetBGMVolume() const
{
    return m_bgmVolume;
}

void SoundManager::SetMasterMute(bool mute)
{
    m_masterMuted = mute;
    if (m_bgmCh)
        m_bgmCh->setMute(mute || m_bgmMuted);
}
bool SoundManager::GetMasterMute() const 
{
    return m_masterMuted; 
}

void SoundManager::SetMasterVolume(float volume)
{
    m_masterVolume = volume;
    if (m_bgmCh)
        m_bgmCh->setVolume(m_bgmVolume * volume);
}
float SoundManager::GetMasterVolume() const 
{ 
    return m_masterVolume; 
}
