/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QWidget>
#include "mygl.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionQuit;
    QAction *actionCamera_Controls;
    QWidget *centralWidget;
    MyGL *mygl;
    QListWidget *vertsListWidget;
    QListWidget *halfEdgesListWidget;
    QListWidget *facesListWidget;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *vertPosXSpinBox;
    QDoubleSpinBox *vertPosYSpinBox;
    QDoubleSpinBox *vertPosZSpinBox;
    QLabel *label_4;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_9;
    QDoubleSpinBox *faceBlueSpinBox;
    QLabel *label_10;
    QDoubleSpinBox *faceGreenSpinBox;
    QDoubleSpinBox *faceRedSpinBox;
    QLabel *label_11;
    QPushButton *chooseOBJ;
    QPushButton *addMidpoint;
    QPushButton *triangulateFace;
    QPushButton *catmullClark;
    QPushButton *loadJSON;
    QTreeWidget *jointsTreeWidget;
    QPushButton *meshSkinning;
    QLabel *label_13;
    QLabel *label_14;
    QDoubleSpinBox *jointPosZSpinBox;
    QLabel *label_15;
    QDoubleSpinBox *jointPosXSpinBox;
    QDoubleSpinBox *jointPosYSpinBox;
    QLabel *label_16;
    QPushButton *jointRotXPlus;
    QPushButton *jointRotYPlus;
    QPushButton *jointRotZPlus;
    QLabel *label_17;
    QPushButton *jointRotXMinus;
    QPushButton *jointRotYMinus;
    QPushButton *jointRotZMinus;
    QTextBrowser *jointRotX;
    QLabel *label_18;
    QLabel *label_19;
    QLabel *label_20;
    QTextBrowser *jointRotY;
    QTextBrowser *jointRotZ;
    QPushButton *exportUSD;
    QPushButton *changeColor;
    QPushButton *MPM;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuHelp;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1201, 498);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        actionCamera_Controls = new QAction(MainWindow);
        actionCamera_Controls->setObjectName(QString::fromUtf8("actionCamera_Controls"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        mygl = new MyGL(centralWidget);
        mygl->setObjectName(QString::fromUtf8("mygl"));
        mygl->setGeometry(QRect(11, 11, 618, 432));
        vertsListWidget = new QListWidget(centralWidget);
        vertsListWidget->setObjectName(QString::fromUtf8("vertsListWidget"));
        vertsListWidget->setEnabled(false);
        vertsListWidget->setGeometry(QRect(670, 50, 16, 171));
        halfEdgesListWidget = new QListWidget(centralWidget);
        halfEdgesListWidget->setObjectName(QString::fromUtf8("halfEdgesListWidget"));
        halfEdgesListWidget->setEnabled(false);
        halfEdgesListWidget->setGeometry(QRect(690, 50, 16, 171));
        facesListWidget = new QListWidget(centralWidget);
        facesListWidget->setObjectName(QString::fromUtf8("facesListWidget"));
        facesListWidget->setEnabled(false);
        facesListWidget->setGeometry(QRect(710, 50, 16, 171));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(650, 370, 71, 31));
        label->setAlignment(Qt::AlignCenter);
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(730, 370, 71, 31));
        label_2->setAlignment(Qt::AlignCenter);
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(810, 370, 71, 31));
        label_3->setAlignment(Qt::AlignCenter);
        vertPosXSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosXSpinBox->setObjectName(QString::fromUtf8("vertPosXSpinBox"));
        vertPosXSpinBox->setEnabled(false);
        vertPosXSpinBox->setGeometry(QRect(1010, 261, 51, 21));
        vertPosXSpinBox->setMinimum(-99.989999999999995);
        vertPosYSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosYSpinBox->setObjectName(QString::fromUtf8("vertPosYSpinBox"));
        vertPosYSpinBox->setEnabled(false);
        vertPosYSpinBox->setGeometry(QRect(1071, 261, 51, 21));
        vertPosYSpinBox->setMinimum(-99.989999999999995);
        vertPosZSpinBox = new QDoubleSpinBox(centralWidget);
        vertPosZSpinBox->setObjectName(QString::fromUtf8("vertPosZSpinBox"));
        vertPosZSpinBox->setEnabled(false);
        vertPosZSpinBox->setGeometry(QRect(1131, 261, 51, 21));
        vertPosZSpinBox->setMinimum(-99.989999999999995);
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setEnabled(false);
        label_4->setGeometry(QRect(940, 260, 71, 21));
        label_5 = new QLabel(centralWidget);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setEnabled(false);
        label_5->setGeometry(QRect(1009, 285, 51, 20));
        label_5->setAlignment(Qt::AlignCenter);
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setEnabled(false);
        label_6->setGeometry(QRect(1070, 285, 51, 20));
        label_6->setAlignment(Qt::AlignCenter);
        label_7 = new QLabel(centralWidget);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setEnabled(false);
        label_7->setGeometry(QRect(1130, 285, 51, 20));
        label_7->setAlignment(Qt::AlignCenter);
        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setEnabled(false);
        label_8->setGeometry(QRect(1130, 330, 51, 20));
        label_8->setAlignment(Qt::AlignCenter);
        label_9 = new QLabel(centralWidget);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setEnabled(false);
        label_9->setGeometry(QRect(1009, 330, 51, 20));
        label_9->setAlignment(Qt::AlignCenter);
        faceBlueSpinBox = new QDoubleSpinBox(centralWidget);
        faceBlueSpinBox->setObjectName(QString::fromUtf8("faceBlueSpinBox"));
        faceBlueSpinBox->setEnabled(false);
        faceBlueSpinBox->setGeometry(QRect(1131, 310, 51, 21));
        faceBlueSpinBox->setMaximum(1.000000000000000);
        faceBlueSpinBox->setSingleStep(0.050000000000000);
        label_10 = new QLabel(centralWidget);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setEnabled(false);
        label_10->setGeometry(QRect(940, 310, 71, 21));
        faceGreenSpinBox = new QDoubleSpinBox(centralWidget);
        faceGreenSpinBox->setObjectName(QString::fromUtf8("faceGreenSpinBox"));
        faceGreenSpinBox->setEnabled(false);
        faceGreenSpinBox->setGeometry(QRect(1071, 310, 51, 21));
        faceGreenSpinBox->setMaximum(1.000000000000000);
        faceGreenSpinBox->setSingleStep(0.050000000000000);
        faceRedSpinBox = new QDoubleSpinBox(centralWidget);
        faceRedSpinBox->setObjectName(QString::fromUtf8("faceRedSpinBox"));
        faceRedSpinBox->setEnabled(false);
        faceRedSpinBox->setGeometry(QRect(1010, 310, 51, 21));
        faceRedSpinBox->setMaximum(1.000000000000000);
        faceRedSpinBox->setSingleStep(0.050000000000000);
        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setEnabled(false);
        label_11->setGeometry(QRect(1070, 330, 51, 20));
        label_11->setAlignment(Qt::AlignCenter);
        chooseOBJ = new QPushButton(centralWidget);
        chooseOBJ->setObjectName(QString::fromUtf8("chooseOBJ"));
        chooseOBJ->setGeometry(QRect(650, 10, 91, 31));
        addMidpoint = new QPushButton(centralWidget);
        addMidpoint->setObjectName(QString::fromUtf8("addMidpoint"));
        addMidpoint->setEnabled(false);
        addMidpoint->setGeometry(QRect(740, 140, 91, 32));
        triangulateFace = new QPushButton(centralWidget);
        triangulateFace->setObjectName(QString::fromUtf8("triangulateFace"));
        triangulateFace->setEnabled(false);
        triangulateFace->setGeometry(QRect(740, 200, 91, 32));
        catmullClark = new QPushButton(centralWidget);
        catmullClark->setObjectName(QString::fromUtf8("catmullClark"));
        catmullClark->setEnabled(false);
        catmullClark->setGeometry(QRect(740, 170, 91, 32));
        loadJSON = new QPushButton(centralWidget);
        loadJSON->setObjectName(QString::fromUtf8("loadJSON"));
        loadJSON->setGeometry(QRect(760, 10, 101, 31));
        jointsTreeWidget = new QTreeWidget(centralWidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        jointsTreeWidget->setHeaderItem(__qtreewidgetitem);
        jointsTreeWidget->setObjectName(QString::fromUtf8("jointsTreeWidget"));
        jointsTreeWidget->setEnabled(false);
        jointsTreeWidget->setGeometry(QRect(650, 50, 16, 171));
        meshSkinning = new QPushButton(centralWidget);
        meshSkinning->setObjectName(QString::fromUtf8("meshSkinning"));
        meshSkinning->setEnabled(false);
        meshSkinning->setGeometry(QRect(830, 50, 100, 32));
        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setEnabled(false);
        label_13->setGeometry(QRect(660, 255, 61, 31));
        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setEnabled(false);
        label_14->setGeometry(QRect(740, 285, 51, 16));
        label_14->setAlignment(Qt::AlignCenter);
        jointPosZSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosZSpinBox->setObjectName(QString::fromUtf8("jointPosZSpinBox"));
        jointPosZSpinBox->setEnabled(false);
        jointPosZSpinBox->setGeometry(QRect(860, 260, 51, 21));
        jointPosZSpinBox->setMinimum(-99.989999999999995);
        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setEnabled(false);
        label_15->setGeometry(QRect(860, 286, 51, 16));
        label_15->setAlignment(Qt::AlignCenter);
        jointPosXSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosXSpinBox->setObjectName(QString::fromUtf8("jointPosXSpinBox"));
        jointPosXSpinBox->setEnabled(false);
        jointPosXSpinBox->setGeometry(QRect(740, 260, 51, 21));
        jointPosXSpinBox->setMinimum(-99.989999999999995);
        jointPosYSpinBox = new QDoubleSpinBox(centralWidget);
        jointPosYSpinBox->setObjectName(QString::fromUtf8("jointPosYSpinBox"));
        jointPosYSpinBox->setEnabled(false);
        jointPosYSpinBox->setGeometry(QRect(800, 260, 51, 21));
        jointPosYSpinBox->setMinimum(-99.989999999999995);
        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setEnabled(false);
        label_16->setGeometry(QRect(800, 285, 51, 16));
        label_16->setAlignment(Qt::AlignCenter);
        jointRotXPlus = new QPushButton(centralWidget);
        jointRotXPlus->setObjectName(QString::fromUtf8("jointRotXPlus"));
        jointRotXPlus->setEnabled(false);
        jointRotXPlus->setGeometry(QRect(740, 50, 41, 32));
        jointRotYPlus = new QPushButton(centralWidget);
        jointRotYPlus->setObjectName(QString::fromUtf8("jointRotYPlus"));
        jointRotYPlus->setEnabled(false);
        jointRotYPlus->setGeometry(QRect(740, 80, 41, 32));
        jointRotZPlus = new QPushButton(centralWidget);
        jointRotZPlus->setObjectName(QString::fromUtf8("jointRotZPlus"));
        jointRotZPlus->setEnabled(false);
        jointRotZPlus->setGeometry(QRect(740, 110, 41, 32));
        label_17 = new QLabel(centralWidget);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setEnabled(false);
        label_17->setGeometry(QRect(660, 310, 71, 21));
        jointRotXMinus = new QPushButton(centralWidget);
        jointRotXMinus->setObjectName(QString::fromUtf8("jointRotXMinus"));
        jointRotXMinus->setEnabled(false);
        jointRotXMinus->setGeometry(QRect(780, 50, 41, 32));
        jointRotYMinus = new QPushButton(centralWidget);
        jointRotYMinus->setObjectName(QString::fromUtf8("jointRotYMinus"));
        jointRotYMinus->setEnabled(false);
        jointRotYMinus->setGeometry(QRect(780, 80, 41, 32));
        jointRotZMinus = new QPushButton(centralWidget);
        jointRotZMinus->setObjectName(QString::fromUtf8("jointRotZMinus"));
        jointRotZMinus->setEnabled(false);
        jointRotZMinus->setGeometry(QRect(780, 110, 41, 32));
        jointRotX = new QTextBrowser(centralWidget);
        jointRotX->setObjectName(QString::fromUtf8("jointRotX"));
        jointRotX->setEnabled(false);
        jointRotX->setGeometry(QRect(740, 312, 51, 20));
        label_18 = new QLabel(centralWidget);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setEnabled(false);
        label_18->setGeometry(QRect(860, 341, 51, 16));
        label_18->setAlignment(Qt::AlignCenter);
        label_19 = new QLabel(centralWidget);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setEnabled(false);
        label_19->setGeometry(QRect(740, 340, 51, 16));
        label_19->setAlignment(Qt::AlignCenter);
        label_20 = new QLabel(centralWidget);
        label_20->setObjectName(QString::fromUtf8("label_20"));
        label_20->setEnabled(false);
        label_20->setGeometry(QRect(800, 340, 51, 16));
        label_20->setAlignment(Qt::AlignCenter);
        jointRotY = new QTextBrowser(centralWidget);
        jointRotY->setObjectName(QString::fromUtf8("jointRotY"));
        jointRotY->setEnabled(false);
        jointRotY->setGeometry(QRect(800, 310, 51, 20));
        jointRotZ = new QTextBrowser(centralWidget);
        jointRotZ->setObjectName(QString::fromUtf8("jointRotZ"));
        jointRotZ->setEnabled(false);
        jointRotZ->setGeometry(QRect(860, 310, 51, 20));
        exportUSD = new QPushButton(centralWidget);
        exportUSD->setObjectName(QString::fromUtf8("exportUSD"));
        exportUSD->setGeometry(QRect(880, 10, 101, 31));
        changeColor = new QPushButton(centralWidget);
        changeColor->setObjectName(QString::fromUtf8("changeColor"));
        changeColor->setEnabled(false);
        changeColor->setGeometry(QRect(830, 80, 101, 31));
        MPM = new QPushButton(centralWidget);
        MPM->setObjectName(QString::fromUtf8("MPM"));
        MPM->setGeometry(QRect(1000, 10, 101, 31));
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1201, 36));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuFile->addAction(actionQuit);
        menuHelp->addAction(actionCamera_Controls);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "OpenGLDemo", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "Quit", nullptr));
#if QT_CONFIG(shortcut)
        actionQuit->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Q", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCamera_Controls->setText(QCoreApplication::translate("MainWindow", "Camera Controls", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Vertices", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Half-Edges", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Faces", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Vertex Pos", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        label_6->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Z", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Blue", nullptr));
        label_9->setText(QCoreApplication::translate("MainWindow", "Red", nullptr));
        label_10->setText(QCoreApplication::translate("MainWindow", "Face Color", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "Green", nullptr));
        chooseOBJ->setText(QCoreApplication::translate("MainWindow", "Choose OBJ", nullptr));
        addMidpoint->setText(QCoreApplication::translate("MainWindow", "Add Midpoint", nullptr));
        triangulateFace->setText(QCoreApplication::translate("MainWindow", "Triangulate ", nullptr));
        catmullClark->setText(QCoreApplication::translate("MainWindow", "Smooth", nullptr));
        loadJSON->setText(QCoreApplication::translate("MainWindow", "Choose JSON", nullptr));
        meshSkinning->setText(QCoreApplication::translate("MainWindow", "Mesh Skinning", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "Joint Pos", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "Z", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        jointRotXPlus->setText(QCoreApplication::translate("MainWindow", "x+5", nullptr));
        jointRotYPlus->setText(QCoreApplication::translate("MainWindow", "y+5", nullptr));
        jointRotZPlus->setText(QCoreApplication::translate("MainWindow", "z+5", nullptr));
        label_17->setText(QCoreApplication::translate("MainWindow", "Joint Rotate", nullptr));
        jointRotXMinus->setText(QCoreApplication::translate("MainWindow", "x-5", nullptr));
        jointRotYMinus->setText(QCoreApplication::translate("MainWindow", "y-5", nullptr));
        jointRotZMinus->setText(QCoreApplication::translate("MainWindow", "z-5", nullptr));
        label_18->setText(QCoreApplication::translate("MainWindow", "Z", nullptr));
        label_19->setText(QCoreApplication::translate("MainWindow", "X", nullptr));
        label_20->setText(QCoreApplication::translate("MainWindow", "Y", nullptr));
        exportUSD->setText(QCoreApplication::translate("MainWindow", "export USD", nullptr));
        changeColor->setText(QCoreApplication::translate("MainWindow", "Change Color", nullptr));
        MPM->setText(QCoreApplication::translate("MainWindow", "Snow Sim", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
