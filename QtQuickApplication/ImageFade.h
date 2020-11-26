#pragma once
#include "Particle.h"
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <vector>
#include <string>
#include <assert.h>
#include "ParticleSystem.h"

class ImageFade : protected ParticleSystem 
{
public:
	ImageFade();
	~ImageFade();
	
	bool Init() override;
	
	void SetSize(int32_t width, int32_t height);
	void SetCurrentTime(float time);

private:
	float mCurTick;
	float mnPreTick;
	int32_t mWidth;
	int32_t mHeight;
};
