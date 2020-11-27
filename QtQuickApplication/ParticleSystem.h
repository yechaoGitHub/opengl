#pragma once
#include "Particle.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <vector>
#include <string>

class ParticleSystem : protected QOpenGLFunctions_4_3_Core
{
public:
	ParticleSystem();
	~ParticleSystem();

	virtual bool Init();
	virtual void ShutDown();
	virtual bool SetComputerShader(const std::string& shaderPath);
	virtual void AddParticle(const Particle& particle);
	virtual void ResetParticles(uint32_t nums);
	uint32_t ParticlesCount();
	uint32_t ParticleCapacity();
	virtual void Compute();
	Particle* Map();
	void Unmap();

	GLuint ProgrmaID();
	void UseProgrma();
	void ReleaseProgma();
	GLuint GlPraticlesBuffer();

private:
	QOpenGLShaderProgram		mComputeShaderProgram;
	GLuint						mGlParticlesBuffer;
	uint32_t					mGlParticlesCount;
	uint32_t					mParticleCount;
};

#define GL_CHECK(FuncCall)    FuncCall; assert(!glGetError())
