#ifndef IMAGE_H
#define IMAGE_H

#define STB_IMAGE_IMPLEMENTATION

#include <iostream>

class Image
{
public:
	unsigned char* data;
	int width, height;
	int numChannels;

	Image(const char* filePath);
	~Image();

private:
	void Flip();
	bool CheckPOT();
};

#endif