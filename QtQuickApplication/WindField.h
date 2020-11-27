//#pragma once
//#include "Particle.h"
//#include <QOpenGLFunctions_4_3_Core>
//#include <QOpenGLShaderProgram>
//#include <vector>
//#include <string>
//
//class ParticleSystem : public QOpenGLFunctions_4_3_Core
//{
//public:
//	ParticleSystem();
//	~ParticleSystem();
//
//	bool Init();
//	void ShutDown();
//	bool SetComputerShader(const std::string &shaderPath);
//	void AddParticle(const Particle &particle);
//	void ResetParticles(uint32_t nums);
//	uint32_t ParticlesCount();
//	void Compute(float time);
//	Particle* Map();
//	void Unmap();
//
//private:
//	QOpenGLShaderProgram	mComputeShaderProgram;
//	GLuint					mGlParticleBuffer;
//	GLuint					mGlAtomicCounter;
//	uint32_t				mGlParticleCount;
//	uint32_t				mParticleCount;
//};
//
