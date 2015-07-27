#include "Image.h"
#include "stb_image.h"

Image::Image(const char* filename)
{
	data = stbi_load(filename, &width, &height, &numChannels, 0);
	if (!data)
	{
		std::cout << "Error: Could not load image (" << filename << ")\n";
		return;
	}

	if (!CheckPOT())
		std::cout << "Warning: Image does not have power of 2 dimensions (" << filename << ")\n";

	Flip();
}

Image::~Image()
{
	stbi_image_free(data);
}

void Image::Flip()
{
	int widthBytes = width * numChannels;
	unsigned char* top = 0;
	unsigned char* bottom = 0;
	unsigned char temp = 0;
	int halfHeight = height / 2;

	for (int row = 0; row < halfHeight; row++)
	{
		top = data + (row * widthBytes);
		bottom = data + ((height - row - 1) * widthBytes);
		for (int col = 0; col < widthBytes; col++)
		{
			temp = *top;
			*top = *bottom;
			*bottom = temp;

			top++;
			bottom++;
		}
	}
}

// Returns true if the image's dimensions are a power of 2
bool Image::CheckPOT()
{
	return ((width & (width - 1)) == 0 && (height & (height - 1)) == 0);
}