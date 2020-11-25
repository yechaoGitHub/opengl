#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "QtMainWindow.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
	QtMainWindow window;

	window.show();

    return app.exec();
}
