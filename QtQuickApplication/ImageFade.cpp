#include "ImageFade.h"

ImageFade::ImageFade():
	mStartTick(0.0),
	mCurTick(0.0),
	mnPreTick(0.0),
	mWidth(0),
	mHeight(0),
	mGlAtomicCounter(0),
	mGlIndexBuffer(0)
{
}

ImageFade::~ImageFade()
{
}

bool ImageFade::Init()
{
	ParticleSystem::Init();

	GL_CHECK(glGenBuffers(1, &mGlAtomicCounter));
	GL_CHECK(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, mGlAtomicCounter));
	GL_CHECK(glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(uint32_t), nullptr, GL_DYNAMIC_COPY));
	GL_CHECK(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0));

	GL_CHECK(glGenBuffers(1, &mGlIndexBuffer));
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlIndexBuffer));
	GL_CHECK(glBufferData(GL_SHADER_STORAGE_BUFFER, ParticleCapacity() * sizeof(uint32_t), nullptr, GL_DYNAMIC_COPY));
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));

	return true;
}

void ImageFade::ResetParticles(uint32_t nums)
{
	ParticleSystem::ResetParticles(nums);

	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlIndexBuffer);
	GL_CHECK(glBufferData(GL_SHADER_STORAGE_BUFFER, ParticlesCount() * sizeof(uint32_t), nullptr, GL_DYNAMIC_COPY)));
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

void ImageFade::SetSize(int32_t width, int32_t height)
{
	mWidth = width;
	mHeight = height;

	UseProgrma();
	
	GLint heightLoc = glGetUniformLocation(ProgrmaID(), "height");
	assert(!glGetError());

	GLint widthLoc = glGetUniformLocation(ProgrmaID(), "width");
	assert(!glGetError());

	GL_CHECK(glUniform1ui(widthLoc, mWidth));
	GL_CHECK(glUniform1ui(heightLoc, mHeight));
	
	ReleaseProgma();

	ResetParticles(width * height);
}

void ImageFade::SetCurrentTime(float time)
{
	mCurTick = time;

	GLint curTickLoc = glGetUniformLocation(ProgrmaID(), "curTick");
	assert(!glGetError());

	UseProgrma();
	GL_CHECK(glUniform1f(curTickLoc, mCurTick));
	ReleaseProgma();
}

void ImageFade::SetStartTime(float time)
{
	mStartTick = time;

	GLint startTickLoc = glGetUniformLocation(ProgrmaID(), "startTick");
	assert(!glGetError());

	UseProgrma();
	GL_CHECK(glUniform1f(startTickLoc, mCurTick));
	ReleaseProgma();
}

void ImageFade::Compute()
{
	UseProgrma();
	auto id = ProgrmaID();

	GLint countLoc = glGetUniformLocation(ProgrmaID(), "particleCount");
	assert(!glGetError());
	GL_CHECK(glUniform1ui(countLoc, ParticlesCount()));

	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, GlPraticlesBuffer()));
	GL_CHECK(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, GlPraticlesBuffer()));
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER,0));

	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlIndexBuffer));
	GL_CHECK(glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, mGlIndexBuffer));
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));

	uint32_t zero(0);
	GL_CHECK(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, mGlAtomicCounter));
	GL_CHECK(glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(uint32_t), &zero));
	GL_CHECK(glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 2, mGlAtomicCounter));
	GL_CHECK(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0));

	GLint widthLoc = glGetUniformLocation(ProgrmaID(), "width");
	assert(!glGetError());
	GL_CHECK(glUniform1ui(widthLoc, mWidth));
	
	GLint heightLoc = glGetUniformLocation(ProgrmaID(), "height");
	assert(!glGetError());
	GL_CHECK(glUniform1ui(heightLoc, mHeight));

	uint32_t x = ::ceil(mWidth/32.0);
	uint32_t y = ::ceil(mHeight/32.0);
	GL_CHECK(glDispatchCompute(x, y, 1));

	//GL_CHECK(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, mGlAtomicCounter));
	//uint32_t *counter = reinterpret_cast<uint32_t*>(glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_READ_WRITE));
	//assert(!glGetError());
	//GL_CHECK(glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER));
	//GL_CHECK(glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0));

	GLint preTickLoc = glGetUniformLocation(ProgrmaID(), "preTick");
	GL_CHECK(glUniform1f(preTickLoc, mCurTick));

	ReleaseProgma();
}


