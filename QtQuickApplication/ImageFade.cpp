#include "ImageFade.h"

ImageFade::ImageFade()
{
}

ImageFade::~ImageFade()
{
}

bool ImageFade::Init()
{
	bool b = ParticleSystem::Init();
	assert(b);

	return true;
}

void ImageFade::SetSize(int32_t width, int32_t height)
{
	mWidth = width;
	mHeight = height;
}

void ImageFade::SetCurrentTime(float time)
{
	mCurTick = time;
}


