/*
 *  Copyright © 2010-2013 Team XBMC
 *  http://xbmc.org
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <windows.h>
#include <io.h>
#include <vector>
#include "core/Vortex.h"

#include <kodi/addon-instance/Visualization.h>

extern char g_pluginPath[];

class CVisualizationVortex
  : public kodi::addon::CAddonBase,
    public kodi::addon::CInstanceVisualization
{
public:
  CVisualizationVortex();
  virtual ~CVisualizationVortex();

  virtual bool Start(int channels, int samplesPerSec, int bitsPerSample, std::string songName) override;
  virtual void Render() override;
  virtual void AudioData(const float* audioData, int audioDataLength, float *freqData, int freqDataLength) override;
  virtual bool UpdateTrack(const VisTrack &track) override;
  virtual bool UpdateAlbumart(std::string albumart) override;
  virtual bool GetPresets(std::vector<std::string>& presets) override;
  virtual bool PrevPreset() override;
  virtual bool NextPreset() override;
  virtual bool LoadPreset(int select) override;
  virtual bool LockPreset(bool lockUnlock) override;
  virtual bool RandomPreset() override;
  virtual int GetActivePreset() override;
  virtual bool IsLocked() override;
  virtual ADDON_STATUS SetSetting(const std::string& settingName, const kodi::CSettingValue& settingValue) override;

private:
  Vortex* m_Vortex;
};

CVisualizationVortex::CVisualizationVortex()
  : m_Vortex(nullptr)
{
  strcpy(g_pluginPath, Presets().c_str());

  m_Vortex = new Vortex;

  UserSettings& userSettings = m_Vortex->GetUserSettings();
  userSettings.RandomPresetsEnabled = kodi::GetSettingBoolean("RandomPresets");
  userSettings.TimeBetweenPresets = (float)(kodi::GetSettingInt("TimeBetweenPresets") * 5 + 5);
  userSettings.TimeBetweenPresetsRand = (float)(kodi::GetSettingInt("AdditionalRandomTime") * 5);
  userSettings.TransitionsEnabled = kodi::GetSettingBoolean("EnableTransitions");
  userSettings.StopFirstPreset = kodi::GetSettingBoolean("StopFirstPreset");
  userSettings.ShowFPS = kodi::GetSettingBoolean("ShowFPS");
  userSettings.ShowDebugConsole = kodi::GetSettingBoolean("ShowDebugConsole");
  userSettings.ShowAudioAnalysis = kodi::GetSettingBoolean("ShowAudioAnalysis");
  userSettings.PresetLocked = kodi::GetSettingBoolean("LockPreset");

  m_Vortex->Init(reinterpret_cast<ID3D11DeviceContext*>(Device()), X(), Y(), Width(), Height(), PixelRatio());
}

CVisualizationVortex::~CVisualizationVortex()
{
  if (m_Vortex)
  {
    m_Vortex->Shutdown();
    delete m_Vortex;
    m_Vortex = nullptr;
  }
}

bool CVisualizationVortex::Start(int channels, int samplesPerSec, int bitsPerSample, std::string songName)
{
  m_Vortex->Start( channels, samplesPerSec, bitsPerSample, songName );
  return true
}

void CVisualizationVortex::AudioData(const float* audioData, int audioDataLength, float *freqData, int freqDataLength)
{
  m_Vortex->AudioData(audioData, audioDataLength, freqData, freqDataLength);
}

void CVisualizationVortex::Render()
{
  m_Vortex->Render();
}

bool CVisualizationVortex::UpdateTrack(const VisTrack &track)
{
  m_Vortex->UpdateTrack(&track);
  return true;
}

bool CVisualizationVortex::UpdateAlbumart(std::string albumart)
{
  m_Vortex->UpdateAlbumArt(albumart.c_str());
  return true;
}

bool CVisualizationVortex::PrevPreset()
{
  m_Vortex->LoadPreviousPreset();
  return true;
}

bool CVisualizationVortex::NextPreset()
{
  m_Vortex->LoadNextPreset();
  return true;
}

bool CVisualizationVortex::LoadPreset(int select)
{
  m_Vortex->LoadPreset(select);
  return true;
}

bool CVisualizationVortex::LockPreset(bool lockUnlock)
{
  m_Vortex->GetUserSettings().PresetLocked = lockUnlock;
  return true;
}

bool CVisualizationVortex::RandomPreset()
{
  m_Vortex->LoadRandomPreset();
  return true;
}

bool CVisualizationVortex::GetPresets(std::vector<std::string>& presets)
{
  return m_Vortex->GetPresets(presets);
}

int CVisualizationVortex::GetActivePreset()
{
  if (m_Vortex)
    return m_Vortex->GetCurrentPresetIndex();
  return 0;
}

bool CVisualizationVortex::IsLocked()
{
  if (m_Vortex)
    return m_Vortex->GetUserSettings().PresetLocked;
  else
    return false;
}

ADDON_STATUS CVisualizationVortex::SetSetting(const std::string& settingName, const kodi::CSettingValue& settingValue)
{
  if (settingName.empty() || settingValue.empty())
    return ADDON_STATUS_UNKNOWN;

  UserSettings& userSettings = m_Vortex->GetUserSettings();

  if (settingName == "RandomPresets")
  {
    userSettings.RandomPresetsEnabled = settingValue.GetBoolean();
    return ADDON_STATUS_OK;
  }
  else if (settingName == "TimeBetweenPresets")
  {
    userSettings.TimeBetweenPresets = (float)(settingValue.GetInt() * 5 + 5);
    return ADDON_STATUS_OK;
  }
  else if (settingName == "AdditionalRandomTime")
  {
    userSettings.TimeBetweenPresetsRand = (float)(settingValue.GetInt() * 5 );
    return ADDON_STATUS_OK;
  }
  else if (settingName == "EnableTransitions")
  {
    userSettings.TransitionsEnabled = settingValue.GetBoolean();
    return ADDON_STATUS_OK;
  }
  else if (settingName == "StopFirstPreset")
  {
    userSettings.StopFirstPreset = settingValue.GetBoolean();
    return ADDON_STATUS_OK;
  }
  else if (settingName == "ShowFPS")
  {
    userSettings.ShowFPS = settingValue.GetBoolean();
    return ADDON_STATUS_OK;
  }
  else if (settingName == "ShowDebugConsole")
  {
    userSettings.ShowDebugConsole = settingValue.GetBoolean();
    return ADDON_STATUS_OK;
  }
  else if (settingName == "ShowAudioAnalysis")
  {
    userSettings.ShowAudioAnalysis = settingValue.GetBoolean();
    return ADDON_STATUS_OK;
  }
  else if (settingName == "LockPreset")
  {
    userSettings.PresetLocked = settingValue.GetBoolean();
    return ADDON_STATUS_OK;
  }

  return ADDON_STATUS_UNKNOWN;
}

ADDONCREATOR(CVisualizationVortex)
