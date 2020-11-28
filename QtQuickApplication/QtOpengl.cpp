#include "QtOpengl.h"

float vertices[] = {
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,	
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,	
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,	
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f	
};

unsigned int indices[] = {
	0, 1, 3, 
	1, 2, 3  
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

	bool b = initializeOpenGLFunctions();
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

	QImage img;
	img.load("./t.jpg", "JPG");

	mImageFade.Init();
	mImageFade.SetComputerShader("ImageFade.glsl");
	mImageFade.SetSize(100, 100);
	mImageFade.Compute();

	Particle* particles = mImageFade.Map();
	GL_CHECK(glGenVertexArrays(1, &mVAO));
	GL_CHECK(glGenBuffers(1, &mVBO));
	GL_CHECK(glBindVertexArray(mVAO));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mVBO));

	uint32_t parCount = mImageFade.ParticlesCount();
	uint32_t parBufferSize = parCount * sizeof(Particle);

	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, parBufferSize, particles, GL_STATIC_DRAW));
	GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)STRUCT_MEMBER_OFFSET(Particle, position)));
	GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)STRUCT_MEMBER_OFFSET(Particle, coordinate)));
	GL_CHECK(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)STRUCT_MEMBER_OFFSET(Particle, velocity)));
	GL_CHECK(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)STRUCT_MEMBER_OFFSET(Particle, acceleration)));
	GL_CHECK(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), (void*)STRUCT_MEMBER_OFFSET(Particle, color)));

	GL_CHECK(glEnableVertexAttribArray(0));
	GL_CHECK(glEnableVertexAttribArray(1));
	GL_CHECK(glEnableVertexAttribArray(2));
	GL_CHECK(glEnableVertexAttribArray(3));
	GL_CHECK(glEnableVertexAttribArray(4));

	GL_CHECK(glBindVertexArray(0));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));

	GL_CHECK(glGenTextures(1, &mTex));
	GL_CHECK(glBindTexture(GL_TEXTURE_2D, mTex));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GL_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GL_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.size().width(), img.size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img.bits()));

	mImageProgram.bind();
	GLint texLoc = glGetUniformLocation(mImageProgram.programId(), "tex");
	assert(!glGetError());
	GL_CHECK(glUniform1i(texLoc, 0));
	mImageProgram.release();

	mShaderProgram.bind();
	GLint shTextLoc = glGetUniformLocation(mShaderProgram.programId(), "tex");
	assert(!glGetError());
	GL_CHECK(glUniform1i(shTextLoc, 0));
	mShaderProgram.release();

	GL_CHECK(glBindTexture(GL_TEXTURE_2D, 0));

	mImageFade.Unmap();

	GL_CHECK(glGenVertexArrays(1, &mImageVAO));
	GL_CHECK(glGenBuffers(1, &mImageVBO));
	GL_CHECK(glGenBuffers(1, &mImageEBO));

	GL_CHECK(glBindVertexArray(mImageVAO));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mImageVBO));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));

	GL_CHECK(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0));
	GL_CHECK(glEnableVertexAttribArray(0));

	GL_CHECK(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat))));
	GL_CHECK(glEnableVertexAttribArray(1));

	GL_CHECK(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat))));
	GL_CHECK(glEnableVertexAttribArray(2));

	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mImageEBO));
	GL_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	GL_CHECK(glBindVertexArray(0));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	mTimer.Start();
	mImageFade.SetStartTime(mTimer.Now());
}

void QtOpengl::resizeGL(int w, int h)
{
	GL_CHECK(glViewport(0, 0, w, h));
}

void QtOpengl::paintGL()
{
	GL_CHECK(glClearColor(1.f, 1.f, 1.f, 1.f));
	GL_CHECK(glClear(GL_COLOR_BUFFER_BIT));
	
	mImageFade.SetCurrentTime(mTimer.Now());

	mImageFade.Compute();
	Particle* particles = mImageFade.Map();
	uint32_t parCount = mImageFade.ParticlesCount();
	uint32_t parBufferSize = parCount * sizeof(Particle);

	//mImageProgram.bind();

	//GL_CHECK(glBindVertexArray(mImageVAO));
	//GL_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mImageEBO));

	//GL_CHECK(glBindTexture(GL_TEXTURE_2D, mTex));

	//GLint imTexLoc = glGetUniformLocation(mImageProgram.programId(), "tex");
	//assert(!glGetError());
	//GL_CHECK(glUniform1i(imTexLoc, 0));
	//GL_CHECK(glActiveTexture(GL_TEXTURE0));

	//GL_CHECK(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));

	//mImageProgram.release();

	GL_CHECK(glBindVertexArray(mVAO));
	GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, mVBO));
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, parBufferSize, particles, GL_DYNAMIC_DRAW));

	mShaderProgram.bind();

	GL_CHECK(glBindTexture(GL_TEXTURE_2D, mTex));
	GLint shTexLoc = glGetUniformLocation(mShaderProgram.programId(), "tex");
	assert(!glGetError());
	GL_CHECK(glUniform1i(shTexLoc, 0));
	GL_CHECK(glActiveTexture(GL_TEXTURE0));

	GL_CHECK(glPointSize(5.0f));
	GL_CHECK(glDrawArrays(GL_POINTS, 0, mImageFade.ParticlesCount()));
	
	mShaderProgram.release();
	mImageFade.Unmap();
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
