#pragma once
#include "Particle.h"
#include <vector>
#include <string>
#include <assert.h>
#include "ParticleSystem.h"

class ImageFade : public ParticleSystem 
{
public:
	ImageFade();
	~ImageFade();
	
	bool Init();
	void ResetParticles(uint32_t nums);
	void Compute();

	void SetSize(int32_t width, int32_t height);
	void SetCurrentTime(float time);
	void SetStartTime(float time);

private:
	float					mStartTick;
	float					mCurTick;
	float					mnPreTick;
	int32_t					mWidth;
	int32_t					mHeight;
	GLuint					mGlAtomicCounter;
	GLuint					mGlIndexBuffer;
};
