#pragma once

#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include "ParticleSystem.h"
#include "ui_QtOpengl.h"

class QtOpengl : public QOpenGLWidget, public QOpenGLFunctions_4_3_Core
{
	Q_OBJECT

public:
	QtOpengl(QWidget *parent = Q_NULLPTR);
	~QtOpengl();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

private:
	Ui::QtOpengl			ui;
	ParticleSystem			mParticleSys;
	GLuint					mVAO, mVBO;
	QOpenGLShaderProgram	mShaderProgram;

};
