/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Sun Apr 7 14:33:54 2013
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QSlider>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "basicopenglview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionAdd_Model;
    QAction *actionRemove_Selected;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout;
    BasicOpenGLView *oglwidget;
    QVBoxLayout *vertLayout;
    QGroupBox *modelsBox;
    QListWidget *modelList;
    QPushButton *addModel;
    QPushButton *removeModel;
    QCheckBox *orthoProj;
    QCheckBox *postMult;
    QGroupBox *groupBox;
    QPushButton *traceButton;
    QGroupBox *translationGB;
    QSlider *ZTransSlider;
    QSlider *XTransSlider;
    QLabel *xLabel_3;
    QLabel *yLabel_3;
    QSlider *YTransSlider;
    QLabel *zLabel_3;
    QGroupBox *rotationGB;
    QSlider *ZRotSlider;
    QLabel *zLabel;
    QLabel *xLabel;
    QLabel *yLabel;
    QSlider *XRotSlider;
    QSlider *YRotSlider;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1009, 907);
        MainWindow->setMaximumSize(QSize(16777215, 16777215));
        actionAdd_Model = new QAction(MainWindow);
        actionAdd_Model->setObjectName(QString::fromUtf8("actionAdd_Model"));
        actionRemove_Selected = new QAction(MainWindow);
        actionRemove_Selected->setObjectName(QString::fromUtf8("actionRemove_Selected"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy);
        horizontalLayout = new QHBoxLayout(centralWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        oglwidget = new BasicOpenGLView(centralWidget);
        oglwidget->setObjectName(QString::fromUtf8("oglwidget"));
        oglwidget->setEnabled(true);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(oglwidget->sizePolicy().hasHeightForWidth());
        oglwidget->setSizePolicy(sizePolicy1);
        oglwidget->setMaximumSize(QSize(700, 700));

        horizontalLayout->addWidget(oglwidget);

        vertLayout = new QVBoxLayout();
        vertLayout->setSpacing(0);
        vertLayout->setObjectName(QString::fromUtf8("vertLayout"));
        vertLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        vertLayout->setContentsMargins(10, 0, -1, -1);
        modelsBox = new QGroupBox(centralWidget);
        modelsBox->setObjectName(QString::fromUtf8("modelsBox"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(modelsBox->sizePolicy().hasHeightForWidth());
        modelsBox->setSizePolicy(sizePolicy2);
        modelsBox->setMinimumSize(QSize(250, 450));
        modelList = new QListWidget(modelsBox);
        modelList->setObjectName(QString::fromUtf8("modelList"));
        modelList->setGeometry(QRect(10, 30, 231, 351));
        modelList->setFocusPolicy(Qt::StrongFocus);
        modelList->setSelectionRectVisible(true);
        addModel = new QPushButton(modelsBox);
        addModel->setObjectName(QString::fromUtf8("addModel"));
        addModel->setGeometry(QRect(40, 390, 75, 23));
        removeModel = new QPushButton(modelsBox);
        removeModel->setObjectName(QString::fromUtf8("removeModel"));
        removeModel->setGeometry(QRect(150, 390, 78, 23));
        orthoProj = new QCheckBox(modelsBox);
        orthoProj->setObjectName(QString::fromUtf8("orthoProj"));
        orthoProj->setGeometry(QRect(10, 420, 141, 17));
        postMult = new QCheckBox(modelsBox);
        postMult->setObjectName(QString::fromUtf8("postMult"));
        postMult->setGeometry(QRect(150, 420, 91, 17));

        vertLayout->addWidget(modelsBox);

        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        sizePolicy2.setHeightForWidth(groupBox->sizePolicy().hasHeightForWidth());
        groupBox->setSizePolicy(sizePolicy2);
        groupBox->setMinimumSize(QSize(250, 135));
        traceButton = new QPushButton(groupBox);
        traceButton->setObjectName(QString::fromUtf8("traceButton"));
        traceButton->setGeometry(QRect(14, 20, 221, 101));

        vertLayout->addWidget(groupBox);

        translationGB = new QGroupBox(centralWidget);
        translationGB->setObjectName(QString::fromUtf8("translationGB"));
        sizePolicy2.setHeightForWidth(translationGB->sizePolicy().hasHeightForWidth());
        translationGB->setSizePolicy(sizePolicy2);
        translationGB->setMinimumSize(QSize(250, 135));
        ZTransSlider = new QSlider(translationGB);
        ZTransSlider->setObjectName(QString::fromUtf8("ZTransSlider"));
        ZTransSlider->setGeometry(QRect(10, 110, 211, 22));
        ZTransSlider->setMinimum(-100);
        ZTransSlider->setMaximum(100);
        ZTransSlider->setOrientation(Qt::Horizontal);
        XTransSlider = new QSlider(translationGB);
        XTransSlider->setObjectName(QString::fromUtf8("XTransSlider"));
        XTransSlider->setGeometry(QRect(10, 30, 211, 22));
        XTransSlider->setMinimum(-100);
        XTransSlider->setMaximum(100);
        XTransSlider->setSingleStep(0);
        XTransSlider->setValue(0);
        XTransSlider->setOrientation(Qt::Horizontal);
        xLabel_3 = new QLabel(translationGB);
        xLabel_3->setObjectName(QString::fromUtf8("xLabel_3"));
        xLabel_3->setGeometry(QRect(110, 10, 61, 16));
        yLabel_3 = new QLabel(translationGB);
        yLabel_3->setObjectName(QString::fromUtf8("yLabel_3"));
        yLabel_3->setGeometry(QRect(110, 50, 51, 20));
        YTransSlider = new QSlider(translationGB);
        YTransSlider->setObjectName(QString::fromUtf8("YTransSlider"));
        YTransSlider->setGeometry(QRect(10, 70, 211, 22));
        YTransSlider->setMinimum(-100);
        YTransSlider->setMaximum(100);
        YTransSlider->setOrientation(Qt::Horizontal);
        zLabel_3 = new QLabel(translationGB);
        zLabel_3->setObjectName(QString::fromUtf8("zLabel_3"));
        zLabel_3->setGeometry(QRect(110, 90, 61, 16));

        vertLayout->addWidget(translationGB);

        rotationGB = new QGroupBox(centralWidget);
        rotationGB->setObjectName(QString::fromUtf8("rotationGB"));
        sizePolicy2.setHeightForWidth(rotationGB->sizePolicy().hasHeightForWidth());
        rotationGB->setSizePolicy(sizePolicy2);
        rotationGB->setMinimumSize(QSize(250, 140));
        ZRotSlider = new QSlider(rotationGB);
        ZRotSlider->setObjectName(QString::fromUtf8("ZRotSlider"));
        ZRotSlider->setGeometry(QRect(10, 110, 211, 22));
        ZRotSlider->setMinimum(0);
        ZRotSlider->setMaximum(628);
        ZRotSlider->setSingleStep(4);
        ZRotSlider->setOrientation(Qt::Horizontal);
        zLabel = new QLabel(rotationGB);
        zLabel->setObjectName(QString::fromUtf8("zLabel"));
        zLabel->setGeometry(QRect(110, 90, 51, 16));
        xLabel = new QLabel(rotationGB);
        xLabel->setObjectName(QString::fromUtf8("xLabel"));
        xLabel->setGeometry(QRect(110, 10, 61, 16));
        yLabel = new QLabel(rotationGB);
        yLabel->setObjectName(QString::fromUtf8("yLabel"));
        yLabel->setGeometry(QRect(110, 50, 51, 16));
        XRotSlider = new QSlider(rotationGB);
        XRotSlider->setObjectName(QString::fromUtf8("XRotSlider"));
        XRotSlider->setGeometry(QRect(10, 30, 211, 22));
        XRotSlider->setMinimum(0);
        XRotSlider->setMaximum(628);
        XRotSlider->setSingleStep(4);
        XRotSlider->setOrientation(Qt::Horizontal);
        YRotSlider = new QSlider(rotationGB);
        YRotSlider->setObjectName(QString::fromUtf8("YRotSlider"));
        YRotSlider->setGeometry(QRect(10, 70, 211, 22));
        YRotSlider->setMinimum(0);
        YRotSlider->setMaximum(628);
        YRotSlider->setSingleStep(4);
        YRotSlider->setOrientation(Qt::Horizontal);

        vertLayout->addWidget(rotationGB);


        horizontalLayout->addLayout(vertLayout);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        actionAdd_Model->setText(QApplication::translate("MainWindow", "Add Model...", 0, QApplication::UnicodeUTF8));
        actionRemove_Selected->setText(QApplication::translate("MainWindow", "Remove Selected", 0, QApplication::UnicodeUTF8));
        modelsBox->setTitle(QApplication::translate("MainWindow", "Models", 0, QApplication::UnicodeUTF8));
        addModel->setText(QApplication::translate("MainWindow", "Add Model", 0, QApplication::UnicodeUTF8));
        removeModel->setText(QApplication::translate("MainWindow", "Remove Model", 0, QApplication::UnicodeUTF8));
        orthoProj->setText(QApplication::translate("MainWindow", "Orthographic Projection", 0, QApplication::UnicodeUTF8));
        postMult->setText(QApplication::translate("MainWindow", "Post Multiply", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "RayTracer", 0, QApplication::UnicodeUTF8));
        traceButton->setText(QApplication::translate("MainWindow", "Trace Scene", 0, QApplication::UnicodeUTF8));
        translationGB->setTitle(QApplication::translate("MainWindow", "Translation", 0, QApplication::UnicodeUTF8));
        xLabel_3->setText(QApplication::translate("MainWindow", "X", 0, QApplication::UnicodeUTF8));
        yLabel_3->setText(QApplication::translate("MainWindow", "Y", 0, QApplication::UnicodeUTF8));
        zLabel_3->setText(QApplication::translate("MainWindow", "Z", 0, QApplication::UnicodeUTF8));
        rotationGB->setTitle(QApplication::translate("MainWindow", "Rotation", 0, QApplication::UnicodeUTF8));
        zLabel->setText(QApplication::translate("MainWindow", "Z", 0, QApplication::UnicodeUTF8));
        xLabel->setText(QApplication::translate("MainWindow", "X", 0, QApplication::UnicodeUTF8));
        yLabel->setText(QApplication::translate("MainWindow", "Y", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
