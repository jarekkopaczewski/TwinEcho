# Table of contents
* [General info](#general-info)
* [Data structure](#data-structure)
* [Built With](#build-with)
* [Roadmap](#roadmap)
* [License](#license)

# About The Project

## General info

<p class="text-justify">
This project is a simple VST3 delay effect that works separately on the left and right channel. You can use different delay time on left and right channel, which can be also synchronized to tempo of the song by setting rhythmic division. 
There is also a bunch of additional effect like flanger or tap echo.
</p>

![Product Name Screen Shot](https://github.com/jarekkopaczewski/TwinEcho/blob/e82460d70e3dbb9e6220f74dcf8eb0781a36adcc/TwinDelay.png)

## Data structure

To achieve a delayed effect I used the circular buffer structure which size depends on from the parameters set by the user.

![Product Name Screen Shot](https://github.com/jarekkopaczewski/TwinEcho/blob/6ff12e2a9b27c65e19132080ee0411574d6aaddc/CircularBuffer.jpg)

## Built With

* [C++](https://isocpp.org/)
* [Juce](https://juce.com/)

## Roadmap

- [ ] Feature 1
- [ ] Feature 2
- [ ] Feature 3
- [ ] Nested Feature

## License

Distributed under the MIT License.

<p align="right">(<a href="#top">back to top</a>)</p>
