#pragma once
#include "Particle.h"
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include <vector>
#include <string>

class ParticleSystem : public QOpenGLFunctions_4_3_Core
{
public:
	ParticleSystem();
	~ParticleSystem();

	virtual bool Init();
	virtual void ShutDown();
	virtual bool SetComputerShader(const std::string& shaderPath);
	virtual void AddParticle(const Particle& particle);
	virtual void ResetParticles(uint32_t nums);
	virtual uint32_t ParticlesCount();
	virtual void Compute();
	virtual Particle* Map();
	virtual void Unmap();

	GLuint ProgrmaID();
	void UseProgrma();
	void ReleaseProgma();

private:
	QOpenGLShaderProgram	mComputeShaderProgram;
	GLuint					mGlParticleBuffer;
	uint32_t				mGlParticleCount;
	uint32_t				mParticleCount;
};
