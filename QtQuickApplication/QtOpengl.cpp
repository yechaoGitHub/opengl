#include "QtOpengl.h"

float vertices[] = {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,	// top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,	// bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,	// bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f	// top left 
};

unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

QtOpengl::QtOpengl(QWidget *parent)
	: QOpenGLWidget(parent),
	mVAO(0),
	mVBO(0),
	mTex(0),
	mImageVAO(0),
	mImageVBO(0),
	mImageEBO(0)
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
	assert(b);


	b = mImageProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, "ImageVertex.glsl");
	assert(b);

	b = mImageProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, "ImageFragment.glsl");
	assert(b);

	b = mImageProgram.link();
	assert(b);

	mParticleSys.Init();
	mParticleSys.ResetParticles(4096);
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

	QImage img;
	img.load("./t.jpg", "JPG");

	glGenTextures(1, &mTex);
	glBindTexture(GL_TEXTURE_2D, mTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.size().width(), img.size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits());

	glUniform1i(glGetUniformLocation(mImageProgram.programId(), "tex"), 0);
	glUniform1i(glGetUniformLocation(mShaderProgram.programId(), "tex"), 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	mParticleSys.Unmap();

	glGenVertexArrays(1, &mImageVAO);
	glGenBuffers(1, &mImageVBO);
	glGenBuffers(1, &mImageEBO);

	glBindVertexArray(mImageVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mImageVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mImageEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void QtOpengl::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void QtOpengl::paintGL()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	mParticleSys.Compute(mTimer.Now());
	Particle* particles = mParticleSys.Map();
	uint32_t parCount = mParticleSys.ParticlesCount();
	uint32_t parBufferSize = parCount * sizeof(Particle);

	mImageProgram.bind();

	glBindVertexArray(mImageVAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mImageEBO);

	glBindTexture(GL_TEXTURE_2D, mTex);
	glUniform1i(glGetUniformLocation(mImageProgram.programId(), "tex"), 0);
	glActiveTexture(GL_TEXTURE0);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	mImageProgram.release();


	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, parBufferSize, particles, GL_DYNAMIC_DRAW);

	mShaderProgram.bind();

	glBindTexture(GL_TEXTURE_2D, mTex);
	glUniform1i(glGetUniformLocation(mShaderProgram.programId(), "tex"), 0);
	glActiveTexture(GL_TEXTURE0);

	glPointSize(2.0f);
	glDrawArrays(GL_POINTS, 0, mParticleSys.ParticlesCount());
	
	mShaderProgram.release();
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
