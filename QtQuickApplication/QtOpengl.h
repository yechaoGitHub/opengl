#pragma once

#include <QOpenGLWidget>
#include <QDebug>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>
#include "GlTimer.h"
#include "ImageFade.h"
#include "ui_QtOpengl.h"

class QtOpengl : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
	Q_OBJECT

public:
	QtOpengl(QWidget *parent = Q_NULLPTR);
	~QtOpengl();

protected:
	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	void mousePressEvent(QMouseEvent* e) override;

private:
	Ui::QtOpengl				ui;
	ImageFade					mImageFade;
	QOpenGLFunctions_4_3_Core*	mGlFunction;
	GlTimer						mTimer;
	GLuint						mVAO, mVBO, mTex, mImageVAO, mImageVBO, mImageEBO;
	QOpenGLShaderProgram		mShaderProgram;
	QOpenGLShaderProgram		mImageProgram;
};
