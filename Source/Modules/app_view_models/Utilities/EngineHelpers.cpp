#include "EngineHelpers.h"

namespace app_view_models {

bool EngineHelpers::isTrackArmed(tracktion::AudioTrack &t, int position) {
    auto &edit = t.edit;
    for (auto instance : edit.getAllInputDevices())
        if (instance->isOnTargetTrack(t, position))
            return instance->isRecordingEnabled(t);

    return false;
}
void generateSyncPulse(juce::AudioBuffer<float> &buffer, double sampleRate,
                       double bpm) {
    const float pulseFrequency = bpm / 60.0; // Pulsos por segundo
    const int numSamples = buffer.getNumSamples();
    auto *channelData = buffer.getWritePointer(0);

    for (int i = 0; i < numSamples; ++i) {
        double phase = std::fmod(i * pulseFrequency / sampleRate, 1.0);
        channelData[i] = (phase < 0.01) ? 1.0f : 0.0f; // Pulso corto
    }
}
juce::Array<tracktion::Track *>
EngineHelpers::getAudioAndMasterTracks(tracktion::Edit &e) {
    juce::Array<tracktion::Track *> tracks;
    tracks.ensureStorageAllocated(32);
    for (auto track : tracktion::getTopLevelTracks(e)) {
        if (track->isAudioTrack() || track->isMasterTrack()) {
            tracks.add(track);
        }
    }

    return tracks;
}

tracktion::VolumeAndPanPlugin *
EngineHelpers::getVolumeAndPanPluginForTrack(tracktion::Track *track) {
    if (track->isMasterTrack()) {
        return track->edit.getMasterVolumePlugin();
    }

    return track->pluginList.getPluginsOfType<tracktion::VolumeAndPanPlugin>()
        .getLast();
}

template <typename ClipType>
typename ClipType::Ptr loopAroundClip(ClipType &clip) {
    auto &transport = clip.edit.getTransport();
    transport.setLoopRange(clip.getEditTimeRange());
    transport.looping = true;
    return clip;
}
} // namespace app_view_models