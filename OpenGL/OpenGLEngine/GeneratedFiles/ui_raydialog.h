/********************************************************************************
** Form generated from reading UI file 'raydialog.ui'
**
** Created: Sun Apr 7 13:13:13 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RAYDIALOG_H
#define UI_RAYDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QGraphicsView>
#include <QtGui/QHeaderView>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RayDialog
{
public:
    QGraphicsView *graphicsView;

    void setupUi(QWidget *RayDialog)
    {
        if (RayDialog->objectName().isEmpty())
            RayDialog->setObjectName(QString::fromUtf8("RayDialog"));
        RayDialog->resize(700, 700);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(RayDialog->sizePolicy().hasHeightForWidth());
        RayDialog->setSizePolicy(sizePolicy);
        graphicsView = new QGraphicsView(RayDialog);
        graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
        graphicsView->setGeometry(QRect(0, 0, 700, 700));
        sizePolicy.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
        graphicsView->setSizePolicy(sizePolicy);
        graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        retranslateUi(RayDialog);

        QMetaObject::connectSlotsByName(RayDialog);
    } // setupUi

    void retranslateUi(QWidget *RayDialog)
    {
        RayDialog->setWindowTitle(QApplication::translate("RayDialog", "RayDialog", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RayDialog: public Ui_RayDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RAYDIALOG_H
