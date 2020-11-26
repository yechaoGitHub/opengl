#include <QGuiApplication>
#include "QtMainWindow.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
	QtMainWindow window;

    window.resize(QSize(658, 851));
	window.show();

    return app.exec();
}
