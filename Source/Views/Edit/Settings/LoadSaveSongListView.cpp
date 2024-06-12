#include "LoadSaveSongListView.h"
#include <ExtendedUIBehaviour.h>
#include <app_navigation/app_navigation.h>
#include <tracktion_engine/tracktion_engine.h>

LoadSaveSongListView::LoadSaveSongListView(
    tracktion::Edit &e, juce::AudioDeviceManager &dm,
    app_services::MidiCommandManager &mcm)
    : edit(e), deviceManager(dm), midiCommandManager(mcm),     
      editTabBarView(e, mcm), 
      viewModel(e, deviceManager, ConfigurationHelpers::getApplicationName()),
      titledList(viewModel.getItemNames(), "Track list",
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
        if (index == 0) { // Add
            const auto i = index;
            const auto loadFile = ConfigurationHelpers::getSavedTrackName();
            const auto loadFileName = loadFile.getFileName();

            auto userAppDataDirectory = juce::File::getSpecialLocation(
                juce::File::userApplicationDataDirectory);

            juce::File savedDirectory =
                userAppDataDirectory.getChildFile(JUCE_APPLICATION_NAME_STRING)
                    .getChildFile("saved");

            juce::File loadprojectDirectory =
                userAppDataDirectory.getChildFile(JUCE_APPLICATION_NAME_STRING)
                    .getChildFile("load_project");

            //mountname
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
            //
            auto saveFile = savedDirectory.getChildFile(loadFileName);           
            
            if (saveFile.existsAsFile()) {
                loadFile.copyFileTo(savedDirectory);
            } else {
                if (savedDirectory.createDirectory()) {
                    if (loadFile.moveFileTo(saveFile)) {
                        juce::Logger::writeToLog(
                            "Track copied to: " +
                            saveFile.getFullPathName());
                    } else {
                        juce::Logger::writeToLog(
                            "Error copy to: " +
                            saveFile.getFullPathName());
                    }
                } else {
                    juce::Logger::writeToLog(
                        "Error create folder: " +
                        loadprojectDirectory.getFullPathName());
                }
            }

            if (loadprojectDirectory.exists()) {
                auto files = loadprojectDirectory.findChildFiles(
                    juce::File::findFiles, false);
                for (auto &file : files) {
                    file.deleteFile();
                }
            }

            auto editFile = loadprojectDirectory.getChildFile(newEditFileName); 
            editFile.create();

            
            tracktion::Engine &engine = *tracktion::Engine::getEngines()[0];
            std::unique_ptr<tracktion::Edit> loadedEdit =
                tracktion::loadEditFromFile(engine, saveFile);

            /* bool success = loadedEdit->engine.getTemporaryFileManager()
                               .getTempDirectory()
                               .deleteRecursively();*/


            restartApplication();          


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
                loadTrackFromFile(projectFile);               
                //mainWindow = nullptr; // (deletes our window)                
            } else {
                juce::Logger::writeToLog("Project file does not exist: " +
                                         projectFile.getFullPathName());
            }
            restartApplication();
            
        }         
    }
}
void LoadSaveSongListView::restartApplication() {
    // Guarda el estado actual si es necesario
    // saveApplicationState();

    // Obtén el nombre del ejecutable de la aplicación
    juce::String appPath =
        juce::File::getSpecialLocation(juce::File::currentExecutableFile)
            .getFullPathName();

    // Inicia un nuevo proceso de la aplicación
    juce::ChildProcess process;
    if (process.start(appPath)) {
        // Cierra la aplicación actual
        juce::JUCEApplication::getInstance()->quit();
    } else {
        juce::Logger::writeToLog("Error al intentar reiniciar la aplicación.");
    }
}
void LoadSaveSongListView::loadTrackFromFile(const juce::File &projectFile) {
    if (projectFile.existsAsFile()) {
        juce::Logger::writeToLog("Loading project: " +
                                 projectFile.getFileName());

        tracktion::Engine &engine = *tracktion::Engine::getEngines()[0];
        std::unique_ptr<tracktion::Edit> loadedEdit =
            tracktion::loadEditFromFile(engine, projectFile);

        if (loadedEdit) {
            auto userAppDataDirectory = juce::File::getSpecialLocation(
                juce::File::userApplicationDataDirectory);

            juce::File loadprojectDirectory =
                userAppDataDirectory.getChildFile(JUCE_APPLICATION_NAME_STRING)
                    .getChildFile("load_project");

            if (loadprojectDirectory.exists() &&
                loadprojectDirectory.isDirectory()) {
                
                juce::Array<juce::File> files;
                loadprojectDirectory.findChildFiles(
                    files, juce::File::findFilesAndDirectories, true);

                for (const auto &file : files) {
                    if (file.deleteRecursively()) {
                        juce::Logger::writeToLog("Archivo eliminado: " +
                                                 file.getFullPathName());
                    } else {
                        juce::Logger::writeToLog(
                            "No se pudo eliminar el archivo: " +
                            file.getFullPathName());
                    }
                } 
            } else {
                juce::Logger::writeToLog(
                    "The directory does not exist or is not a directory.");
            }   
            
            juce::String fileName = projectFile.getFileName();
            juce::File destinationFile =
                loadprojectDirectory.getChildFile(fileName);

            if (!projectFile.copyFileTo(destinationFile)) {
                juce::Logger::writeToLog("Error copy on load");
            }
            juce::Logger::writeToLog("Project loaded successfully.");
        } else {
            juce::Logger::writeToLog("Failed to load project.");
        }
    } else {
        juce::Logger::writeToLog("Project file does not exist: " +
                                 projectFile.getFullPathName());
    }
}

void LoadSaveSongListView::selectedIndexChanged(int newIndex) {
    titledList.getListView().getListBox().selectRow(newIndex);
    sendLookAndFeelChange();
}
