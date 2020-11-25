/********************************************************************************
** Form generated from reading UI file 'QtMainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTMAINWINDOW_H
#define UI_QTMAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtMainWindow
{
public:

    void setupUi(QWidget *QtMainWindow)
    {
        if (QtMainWindow->objectName().isEmpty())
            QtMainWindow->setObjectName(QString::fromUtf8("QtMainWindow"));
        QtMainWindow->resize(400, 300);

        retranslateUi(QtMainWindow);

        QMetaObject::connectSlotsByName(QtMainWindow);
    } // setupUi

    void retranslateUi(QWidget *QtMainWindow)
    {
        QtMainWindow->setWindowTitle(QApplication::translate("QtMainWindow", "QtMainWindow", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtMainWindow: public Ui_QtMainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTMAINWINDOW_H
