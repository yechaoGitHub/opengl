#pragma once

#include <QMainWindow>
#include <QTimer>
#include "QtOpengl.h"
#include "ui_QtMainWindow.h"

class QtMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QtMainWindow(QWidget *parent = Q_NULLPTR);
	~QtMainWindow();

private:
	Ui::QtMainWindow ui;
	QWidget *mCentralWidget;
	QtOpengl *mGlWidget;
	QTimer	mTimer;

	virtual void resizeEvent(QResizeEvent *event) override;

private slots:
	void Tick();
};
