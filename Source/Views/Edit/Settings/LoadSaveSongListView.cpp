#include "LoadSaveSongListView.h"
#include <app_navigation/app_navigation.h>
#include <tracktion_engine/tracktion_engine.h>
#include <ExtendedUIBehaviour.h>

LoadSaveSongListView::LoadSaveSongListView(
    tracktion::Edit &e, juce::AudioDeviceManager &dm,
    app_services::MidiCommandManager &mcm)
    : edit(e), 
      deviceManager(dm), midiCommandManager(mcm),
      viewModel(e, deviceManager, ConfigurationHelpers::getApplicationName()),
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
        const auto index = viewModel.itemListState.getSelectedItemIndex();
        if (index == 0) { //Save
            const auto i = index;
            const auto track_name = ConfigurationHelpers::getSavedTrackName();

            auto userAppDataDirectory = juce::File::getSpecialLocation(
                juce::File::userApplicationDataDirectory);
            juce::File savedDirectory =
                userAppDataDirectory.getChildFile(JUCE_APPLICATION_NAME_STRING)
                    .getChildFile("saved");

            auto currentTime = juce::Time::getCurrentTime();
            auto day =
                juce::String(currentTime.getDayOfMonth()).paddedLeft('0', 2);
            auto month = juce::String(currentTime.getMonth() + 1)
                             .paddedLeft('0', 2); // Meses son 0-based
            auto year = juce::String(currentTime.getYear());
            auto hours =
                juce::String(currentTime.getHours()).paddedLeft('0', 2);
            auto minutes =
                juce::String(currentTime.getMinutes()).paddedLeft('0', 2);
            auto seconds =
                juce::String(currentTime.getSeconds()).paddedLeft('0', 2);

            juce::String newEditFileName = "edit_" + day + month + year +
                                           hours + minutes + seconds + ".xml";
            auto editFile = savedDirectory.getChildFile(newEditFileName);

            editFile.create();
            /* tracktion::EditFileOperations fileOperations(edit);
            fileOperations.saveAs(editFile, true);*/

            int pos = newEditFileName.lastIndexOf(".xml");
            if (pos != -1 && pos == newEditFileName.length() - 4) {
                // Quitar la extensión ".xml"
                newEditFileName = newEditFileName.substring(0, pos);
            }

            juce::Logger::writeToLog("Complete! (" + newEditFileName + ")");
            messageBox.setMessage("Complete! (" + newEditFileName + ")");
            resized();
            messageBox.setVisible(true);
            //startTimer(1000);
        } else { // Load
            juce::String projectName = viewModel.getItemNames()[index];
            juce::File projectDirectory =
                juce::File::getSpecialLocation(
                    juce::File::userApplicationDataDirectory)
                    .getChildFile(JUCE_APPLICATION_NAME_STRING)
                    .getChildFile("saved");

            juce::File projectFile =
                projectDirectory.getChildFile(projectName + ".xml");

            if (projectFile.existsAsFile()) {
                juce::Logger::writeToLog("Loading project: " + projectName);

                // Almacenar el engine en una variable
                tracktion::engine::Engine &engine =
                    *tracktion::engine::Engine::getEngines()[0];

                // Cargar el proyecto utilizando el engine
                std::unique_ptr<tracktion::Edit> loadedEdit =
                    tracktion::loadEditFromFile(engine, projectFile);

                if (loadedEdit) {
                    juce::Logger::writeToLog("Project loaded successfully.");

                    // Asegurarse de que el contexto del transporte está
                    // asignado
                    loadedEdit->getTransport().ensureContextAllocated();
                    loadedEdit->clickTrackEnabled.setValue(true, nullptr);
                    loadedEdit->setCountInMode(
                        tracktion::Edit::CountIn::oneBar);

                    // Configurar el MidiCommandManager
                    auto midiCmdManager =
                        std::make_unique<app_services::MidiCommandManager>(
                            engine);

                    if (auto uiBehavior = dynamic_cast<ExtendedUIBehaviour *>(
                            &engine.getUIBehaviour())) {
                        uiBehavior->setEdit(loadedEdit.get());
                        uiBehavior->setMidiCommandManager(midiCmdManager.get());
                    }

                    // Inicializar el dispositivo de audio
                    auto &deviceManager =
                        engine.getDeviceManager().deviceManager;
                    deviceManager.getCurrentDeviceTypeObject()
                        ->scanForDevices();
                    auto result =
                        deviceManager.initialiseWithDefaultDevices(0, 2);
                    if (result != "") {
                        juce::Logger::writeToLog(
                            "Attempt to initialise default devices failed!");
                    }

                    // Configurar los plugins en las pistas principales
                    for (auto track :
                         tracktion::getTopLevelTracks(*loadedEdit)) {
                        if (track->isMasterTrack()) {
                            if (track->pluginList
                                    .getPluginsOfType<
                                        tracktion::VolumeAndPanPlugin>()
                                    .getLast() == nullptr) {
                                track->pluginList.addDefaultTrackPlugins(false);
                            }
                        }
                    }

                    // Actualizar la UI para mostrar el nuevo Edit
                    /* if (auto parentComponent = findParentComponentOfClass<
                            YourMainComponentClass>()) {
                        parentComponent->setEdit(loadedEdit.get());
                        parentComponent->repaint();
                        parentComponent->resized();
                    }*/

                    // Llamar a initSamples con la referencia al engine
                    ConfigurationHelpers::setSavedTrackName(projectFile);
                    ConfigurationHelpers::initSamples(engine);
                } else {
                    juce::Logger::writeToLog("Failed to load project.");
                }
            } else {
                juce::Logger::writeToLog("Project file does not exist: " +
                                         projectFile.getFullPathName());
            }
        }

        if (midiCommandManager.getFocusedComponent() == this) {
            if (auto stackNavigationController = findParentComponentOfClass<
                    app_navigation::StackNavigationController>()) {
                stackNavigationController->popToRoot();
                midiCommandManager.setFocusedComponent(
                    stackNavigationController->getTopComponent());
            }
        }

        // Obtener y mostrar el valor de text
        /* if (midiCommandManager.getFocusedComponent() == this) {
            if (auto stackNavigationController = findParentComponentOfClass<
                    app_navigation::StackNavigationController>()) {
                stackNavigationController->popToRoot();
                midiCommandManager.setFocusedComponent(
                    stackNavigationController->getTopComponent());
            }
        }*/
    }
}
void LoadSaveSongListView::selectedIndexChanged(int newIndex) {
    titledList.getListView().getListBox().selectRow(newIndex);
    sendLookAndFeelChange();
}