#include "LFODestinationParameter.h"
#include "Identifiers.h"


const juce::StringArray LFODestinationParameter::destinations =  {"ENG", "ENV", "FX"};

LFODestinationParameter::LFODestinationParameter(juce::ValueTree v)
    : state(v)
{
    jassert(v.hasType(IDs::PARAMETER));
    name.referTo(state, IDs::name, nullptr, "Destination");
    value.referTo(state, IDs::value, nullptr, "SND");

}

void LFODestinationParameter::increment()
{

    if (value.get() == destinations[0]) {

        value.setValue(destinations[1], nullptr);

    } else if (value.get() == destinations[1]) {

        value.setValue(destinations[2], nullptr);

    } else if (value.get() == destinations[2]) {

        value.setValue(destinations[1], nullptr);

    }


}

void LFODestinationParameter::decrement()
{

    if (value.get() == destinations[0]) {

        value.setValue(destinations[2], nullptr);

    } else if (value.get() == destinations[1]) {

        value.setValue(destinations[0], nullptr);

    } else if (value.get() == destinations[2]) {

        value.setValue(destinations[1], nullptr);

    }

}