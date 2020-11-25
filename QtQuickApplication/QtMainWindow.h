#pragma once

#include <QMainWindow>
#include "QtOpengl.h"
#include "ui_QtMainWindow.h"

class QtMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QtMainWindow(QWidget *parent = Q_NULLPTR);
	~QtMainWindow();
private:
	QWidget *mCentralWidget;
	QtOpengl *mGlWidget;
	Ui::QtMainWindow ui;

	virtual void resizeEvent(QResizeEvent *event) override;

};
