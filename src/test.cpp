#ifdef TESTING

#include "gtest/gtest.h"
#include "movie.h"

#include <vector>

using namespace std;

const unsigned int width = 1024;
const unsigned int height = 768;
const unsigned int nframes = 128;

TEST(movie, random_pixels_write)
{
	MovieWriter writer("random_pixels", width, height, 30);

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

		writer.addFrame(&pixels[0]);
	}
}

TEST(movie, random_pixels_read)
{
	MovieReader reader("random_pixels", width, height);
	MovieWriter writer("random_pixels2", width, height, reader.getFrameRate());

	vector<uint8_t> pixels;	
	while (1)
	{
		if (!reader.getFrame(pixels))
			break;
	
		writer.addFrame(&pixels[0]);
	}
}

TEST(movie, svg_image_write)
{
	MovieWriter movie("svg_image", width, height);

	for (unsigned int iframe = 0; iframe < nframes; iframe++)
	{
		movie.addFrame(TIGER_SVG);
	}
}

TEST(movie, svg_image_read)
{	
	MovieReader reader("svg_image", width, height);
	MovieWriter writer("svg_image2", width, height);

	vector<uint8_t> pixels;	
	while (1)
	{
		if (!reader.getFrame(pixels))
			break;
	
		writer.addFrame(&pixels[0]);
	}
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

#endif // TESTING

