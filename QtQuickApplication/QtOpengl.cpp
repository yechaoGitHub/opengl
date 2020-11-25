#include "QtOpengl.h"

QtOpengl::QtOpengl(QWidget *parent)
	: QOpenGLWidget(parent),
	mVAO(0),
	mVBO(0)
{
	ui.setupUi(this);
	mTimer.Start();
}

QtOpengl::~QtOpengl()
{
	
}

void QtOpengl::initializeGL()
{
	bool b(false);

	b = initializeOpenGLFunctions();
	assert(b);

	b = mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "Vertex.glsl");
	assert(b);

	b = mShaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "Fragment.glsl");
	assert(b);

	b = mShaderProgram.link();

	qDebug() << mShaderProgram.log();
	assert(b);

	mParticleSys.Init();
	mParticleSys.ResetParticles(1024);
	mParticleSys.SetComputerShader("Particle.glsl");
	mParticleSys.Compute(mTimer.Now());

	Particle* particles = mParticleSys.Map();
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);

	uint32_t parCount = mParticleSys.ParticlesCount();
	uint32_t parBufferSize = parCount * sizeof(Particle);

	glBufferData(GL_ARRAY_BUFFER, parBufferSize, particles, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)STRUCT_MEMBER_OFFSET(Particle, position));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)STRUCT_MEMBER_OFFSET(Particle, velocity));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)STRUCT_MEMBER_OFFSET(Particle, acceleration));
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)STRUCT_MEMBER_OFFSET(Particle, color));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	mParticleSys.Unmap();

}

void QtOpengl::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void QtOpengl::paintGL()
{
	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	mParticleSys.Compute(mTimer.Now());
	Particle* particles = mParticleSys.Map();
	uint32_t parCount = mParticleSys.ParticlesCount();
	uint32_t parBufferSize = parCount * sizeof(Particle);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, parBufferSize, particles, GL_DYNAMIC_DRAW);

	mShaderProgram.bind();

	glPointSize(2.0f);
	glDrawArrays(GL_LINES, 0, mParticleSys.ParticlesCount());

	mParticleSys.Unmap();
}

void QtOpengl::mousePressEvent(QMouseEvent* e)
{
	if (mTimer.Running()) 
	{
		mTimer.Stop();
	}
	else 
	{
		mTimer.Start();
	}
}
