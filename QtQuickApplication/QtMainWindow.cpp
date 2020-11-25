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

	QObject::connect(&mTimer, &QTimer::timeout, this, &QtMainWindow::Tick);
	mTimer.start(1000 / 60);
}

QtMainWindow::~QtMainWindow()
{
}

void QtMainWindow::resizeEvent(QResizeEvent *event)
{	
	mGlWidget->resize(event->size());
}

void QtMainWindow::Tick()
{
	mGlWidget->update();
}

