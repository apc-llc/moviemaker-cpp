#ifdef TESTING

#include "gtest/gtest.h"
#include "movie.h"

#include <vector>

using namespace std;

TEST(movie, random_pixels)
{
	const unsigned int width = 1024;
	const unsigned int height = 768;
	const unsigned int nframes = 4;//128;

	Movie movie("random_pixels", width, height);
	
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
}

TEST(movie, svg_image)
{
	const unsigned int width = 1024;
	const unsigned int height = 768;
	const unsigned int nframes = 4;//128;

	Movie movie("svg_image", width, height);

	for (unsigned int iframe = 0; iframe < nframes; iframe++)
	{
		movie.addFrame(TIGER_SVG);
	}
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

#endif // TESTING

