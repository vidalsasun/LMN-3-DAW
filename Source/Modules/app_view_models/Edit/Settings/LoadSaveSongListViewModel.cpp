namespace app_view_models {
LoadSaveSongListViewModel::LoadSaveSongListViewModel(
    tracktion::Edit &e,
                                                 juce::AudioDeviceManager &dm)
    : deviceManager(dm),
      state(e.state.getOrCreateChildWithName(IDs::SETTINGS_VIEW_STATE, nullptr)
                .getOrCreateChildWithName(IDs::DEVICE_TYPE_LIST_VIEW_STATE,
                                          nullptr)),
      itemListState(state, deviceTypes.size()) {
    for (auto deviceType : deviceManager.getAvailableDeviceTypes()) {
        deviceTypes.add(deviceType->getTypeName());
    }
    itemListState.listSize = deviceTypes.size();

    int currentDeviceIndex =
        deviceTypes.indexOf(deviceManager.getCurrentAudioDeviceType());
    if (currentDeviceIndex == -1) {
        currentDeviceIndex = 0;
    }
    itemListState.setSelectedItemIndex(currentDeviceIndex);
    itemListState.addListener(this);
}

LoadSaveSongListViewModel::~LoadSaveSongListViewModel() {
    itemListState.removeListener(this);
}

juce::StringArray LoadSaveSongListViewModel::getItemNames() {
    return deviceTypes;
}

juce::String LoadSaveSongListViewModel::getSelectedItem() {
    return deviceTypes[itemListState.getSelectedItemIndex()];
}

void LoadSaveSongListViewModel::updateDeviceManagerDeviceType() {
    deviceManager.setCurrentAudioDeviceType(getSelectedItem(), true);
}

void LoadSaveSongListViewModel::selectedIndexChanged(int newIndex) {
    updateDeviceManagerDeviceType();
}
} // namespace app_view_models