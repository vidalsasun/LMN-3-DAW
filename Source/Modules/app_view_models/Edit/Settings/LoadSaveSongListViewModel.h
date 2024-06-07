#pragma once

namespace app_view_models {
namespace IDs {
const juce::Identifier
    LOAD_SAVE_SONG_VIEW_STATE("LOAD_SAVE_SONG_VIEW_STATE");
}

class LoadSaveSongListViewModel : private ItemListState::Listener {
  public:
    LoadSaveSongListViewModel(tracktion::Edit &e, juce::AudioDeviceManager &dm);
    ~LoadSaveSongListViewModel() override;

    juce::StringArray getItemNames();
    juce::String getSelectedItem();
    //void updateDeviceManagerDeviceType();
    //void loadSongList();

  private:
    juce::AudioDeviceManager &deviceManager;
    juce::ValueTree state;
    juce::String applicationName = JUCE_APPLICATION_NAME_STRING;

    void selectedIndexChanged(int newIndex) override;
    void loadSongList(const juce::File &directory);
    void updateLoadSaveSong(const juce::File &directory);
  public:
    // Must appear below the other variables since it needs to be initialized
    // last
    //juce::StringArray deviceTypes;
    juce::StringArray songNames;
    ItemListState itemListState;
};

} // namespace app_view_models
