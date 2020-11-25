#include "QtMainWindow.h"
#include <QEvent.h>

QtMainWindow::QtMainWindow(QWidget *parent)
	: QMainWindow(parent),
	mCentralWidget(nullptr),
	mGlWidget(nullptr)
{
	mCentralWidget = new QWidget(this);
	mGlWidget = new QtOpengl(mCentralWidget);
	this->setCentralWidget(mCentralWidget);
}

QtMainWindow::~QtMainWindow()
{
}


void QtMainWindow::resizeEvent(QResizeEvent *event)
{	
	mGlWidget->resize(event->size());
}