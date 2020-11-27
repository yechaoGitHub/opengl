#include "ParticleSystem.h"


#define DEFAULT_PARTICLE_NUM 1024

ParticleSystem::ParticleSystem() :
	mGlParticlesBuffer(0),
	mGlParticlesCount(0),
	mParticleCount(0)
{
	
}

ParticleSystem::~ParticleSystem()
{
	ShutDown();
}

bool ParticleSystem::Init()
{
	bool b = initializeOpenGLFunctions();
	assert(b);

	GL_CHECK(glGenBuffers(1,&mGlParticlesBuffer));
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticlesBuffer));
	GL_CHECK(glBufferData(GL_SHADER_STORAGE_BUFFER, DEFAULT_PARTICLE_NUM * sizeof(Particle), nullptr, GL_DYNAMIC_COPY));
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));

	return true;
}

void ParticleSystem::ShutDown()
{
	if (mGlParticlesBuffer)
	{
		GL_CHECK(glDeleteBuffers(1, &mGlParticlesBuffer));
		mGlParticlesBuffer = 0;
	}
}

bool ParticleSystem::SetComputerShader(const std::string& shaderPath)
{
	bool b = mComputeShaderProgram.addShaderFromSourceFile(QOpenGLShader::Compute, shaderPath.c_str());
	if (!b)
	{
		qDebug() << mComputeShaderProgram.log();
	}

	b = mComputeShaderProgram.link();

	return b;
}

void ParticleSystem::AddParticle(const Particle& particle)
{
	mParticleCount++;
	if (mParticleCount > mGlParticlesCount) 
	{
		mGlParticlesCount = mParticleCount * 1.5;
		GLuint newBuffer(0);

		Particle* particles = Map();
		GL_CHECK(glGenBuffers(1, &newBuffer));
		GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, newBuffer));
		GL_CHECK(glBufferData(GL_SHADER_STORAGE_BUFFER, mGlParticlesCount, nullptr, GL_DYNAMIC_COPY));
		GL_CHECK(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, mParticleCount, particles));
		Unmap();

		GL_CHECK(glDeleteBuffers(GL_SHADER_STORAGE_BUFFER, &mGlParticlesBuffer));
		mGlParticlesBuffer = newBuffer;
	}

	Particle* particles = Map();
	particles[mParticleCount] = particle;
}

void ParticleSystem::ResetParticles(uint32_t nums)
{
	mParticleCount = nums;
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticlesBuffer));
	if (mParticleCount <= mGlParticlesCount)
	{
		GL_CHECK(glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, mParticleCount * sizeof(Particle), nullptr));
	}
	else
	{
		mGlParticlesCount = mParticleCount * 1.5;
		GL_CHECK(glBufferData(GL_SHADER_STORAGE_BUFFER, mGlParticlesCount * sizeof(Particle), nullptr, GL_DYNAMIC_COPY));
	}

	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}

uint32_t ParticleSystem::ParticlesCount()
{
	return mParticleCount;
}

uint32_t ParticleSystem::ParticleCapacity()
{
	return mGlParticlesCount;
}

void ParticleSystem::Compute()
{

}

GLuint ParticleSystem::ProgrmaID()
{
	return mComputeShaderProgram.programId();
}

void ParticleSystem::UseProgrma()
{
	bool b = mComputeShaderProgram.bind();
	assert(b);
}

void ParticleSystem::ReleaseProgma()
{
	mComputeShaderProgram.release();
}

GLuint ParticleSystem::GlPraticlesBuffer()
{
	return mGlParticlesBuffer;
}

Particle* ParticleSystem::Map()
{
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticlesBuffer));
	Particle* ret = reinterpret_cast<Particle*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
	assert(!glGetError());
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
	return ret;
}

void ParticleSystem::Unmap()
{
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticlesBuffer));
	GL_CHECK(glUnmapBuffer(GL_SHADER_STORAGE_BUFFER));
	GL_CHECK(glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0));
}