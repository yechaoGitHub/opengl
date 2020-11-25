#include "QtOpengl.h"

static const char* VERTEX_SHADER_CODE =
"#version 330 \n"
"layout(location = 0) in vec3 posVertex;\n"
"void main() {\n"
"  gl_Position = vec4(posVertex, 1.0f);\n"
"}\n";

static const char* FRAGMENT_SHADER_CODE =
"#version 330\n"
"out vec4 fragColor;\n"
"void main() {\n"
"  fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
"}\n";

static const GLfloat VERTEX_DATA[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, 0.5f, 0.0f
};

QtOpengl::QtOpengl(QWidget *parent)
	: QOpenGLWidget(parent),
	mVAO(0),
	mVBO(0)
{
	ui.setupUi(this);

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
	mParticleSys.ResetParticles(4096);
	mParticleSys.SetComputerShader("Particle.glsl");
	mParticleSys.Compute();

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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void QtOpengl::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void QtOpengl::paintGL()
{
	glClearColor(1.f, 1.f, 1.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	mShaderProgram.bind();
	glBindVertexArray(mVAO);

	glPointSize(2.0f);
	glDrawArrays(GL_POINTS, 0, mParticleSys.ParticlesCount());

	//glDrawArrays(GL_TRIANGLES, 0, 3);

}
