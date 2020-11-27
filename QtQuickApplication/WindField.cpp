//#include "WindField.h"
//
//#define DEFAULT_PARTICLE_NUM 1024
//
//ParticleSystem::ParticleSystem() :
//	mGlParticleBuffer(0),
//	mGlAtomicCounter(0),
//	mGlParticleCount(0),
//	mParticleCount(0)
//{
//}
//
//ParticleSystem::~ParticleSystem()
//{
//	ShutDown();
//}
//
//bool ParticleSystem::Init()
//{
//	if (!initializeOpenGLFunctions()) 
//	{
//		return false;
//	}
//
//	glGenBuffers(1, &mGlParticleBuffer);
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticleBuffer);
//	glBufferData(GL_SHADER_STORAGE_BUFFER, DEFAULT_PARTICLE_NUM * sizeof(Particle), nullptr, GL_DYNAMIC_COPY);
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//
//	glGenBuffers(1, &mGlAtomicCounter);
//	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, mGlAtomicCounter);
//	glBufferData(GL_ATOMIC_COUNTER_BUFFER, sizeof(GLuint), nullptr, GL_DYNAMIC_COPY);
//	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
//
//	mGlParticleCount = DEFAULT_PARTICLE_NUM;
//
//	return true;
//}
//
//void ParticleSystem::ShutDown() 
//{
//	if (mGlParticleBuffer) 
//	{
//		glDeleteBuffers(1, &mGlParticleBuffer);
//		mGlParticleBuffer = 0;
//	}
//
//	if (mGlAtomicCounter)
//	{
//		glDeleteBuffers(1, &mGlAtomicCounter);
//		mGlAtomicCounter = 0;
//	}
//}
//
//bool ParticleSystem::SetComputerShader(const std::string &shaderPath)
//{
//	bool b = mComputeShaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Compute, shaderPath.c_str());
//	if (!b) 
//	{
//		qDebug() << mComputeShaderProgram.log();
//		assert(b);
//	}
//
//	return b;
//}
//
//void ParticleSystem::AddParticle(const Particle& particle)
//{
//	//mVecParticles.push_back(particle);
//}
//
//void ParticleSystem::ResetParticles(uint32_t nums)
//{
//	mParticleCount = nums;
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticleBuffer);
//	if (mParticleCount <= mGlParticleCount) 
//	{
//		glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, mParticleCount * sizeof(Particle), nullptr);
//	}
//	else 
//	{
//		mGlParticleCount = mParticleCount * 1.5;
//		glBufferData(GL_SHADER_STORAGE_BUFFER, mParticleCount * sizeof(Particle), nullptr, GL_DYNAMIC_COPY);
//	}
//
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//}
//
//uint32_t ParticleSystem::ParticlesCount()
//{
//	return mParticleCount;
//}
//
//void ParticleSystem::Compute(float time)
//{
//	mComputeShaderProgram.bind();
//
//	GLint parcLoc = glGetUniformLocation(mComputeShaderProgram.programId(), "particleCount");
//	glUniform1i(parcLoc, mParticleCount);
//
//	GLint curTickLoc = glGetUniformLocation(mComputeShaderProgram.programId(), "curTick");
//	glUniform1f(curTickLoc, time);
//
//	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, mGlAtomicCounter);
//	glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 1, mGlAtomicCounter);
//
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticleBuffer);
//	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, mGlParticleBuffer);
//
//	double n = static_cast<double>(mParticleCount) / 1024.0;
//	GLuint x = static_cast<GLuint>(::ceil(n));
//	glDispatchCompute(x, 1, 1);
//
//	GLint preTickLoc = glGetUniformLocation(mComputeShaderProgram.programId(), "preTick");
//	glUniform1f(preTickLoc, time);
//
//	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, mGlAtomicCounter);
//	GLuint *uCounter = reinterpret_cast<GLuint*>(glMapBuffer(GL_ATOMIC_COUNTER_BUFFER, GL_READ_WRITE));
//	
//	glUnmapBuffer(GL_ATOMIC_COUNTER_BUFFER);
//	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, 0);
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//}
//
//Particle* ParticleSystem::Map()
//{
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticleBuffer);
//	Particle* ret =	reinterpret_cast<Particle*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_WRITE));
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//	return ret;
//}
//
//void ParticleSystem::Unmap()
//{
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, mGlParticleBuffer);
//	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
//	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
//}
