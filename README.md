# Table of contents
* [General info](#general-info)
* [Data structure](#data-structure)
* [Built With](#built-with)
* [License](#license)

# About The Project

## General info

<p class="text-justify">
This project is a simple VST3 delay effect that works separately on the left and right channel. You can use different delay time on left and right channel, which can be also synchronized to tempo of the song by setting rhythmic division.   There is also a bunch of additional effect like tap echo or flanger.
</p>

![Product Name Screen Shot](https://github.com/jarekkopaczewski/TwinEcho/blob/e82460d70e3dbb9e6220f74dcf8eb0781a36adcc/TwinDelay.png)

## Data structure

To achieve a delayed effect I used the circular buffer structure which size depends on from the parameters set by the user.

![Product Name Screen Shot](https://github.com/jarekkopaczewski/TwinEcho/blob/6ff12e2a9b27c65e19132080ee0411574d6aaddc/CircularBuffer.jpg)

## Built With

* [C++](https://isocpp.org/)
* [Juce](https://juce.com/)

## Save parameters

```cpp
void DelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(treeState.state.getType()))
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
}
```

## License

Distributed under the MIT License.

<p align="right">(<a href="#top">back to top</a>)</p>
