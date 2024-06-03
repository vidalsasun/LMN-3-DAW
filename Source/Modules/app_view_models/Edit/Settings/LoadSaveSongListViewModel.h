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
    void updateDeviceManagerDeviceType();

  private:
    juce::AudioDeviceManager &deviceManager;
    juce::ValueTree state;

    void selectedIndexChanged(int newIndex) override;

  public:
    // Must appear below the other variables since it needs to be initialized
    // last
    juce::StringArray deviceTypes;
    ItemListState itemListState;
};

} // namespace app_view_models
