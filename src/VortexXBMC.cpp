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
#include <kodi/addon-instance/Visualization.h>
#include <io.h>
#include <vector>
#include "Vortex.h"

class CVisualizationVortex
  : public kodi::addon::CAddonBase
  , public kodi::addon::CInstanceVisualization
{
public:
  virtual ~CVisualizationVortex();

  virtual ADDON_STATUS Create() override;
  virtual bool Start(int channels, int samplesPerSec, int bitsPerSample, std::string songName) override;
  virtual void Stop() override;
  virtual void Render() override;
  virtual bool GetPresets(std::vector<std::string>& presets) override;
  virtual int GetActivePreset() override;
  virtual bool IsLocked() override;
  virtual bool PrevPreset() override;
  virtual bool NextPreset() override;
  virtual bool LoadPreset(int select) override;
  virtual bool RandomPreset() override;
  virtual bool LockPreset(bool lockUnlock) override;
  virtual void AudioData(const float* audioData, int audioDataLength, float *freqData, int freqDataLength) override;
  virtual ADDON_STATUS SetSetting(const std::string& settingName, const kodi::CSettingValue& settingValue) override;
  virtual bool UpdateTrack(const VisTrack &track) override;
  virtual bool UpdateAlbumart(std::string albumart) override;
private:
  Vortex* m_Vortex{ nullptr };
  std::string g_pluginPath;
};


// settings vector
//StructSetting** g_structSettings;

CVisualizationVortex::~CVisualizationVortex()
{
  if ( m_Vortex )
  {
    m_Vortex->Shutdown();
    delete m_Vortex;
    m_Vortex = NULL;
  }
}

ADDON_STATUS CVisualizationVortex::Create()
{
  g_pluginPath = Presets();

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

  return ADDON_STATUS_OK;
}

bool CVisualizationVortex::Start(int channels, int samplesPerSec, int bitsPerSample, std::string songName)
{
  m_Vortex->Start( channels, samplesPerSec, bitsPerSample, songName.c_str() );
  return true;
}

void CVisualizationVortex::Stop()
{
  if ( m_Vortex )
  {
    m_Vortex->Shutdown();
    delete m_Vortex;
    m_Vortex = NULL;
  }
}

void CVisualizationVortex::AudioData(const float* pAudioData, int iAudioDataLength, float *pFreqData, int iFreqDataLength)
{
  m_Vortex->AudioData( pAudioData, iAudioDataLength, pFreqData, iFreqDataLength );
}

bool CVisualizationVortex::UpdateTrack(const VisTrack & track)
{
  m_Vortex->UpdateTrack(track);
  return true;
}

bool CVisualizationVortex::UpdateAlbumart(std::string albumart)
{
  m_Vortex->UpdateAlbumArt((char*)albumart.c_str());
  return true;
}

void CVisualizationVortex::Render()
{
  m_Vortex->Render();
}

bool CVisualizationVortex::GetPresets(std::vector<std::string>& presets)
{
  if( m_Vortex == NULL )
  {
    return 0;
  }
  return m_Vortex->GetPresets( presets ) > 0;
}

//-- GetPreset ----------------------------------------------------------------
// Return the index of the current playing preset
//-----------------------------------------------------------------------------
int CVisualizationVortex::GetActivePreset()
{
  if (m_Vortex)
    return m_Vortex->GetCurrentPresetIndex();
  return 0;
}

//-- IsLocked -----------------------------------------------------------------
// Returns true if this add-on use settings
//-----------------------------------------------------------------------------
bool CVisualizationVortex::IsLocked()
{
  if (m_Vortex)
    return m_Vortex->GetUserSettings().PresetLocked;
  else
    return false;
}

ADDON_STATUS CVisualizationVortex::SetSetting(const std::string & settingName, const kodi::CSettingValue & settingValue)
{
  if ( settingName.empty() || settingValue.empty() || m_Vortex == NULL )
    return ADDON_STATUS_UNKNOWN;

  UserSettings& userSettings = m_Vortex->GetUserSettings();

  if (settingName == "RandomPresets")
  {
    userSettings.RandomPresetsEnabled = settingValue.GetBoolean();
  }
  else if (settingName == "TimeBetweenPresets")
  {
    userSettings.TimeBetweenPresets = (float)(settingValue.GetInt() * 5 + 5);
  }
  else if (settingName == "AdditionalRandomTime")
  {
    userSettings.TimeBetweenPresetsRand = (float)(settingValue.GetInt() * 5);
  }
  else if (settingName == "EnableTransitions")
  {
    userSettings.TransitionsEnabled = settingValue.GetBoolean();
  }
  else if (settingName == "StopFirstPreset")
  {
    userSettings.StopFirstPreset = settingValue.GetBoolean();
  }
  else if (settingName == "ShowFPS")
  {
    userSettings.ShowFPS = settingValue.GetBoolean();
  }
  else if (settingName == "ShowDebugConsole")
  {
    userSettings.ShowDebugConsole = settingValue.GetBoolean();
  }
  else if (settingName == "ShowAudioAnalysis")
  {
    userSettings.ShowAudioAnalysis = settingValue.GetBoolean();
  }
  else if (settingName == "LockPreset")
  {
    userSettings.PresetLocked = settingValue.GetBoolean();
  }
  else
    return ADDON_STATUS_UNKNOWN;

  return ADDON_STATUS_OK;
}

//-- GetSubModules ------------------------------------------------------------
// Return any sub modules supported by this vis
//-----------------------------------------------------------------------------
extern "C"   unsigned int GetSubModules(char ***presets)
{
  return 0; // this vis supports 0 sub modules
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
  m_Vortex->LoadPreset( select );
  return true;
}

bool CVisualizationVortex::RandomPreset()
{
  m_Vortex->LoadRandomPreset();
  return true;
}

bool CVisualizationVortex::LockPreset(bool lockUnlock)
{
  m_Vortex->GetUserSettings().PresetLocked = lockUnlock;
  return true;
}

ADDONCREATOR(CVisualizationVortex) // Don't touch this!