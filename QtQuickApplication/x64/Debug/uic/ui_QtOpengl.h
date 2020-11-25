/********************************************************************************
** Form generated from reading UI file 'QtOpengl.ui'
**
** Created by: Qt User Interface Compiler version 5.12.10
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTOPENGL_H
#define UI_QTOPENGL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtOpengl
{
public:

    void setupUi(QWidget *QtOpengl)
    {
        if (QtOpengl->objectName().isEmpty())
            QtOpengl->setObjectName(QString::fromUtf8("QtOpengl"));
        QtOpengl->resize(400, 300);

        retranslateUi(QtOpengl);

        QMetaObject::connectSlotsByName(QtOpengl);
    } // setupUi

    void retranslateUi(QWidget *QtOpengl)
    {
        QtOpengl->setWindowTitle(QApplication::translate("QtOpengl", "QtOpengl", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtOpengl: public Ui_QtOpengl {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTOPENGL_H
