/********************************************************************************
** Form generated from reading UI file 'ConfigUI.ui'
**
** Created by: Qt User Interface Compiler version 5.9.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONFIGUI_H
#define UI_CONFIGUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigUIClass
{
public:
	QAction * actionExit;
	QWidget *centralWidget;
	QTabWidget *tabWidget;
	QWidget *tab;
	QPushButton *pushButton;
	QPushButton *pushButton_2;
	QPushButton *pushButton_4;
	QLabel *label;
	QFrame *line;
	QPushButton *pushButton_3;
	QLabel *label_6;
	QWidget *tab_2;
	QLabel *label_2;
	QLabel *label_3;
	QLabel *label_4;
	QSlider *horizontalSlider;
	QLabel *label_5;
	QCheckBox *checkBox;
	QSlider *horizontalSlider_2;
	QDoubleSpinBox *doubleSpinBox;
	QDoubleSpinBox *doubleSpinBox_2;
	QWidget *tab_3;
	QLineEdit *cameraPosXEdit;
	QLineEdit *cameraPosYEdit;
	QLineEdit *cameraPosZEdit;
	QLabel *label_8;
	QLabel *label_7;
	QLineEdit *cameraEyeYEdit;
	QLineEdit *cameraEyeZEdit;
	QLineEdit *cameraEyeXEdit;
	QLabel *label_9;
	QLineEdit *cameraUpYEdit;
	QLineEdit *cameraUpZEdit;
	QLineEdit *cameraUpXEdit;
	QLabel *label_10;
	QMenuBar *menuBar;
	QMenu *menuFile;
	QMenu *menuAbout;
	QStatusBar *statusBar;

	void setupUi(QMainWindow *ConfigUIClass)
	{
		if (ConfigUIClass->objectName().isEmpty())
			ConfigUIClass->setObjectName(QStringLiteral("ConfigUIClass"));
		ConfigUIClass->resize(360, 527);
		actionExit = new QAction(ConfigUIClass);
		actionExit->setObjectName(QStringLiteral("actionExit"));
		centralWidget = new QWidget(ConfigUIClass);
		centralWidget->setObjectName(QStringLiteral("centralWidget"));
		tabWidget = new QTabWidget(centralWidget);
		tabWidget->setObjectName(QStringLiteral("tabWidget"));
		tabWidget->setGeometry(QRect(0, 0, 421, 591));
		QFont font;
		font.setPointSize(10);
		tabWidget->setFont(font);
		tabWidget->setIconSize(QSize(32, 32));
		tab = new QWidget();
		tab->setObjectName(QStringLiteral("tab"));
		pushButton = new QPushButton(tab);
		pushButton->setObjectName(QStringLiteral("pushButton"));
		pushButton->setGeometry(QRect(40, 60, 81, 31));
		pushButton_2 = new QPushButton(tab);
		pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
		pushButton_2->setGeometry(QRect(250, 100, 81, 31));
		pushButton_4 = new QPushButton(tab);
		pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
		pushButton_4->setGeometry(QRect(40, 100, 81, 31));
		label = new QLabel(tab);
		label->setObjectName(QStringLiteral("label"));
		label->setGeometry(QRect(20, 20, 161, 21));
		QFont font1;
		font1.setFamily(QStringLiteral("Arial"));
		font1.setPointSize(12);
		font1.setItalic(false);
		label->setFont(font1);
		line = new QFrame(tab);
		line->setObjectName(QStringLiteral("line"));
		line->setGeometry(QRect(10, 140, 331, 21));
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);
		pushButton_3 = new QPushButton(tab);
		pushButton_3->setObjectName(QStringLiteral("pushButton_3"));
		pushButton_3->setGeometry(QRect(40, 210, 81, 31));
		label_6 = new QLabel(tab);
		label_6->setObjectName(QStringLiteral("label_6"));
		label_6->setGeometry(QRect(20, 170, 161, 21));
		label_6->setFont(font1);
		tabWidget->addTab(tab, QString());
		tab_2 = new QWidget();
		tab_2->setObjectName(QStringLiteral("tab_2"));
		label_2 = new QLabel(tab_2);
		label_2->setObjectName(QStringLiteral("label_2"));
		label_2->setGeometry(QRect(110, 150, 141, 16));
		QFont font2;
		font2.setFamily(QStringLiteral("Arial"));
		font2.setPointSize(14);
		label_2->setFont(font2);
		label_3 = new QLabel(tab_2);
		label_3->setObjectName(QStringLiteral("label_3"));
		label_3->setGeometry(QRect(20, 190, 71, 16));
		QFont font3;
		font3.setFamily(QStringLiteral("Arial"));
		font3.setPointSize(12);
		label_3->setFont(font3);
		label_4 = new QLabel(tab_2);
		label_4->setObjectName(QStringLiteral("label_4"));
		label_4->setGeometry(QRect(20, 220, 61, 16));
		label_4->setFont(font3);
		horizontalSlider = new QSlider(tab_2);
		horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
		horizontalSlider->setGeometry(QRect(130, 190, 141, 22));
		horizontalSlider->setMaximum(200);
		horizontalSlider->setSingleStep(1);
		horizontalSlider->setPageStep(10);
		horizontalSlider->setValue(100);
		horizontalSlider->setSliderPosition(100);
		horizontalSlider->setOrientation(Qt::Horizontal);
		label_5 = new QLabel(tab_2);
		label_5->setObjectName(QStringLiteral("label_5"));
		label_5->setGeometry(QRect(140, 10, 61, 16));
		label_5->setFont(font2);
		checkBox = new QCheckBox(tab_2);
		checkBox->setObjectName(QStringLiteral("checkBox"));
		checkBox->setGeometry(QRect(30, 40, 121, 21));
		checkBox->setCheckable(false);
		checkBox->setChecked(false);
		horizontalSlider_2 = new QSlider(tab_2);
		horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
		horizontalSlider_2->setGeometry(QRect(130, 220, 141, 22));
		horizontalSlider_2->setMaximum(200);
		horizontalSlider_2->setSingleStep(1);
		horizontalSlider_2->setPageStep(10);
		horizontalSlider_2->setValue(0);
		horizontalSlider_2->setSliderPosition(0);
		horizontalSlider_2->setOrientation(Qt::Horizontal);
		doubleSpinBox = new QDoubleSpinBox(tab_2);
		doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
		doubleSpinBox->setGeometry(QRect(290, 190, 51, 21));
		doubleSpinBox->setMaximum(2);
		doubleSpinBox->setSingleStep(0.01);
		doubleSpinBox->setValue(1);
		doubleSpinBox_2 = new QDoubleSpinBox(tab_2);
		doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));
		doubleSpinBox_2->setGeometry(QRect(290, 220, 51, 21));
		doubleSpinBox_2->setMaximum(2);
		doubleSpinBox_2->setSingleStep(0.01);
		tabWidget->addTab(tab_2, QString());
		tab_3 = new QWidget();
		tab_3->setObjectName(QStringLiteral("tab_3"));
		cameraPosXEdit = new QLineEdit("0",tab_3);
		cameraPosXEdit->setObjectName(QStringLiteral("cameraPosXEdit"));
		cameraPosXEdit->setGeometry(QRect(40, 90, 41, 20));
		cameraPosYEdit = new QLineEdit("0",tab_3);
		cameraPosYEdit->setObjectName(QStringLiteral("cameraPosYEdit"));
		cameraPosYEdit->setGeometry(QRect(40, 120, 41, 20));
		cameraPosZEdit = new QLineEdit("0.5",tab_3);
		cameraPosZEdit->setObjectName(QStringLiteral("cameraPosZEdit"));
		cameraPosZEdit->setGeometry(QRect(40, 150, 41, 20));
		label_8 = new QLabel(tab_3);
		label_8->setObjectName(QStringLiteral("label_8"));
		label_8->setGeometry(QRect(140, 10, 91, 16));
		label_8->setFont(font2);
		label_7 = new QLabel(tab_3);
		label_7->setObjectName(QStringLiteral("label_7"));
		label_7->setGeometry(QRect(30, 60, 71, 16));
		QFont font4;
		font4.setFamily(QStringLiteral("Arial"));
		font4.setPointSize(13);
		label_7->setFont(font4);
		cameraEyeYEdit = new QLineEdit("0",tab_3);
		cameraEyeYEdit->setObjectName(QStringLiteral("cameraEyeYEdit"));
		cameraEyeYEdit->setGeometry(QRect(160, 120, 41, 20));
		cameraEyeZEdit = new QLineEdit("-1",tab_3);
		cameraEyeZEdit->setObjectName(QStringLiteral("cameraEyeZEdit"));
		cameraEyeZEdit->setGeometry(QRect(160, 150, 41, 20));
		cameraEyeXEdit = new QLineEdit("0",tab_3);
		cameraEyeXEdit->setObjectName(QStringLiteral("cameraEyeXEdit"));
		cameraEyeXEdit->setGeometry(QRect(160, 90, 41, 20));
		label_9 = new QLabel(tab_3);
		label_9->setObjectName(QStringLiteral("label_9"));
		label_9->setGeometry(QRect(160, 50, 71, 31));
		label_9->setFont(font4);
		cameraUpYEdit = new QLineEdit("1",tab_3);
		cameraUpYEdit->setObjectName(QStringLiteral("cameraUpYEdit"));
		cameraUpYEdit->setGeometry(QRect(270, 120, 41, 20));
		cameraUpZEdit = new QLineEdit("0",tab_3);
		cameraUpZEdit->setObjectName(QStringLiteral("cameraUpZEdit"));
		cameraUpZEdit->setGeometry(QRect(270, 150, 41, 20));
		cameraUpXEdit = new QLineEdit("0",tab_3);
		cameraUpXEdit->setObjectName(QStringLiteral("cameraUpXEdit"));
		cameraUpXEdit->setGeometry(QRect(270, 90, 41, 20));
		label_10 = new QLabel(tab_3);
		label_10->setObjectName(QStringLiteral("label_10"));
		label_10->setGeometry(QRect(280, 50, 31, 31));
		label_10->setFont(font4);
		tabWidget->addTab(tab_3, QString());
		ConfigUIClass->setCentralWidget(centralWidget);
		menuBar = new QMenuBar(ConfigUIClass);
		menuBar->setObjectName(QStringLiteral("menuBar"));
		menuBar->setGeometry(QRect(0, 0, 360, 21));
		menuFile = new QMenu(menuBar);
		menuFile->setObjectName(QStringLiteral("menuFile"));
		menuAbout = new QMenu(menuBar);
		menuAbout->setObjectName(QStringLiteral("menuAbout"));
		ConfigUIClass->setMenuBar(menuBar);
		statusBar = new QStatusBar(ConfigUIClass);
		statusBar->setObjectName(QStringLiteral("statusBar"));
		ConfigUIClass->setStatusBar(statusBar);

		menuBar->addAction(menuFile->menuAction());
		menuBar->addAction(menuAbout->menuAction());
		menuFile->addAction(actionExit);

		retranslateUi(ConfigUIClass);

		tabWidget->setCurrentIndex(0);


		QMetaObject::connectSlotsByName(ConfigUIClass);
	} // setupUi

	void retranslateUi(QMainWindow *ConfigUIClass)
	{
		ConfigUIClass->setWindowTitle(QApplication::translate("ConfigUIClass", "ConfigUI", Q_NULLPTR));
		actionExit->setText(QApplication::translate("ConfigUIClass", "Exit", Q_NULLPTR));
		pushButton->setText(QApplication::translate("ConfigUIClass", "Triangulate", Q_NULLPTR));
		pushButton_2->setText(QApplication::translate("ConfigUIClass", "reset", Q_NULLPTR));
		pushButton_4->setText(QApplication::translate("ConfigUIClass", "Scan", Q_NULLPTR));
		label->setText(QApplication::translate("ConfigUIClass", "Point Cloud operations", Q_NULLPTR));
		pushButton_3->setText(QApplication::translate("ConfigUIClass", "Annonate", Q_NULLPTR));
		label_6->setText(QApplication::translate("ConfigUIClass", "Drawing", Q_NULLPTR));
		tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ConfigUIClass", "Action", Q_NULLPTR));
		label_2->setText(QApplication::translate("ConfigUIClass", "One Euro Filter", Q_NULLPTR));
		label_3->setText(QApplication::translate("ConfigUIClass", "Min. cutoff", Q_NULLPTR));
		label_4->setText(QApplication::translate("ConfigUIClass", "Beta", Q_NULLPTR));
		label_5->setText(QApplication::translate("ConfigUIClass", "Status", Q_NULLPTR));
		checkBox->setText(QApplication::translate("ConfigUIClass", "Tracked Joints", Q_NULLPTR));
		tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ConfigUIClass", "Kinect", Q_NULLPTR));
		label_8->setText(QApplication::translate("ConfigUIClass", "Transform", Q_NULLPTR));
		label_7->setText(QApplication::translate("ConfigUIClass", "Position", Q_NULLPTR));
		label_9->setText(QApplication::translate("ConfigUIClass", "Eye", Q_NULLPTR));
		label_10->setText(QApplication::translate("ConfigUIClass", "Up", Q_NULLPTR));
		tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("ConfigUIClass", "Camera", Q_NULLPTR));
		menuFile->setTitle(QApplication::translate("ConfigUIClass", "File", Q_NULLPTR));
		menuAbout->setTitle(QApplication::translate("ConfigUIClass", "About", Q_NULLPTR));
	} // retranslateUi

};

namespace Ui {
	class ConfigUIClass : public Ui_ConfigUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGUI_H
