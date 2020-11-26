#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() :
	mGlParticleBuffer(0),
	mGlParticleCount(0),
	mParticleCount(0)
{
}

ParticleSystem::~ParticleSystem()
{
	ShutDown();
}

bool ParticleSystem::Init()
{
	return initializeOpenGLFunctions();
}

void ParticleSystem::ShutDown()
{
	if (mGlParticleBuffer)
	{
		glDeleteBuffers(1, &mGlParticleBuffer);
		mGlParticleBuffer = 0;
	}
}

bool ParticleSystem::SetComputerShader(const std::string& shaderPath)
{
	bool b = mComputeShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Compute, shaderPath.c_str());
	if (!b)
	{
		qDebug() << mComputeShaderProgram.log();
	}

	return b;
}

void ParticleSystem::AddParticle(const Particle& particle)
{
	mParticleCount++;
	if (mParticleCount > mGlParticleCount) 
	{
		mGlParticleCount = mParticleCount * 1.5;
		GLuint newBuffer(0);

		Particle* particles = Map();
		glGenBuffers(GL_SHADER_STORAGE_BUFFER, &newBuffer);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, newBuffer);
		glBufferData(GL_SHADER_STORAGE_BUFFER, mGlParticleCount, nullptr, GL_DYNAMIC_COPY);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, mParticleCount, particles);
		Unmap();

		glDeleteBuffers(GL_SHADER_STORAGE_BUFFER, &mGlParticleBuffer);
		mGlParticleBuffer = newBuffer;
	}

	Particle* particles = Map();
	particles[mParticleCount] = particle;
}

void ParticleSystem::ResetParticles(uint32_t nums)
{
	mParticleCount = nums;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticleBuffer);
	if (mParticleCount <= mGlParticleCount)
	{
		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, mParticleCount * sizeof(Particle), nullptr);
	}
	else
	{
		mGlParticleCount = mParticleCount * 1.5;
		glBufferData(GL_SHADER_STORAGE_BUFFER, mGlParticleCount * sizeof(Particle), nullptr, GL_DYNAMIC_COPY);
	}

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

uint32_t ParticleSystem::ParticlesCount()
{
	return mParticleCount;
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
}

void ParticleSystem::ReleaseProgma()
{
}

Particle* ParticleSystem::Map()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticleBuffer);
	Particle* ret = reinterpret_cast<Particle*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	return ret;
}

void ParticleSystem::Unmap()
{
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticleBuffer);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}