## Encode video frame by frame in C++ using FFmpeg

<img src="screenshot.png" width="200"/>

Results of scientific or engineering simulations are often needed to be recorded on-the-fly. That is, the volume of data is often inapplicable to saving all individual frames on disk and postprocess them into video afterwards. This library leverages FFmpeg to embed video encoding directly into frame-producing application:

```cpp
#include "movie.h"

#include <cstdio>
#include <vector>

using namespace std;

...

const unsigned int width = 1024;
const unsigned int height = 768;
const unsigned int nframes = 128;

MovieWriter movie("random_pixels", width, height);

vector<uint8_t> pixels(4 * width * height);
for (unsigned int iframe = 0; iframe < nframes; iframe++)
{
        for (unsigned int j = 0; j < height; j++)
                for (unsigned int i = 0; i < width; i++)
                {
                        pixels[4 * width * j + 4 * i + 0] = rand() % 256;
                        pixels[4 * width * j + 4 * i + 1] = rand() % 256;
                        pixels[4 * width * j + 4 * i + 2] = rand() % 256;
                        pixels[4 * width * j + 4 * i + 3] = rand() % 256;
                }

        movie.addFrame(&pixels[0]);
}
```

Additionally, the library offers an interface to encode SVG images.

## Prerequisites

```
sudo apt install gcc-c++ #Fedora
sudo apt install g++ #Ubuntu
sudo apt install cmake librsvg2-dev libx264-dev libgtk-3-dev
sudo apt-get install libavcodec-dev libavfilter-dev libavformat-dev libavutil-dev
```

## Building

```
git clone --recurse-submodules https://github.com/apc-llc/moviemaker-cpp.git
cd moviemaker-cpp
mkdir build
cd build
cmake ..
make -j12
./moviemaker-cpp_test
```

## Acknowledgements

This work is largely based on original [example on StackOverflow](https://stackoverflow.com/questions/34511312/how-to-encode-a-video-from-several-images-generated-in-a-c-program-without-wri).
