#include "LoadSaveSongListViewModel.h"

namespace app_view_models {

LoadSaveSongListViewModel::LoadSaveSongListViewModel(
    tracktion::Edit &e, juce::AudioDeviceManager &dm)
    : deviceManager(dm), state(e.state.getOrCreateChildWithName(
                             IDs::LOAD_SAVE_SONG_VIEW_STATE, nullptr)),
      itemListState(state, songNames.size()) {
    // Inicializar la lista de canciones
    juce::File songDirectory(juce::File::getCurrentWorkingDirectory().getChildFile("saved"));
    loadSongList(songDirectory);
    itemListState.addListener(this);
}

LoadSaveSongListViewModel::~LoadSaveSongListViewModel() {
    itemListState.removeListener(this);
}

juce::StringArray LoadSaveSongListViewModel::getItemNames() {
    return songNames;
}

juce::String LoadSaveSongListViewModel::getSelectedItem() {
    return songNames[itemListState.getSelectedItemIndex()];
}

void LoadSaveSongListViewModel::selectedIndexChanged(int newIndex) {
    // Aquí puedes añadir la lógica que necesites cuando cambie el índice
    // seleccionado
}

void LoadSaveSongListViewModel::loadSongList(const juce::File &directory) {
    songNames.clear();
    juce::Array<juce::File> songFiles =
        directory.findChildFiles(juce::File::findFiles, false, "*.xml");

    for (const auto &file : songFiles) {
        songNames.add(file.getFileNameWithoutExtension());
    }

    itemListState.listSize = songNames.size();
    itemListState.setSelectedItemIndex(
        0); // Opcional: Resetear al primer ítem o cualquier otra lógica que
            // necesites
    itemListState.addListener(this);
}

} // namespace app_view_models
