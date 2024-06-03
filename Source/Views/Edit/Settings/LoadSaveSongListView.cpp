#include "LoadSaveSongListView.h"
#include <app_navigation/app_navigation.h>

LoadSaveSongListView::LoadSaveSongListView(
    tracktion::Edit &e, juce::AudioDeviceManager &dm,
    app_services::MidiCommandManager &mcm)
    : deviceManager(dm), midiCommandManager(mcm), viewModel(e, deviceManager),
      titledList(viewModel.getItemNames(), "Song list",
                 ListTitle::IconType::FONT_AWESOME,
                 juce::String::charToString(0xf7d9)) {
    viewModel.itemListState.addListener(this);
    midiCommandManager.addListener(this);

    addAndMakeVisible(titledList);
}

LoadSaveSongListView::~LoadSaveSongListView() {
    midiCommandManager.removeListener(this);
    viewModel.itemListState.removeListener(this);
}

void LoadSaveSongListView::paint(juce::Graphics &g) {
    g.fillAll(
        getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void LoadSaveSongListView::resized() {
    titledList.setBounds(getLocalBounds());
    titledList.getListView().getListBox().scrollToEnsureRowIsOnscreen(
        viewModel.itemListState.getSelectedItemIndex());
}

void LoadSaveSongListView::encoder1Increased() {
    if (isShowing()) {
        if (midiCommandManager.getFocusedComponent() == this) {
            viewModel.itemListState.setSelectedItemIndex(
                viewModel.itemListState.getSelectedItemIndex() + 1);
        }
    }
}

void LoadSaveSongListView::encoder1Decreased() {
    if (isShowing()) {
        if (midiCommandManager.getFocusedComponent() == this) {
            viewModel.itemListState.setSelectedItemIndex(
                viewModel.itemListState.getSelectedItemIndex() - 1);
        }
    }
}

void LoadSaveSongListView::encoder1ButtonReleased() {
    if (isShowing()) {
        if (midiCommandManager.getFocusedComponent() == this) {
            if (auto stackNavigationController = findParentComponentOfClass<
                    app_navigation::StackNavigationController>()) {
                stackNavigationController->popToRoot();
                midiCommandManager.setFocusedComponent(
                    stackNavigationController->getTopComponent());
            }
        }
    }
}

void saveEditState(const tracktion::engine::Edit &edit,
                   const juce::File &file) {
    // Serializar el estado del Edit a XML
    auto xml = edit.state.createXml();

    // Guardar el XML en un archivo
    if (xml) {
        if (!xml->writeTo(file)) {
            // Manejar el error si la escritura al archivo falla
            DBG("Failed to write Edit state to file.");
        }
    } else {
        // Manejar el error si la serialización falla
        DBG("Failed to create XML from Edit state.");
    }
}
void loadEditState(tracktion::engine::Edit &edit, const juce::File &file) {
    // Leer el XML desde el archivo
    auto xml = juce::XmlDocument::parse(file);

    if (xml) {
        // Deserializar el XML para restaurar el estado del Edit
        edit.state = juce::ValueTree::fromXml(*xml);
    } else {
        // Manejar el error si la lectura del archivo falla
        DBG("Failed to read Edit state from file.");
    }
}
void LoadSaveSongListView::selectedIndexChanged(int newIndex) {
    titledList.getListView().getListBox().selectRow(newIndex);
    sendLookAndFeelChange();
}