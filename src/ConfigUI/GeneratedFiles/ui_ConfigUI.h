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
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ConfigUIClass
{
public:
	QAction * actionExit;
	QAction *actionHelp;
	QAction *actionCredits;
	QWidget *centralWidget;
	QHBoxLayout *horizontalLayout;
	QTabWidget *tabWidget;
	QWidget *tab;
	QGridLayout *gridLayout_3;
	QScrollArea *scrollArea_2;
	QWidget *scrollAreaWidgetContents;
	QGridLayout *gridLayout_8;
	QGroupBox *groupBox;
	QVBoxLayout *verticalLayout;
	QPushButton *pushButton_4;
	QPushButton *pushButton;
	QPushButton *pushButton_2;
	QWidget *tab_2;
	QGridLayout *gridLayout_2;
	QScrollArea *scrollArea;
	QWidget *scrollAreaWidgetContents_2;
	QGridLayout *gridLayout_7;
	QGroupBox *groupBox_3;
	QGridLayout *gridLayout_6;
	QGridLayout *gridLayout_5;
	QLabel *label_3;
	QSlider *horizontalSlider;
	QDoubleSpinBox *doubleSpinBox;
	QLabel *label_4;
	QSlider *horizontalSlider_2;
	QDoubleSpinBox *doubleSpinBox_2;
	QGroupBox *groupBox_2;
	QGridLayout *gridLayout;
	QGridLayout *gridLayout_4;
	QDoubleSpinBox *SpinBoxPosY;
	QLabel *label_11;
	QLabel *label_12;
	QLabel *label_13;
	QDoubleSpinBox *SpinBoxPosX;
	QDoubleSpinBox *SpinBoxPosZ;
	QDoubleSpinBox *SpinBoxEyeX;
	QDoubleSpinBox *SpinBoxEyeY;
	QDoubleSpinBox *SpinBoxEyeZ;
	QDoubleSpinBox *SpinBoxUpX;
	QDoubleSpinBox *SpinBoxUpY;
	QDoubleSpinBox *SpinBoxUpZ;
	QLabel *label_5;
	QCheckBox *checkBoxTracked;
	QWidget *tab_3;
	QMenuBar *menuBar;
	QMenu *menuFile;
	QMenu *menuAbout;
	QStatusBar *statusBar;

	void setupUi(QMainWindow *ConfigUIClass)
	{
		if (ConfigUIClass->objectName().isEmpty())
			ConfigUIClass->setObjectName(QStringLiteral("ConfigUIClass"));
		ConfigUIClass->resize(319, 598);
		actionExit = new QAction(ConfigUIClass);
		actionExit->setObjectName(QStringLiteral("actionExit"));
		actionHelp = new QAction(ConfigUIClass);
		actionHelp->setObjectName(QStringLiteral("actionHelp"));
		actionCredits = new QAction(ConfigUIClass);
		actionCredits->setObjectName(QStringLiteral("actionCredits"));
		centralWidget = new QWidget(ConfigUIClass);
		centralWidget->setObjectName(QStringLiteral("centralWidget"));
		horizontalLayout = new QHBoxLayout(centralWidget);
		horizontalLayout->setSpacing(6);
		horizontalLayout->setContentsMargins(11, 11, 11, 11);
		horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
		tabWidget = new QTabWidget(centralWidget);
		tabWidget->setObjectName(QStringLiteral("tabWidget"));
		QFont font;
		font.setPointSize(10);
		tabWidget->setFont(font);
		tabWidget->setIconSize(QSize(32, 32));
		tab = new QWidget();
		tab->setObjectName(QStringLiteral("tab"));
		gridLayout_3 = new QGridLayout(tab);
		gridLayout_3->setSpacing(6);
		gridLayout_3->setContentsMargins(11, 11, 11, 11);
		gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
		gridLayout_3->setContentsMargins(0, 0, 0, 0);
		scrollArea_2 = new QScrollArea(tab);
		scrollArea_2->setObjectName(QStringLiteral("scrollArea_2"));
		scrollArea_2->setWidgetResizable(true);
		scrollAreaWidgetContents = new QWidget();
		scrollAreaWidgetContents->setObjectName(QStringLiteral("scrollAreaWidgetContents"));
		scrollAreaWidgetContents->setGeometry(QRect(0, 0, 293, 508));
		gridLayout_8 = new QGridLayout(scrollAreaWidgetContents);
		gridLayout_8->setSpacing(6);
		gridLayout_8->setContentsMargins(11, 11, 11, 11);
		gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
		groupBox = new QGroupBox(scrollAreaWidgetContents);
		groupBox->setObjectName(QStringLiteral("groupBox"));
		QFont font1;
		font1.setPointSize(14);
		groupBox->setFont(font1);
		verticalLayout = new QVBoxLayout(groupBox);
		verticalLayout->setSpacing(6);
		verticalLayout->setContentsMargins(11, 11, 11, 11);
		verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
		pushButton_4 = new QPushButton(groupBox);
		pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
		pushButton_4->setFont(font);

		verticalLayout->addWidget(pushButton_4);

		pushButton = new QPushButton(groupBox);
		pushButton->setObjectName(QStringLiteral("pushButton"));
		pushButton->setFont(font);

		verticalLayout->addWidget(pushButton);

		pushButton_2 = new QPushButton(groupBox);
		pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
		pushButton_2->setFont(font);

		verticalLayout->addWidget(pushButton_2);


		gridLayout_8->addWidget(groupBox, 0, 0, 1, 1);

		scrollArea_2->setWidget(scrollAreaWidgetContents);

		gridLayout_3->addWidget(scrollArea_2, 0, 0, 1, 1);

		tabWidget->addTab(tab, QString());
		tab_2 = new QWidget();
		tab_2->setObjectName(QStringLiteral("tab_2"));
		gridLayout_2 = new QGridLayout(tab_2);
		gridLayout_2->setSpacing(6);
		gridLayout_2->setContentsMargins(11, 11, 11, 11);
		gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
		gridLayout_2->setContentsMargins(0, 10, 0, 0);
		scrollArea = new QScrollArea(tab_2);
		scrollArea->setObjectName(QStringLiteral("scrollArea"));
		scrollArea->setWidgetResizable(true);
		scrollAreaWidgetContents_2 = new QWidget();
		scrollAreaWidgetContents_2->setObjectName(QStringLiteral("scrollAreaWidgetContents_2"));
		scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 310, 453));
		gridLayout_7 = new QGridLayout(scrollAreaWidgetContents_2);
		gridLayout_7->setSpacing(6);
		gridLayout_7->setContentsMargins(11, 11, 11, 11);
		gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
		gridLayout_7->setSizeConstraint(QLayout::SetDefaultConstraint);
		gridLayout_7->setHorizontalSpacing(3);
		gridLayout_7->setVerticalSpacing(2);
		gridLayout_7->setContentsMargins(0, -1, 15, -1);
		groupBox_3 = new QGroupBox(scrollAreaWidgetContents_2);
		groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
		groupBox_3->setFont(font1);
		gridLayout_6 = new QGridLayout(groupBox_3);
		gridLayout_6->setSpacing(6);
		gridLayout_6->setContentsMargins(11, 11, 11, 11);
		gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
		gridLayout_5 = new QGridLayout();
		gridLayout_5->setSpacing(6);
		gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
		label_3 = new QLabel(groupBox_3);
		label_3->setObjectName(QStringLiteral("label_3"));
		QFont font2;
		font2.setFamily(QStringLiteral("Arial"));
		font2.setPointSize(12);
		label_3->setFont(font2);

		gridLayout_5->addWidget(label_3, 0, 0, 1, 1);

		horizontalSlider = new QSlider(groupBox_3);
		horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
		horizontalSlider->setMaximum(200);
		horizontalSlider->setSingleStep(1);
		horizontalSlider->setPageStep(10);
		horizontalSlider->setValue(100);
		horizontalSlider->setSliderPosition(100);
		horizontalSlider->setOrientation(Qt::Horizontal);

		gridLayout_5->addWidget(horizontalSlider, 0, 1, 1, 1);

		doubleSpinBox = new QDoubleSpinBox(groupBox_3);
		doubleSpinBox->setObjectName(QStringLiteral("doubleSpinBox"));
		doubleSpinBox->setMinimum(0.01);
		doubleSpinBox->setMaximum(2);
		doubleSpinBox->setSingleStep(0.01);
		doubleSpinBox->setValue(1);

		gridLayout_5->addWidget(doubleSpinBox, 0, 2, 1, 1);

		label_4 = new QLabel(groupBox_3);
		label_4->setObjectName(QStringLiteral("label_4"));
		label_4->setFont(font2);

		gridLayout_5->addWidget(label_4, 1, 0, 1, 1);

		horizontalSlider_2 = new QSlider(groupBox_3);
		horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
		horizontalSlider_2->setMaximum(200);
		horizontalSlider_2->setSingleStep(1);
		horizontalSlider_2->setPageStep(10);
		horizontalSlider_2->setValue(0);
		horizontalSlider_2->setSliderPosition(0);
		horizontalSlider_2->setOrientation(Qt::Horizontal);

		gridLayout_5->addWidget(horizontalSlider_2, 1, 1, 1, 1);

		doubleSpinBox_2 = new QDoubleSpinBox(groupBox_3);
		doubleSpinBox_2->setObjectName(QStringLiteral("doubleSpinBox_2"));
		doubleSpinBox_2->setMaximum(2);
		doubleSpinBox_2->setSingleStep(0.01);

		gridLayout_5->addWidget(doubleSpinBox_2, 1, 2, 1, 1);


		gridLayout_6->addLayout(gridLayout_5, 0, 0, 1, 1);


		gridLayout_7->addWidget(groupBox_3, 0, 0, 1, 1);

		groupBox_2 = new QGroupBox(scrollAreaWidgetContents_2);
		groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
		groupBox_2->setFont(font1);
		gridLayout = new QGridLayout(groupBox_2);
		gridLayout->setSpacing(6);
		gridLayout->setContentsMargins(11, 11, 11, 11);
		gridLayout->setObjectName(QStringLiteral("gridLayout"));
		gridLayout_4 = new QGridLayout();
		gridLayout_4->setSpacing(6);
		gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
		gridLayout_4->setHorizontalSpacing(6);
		gridLayout_4->setContentsMargins(10, 10, 10, 10);
		SpinBoxPosY = new QDoubleSpinBox(groupBox_2);
		SpinBoxPosY->setObjectName(QStringLiteral("SpinBoxPosY"));
		SpinBoxPosY->setMinimum(-30);
		SpinBoxPosY->setMaximum(30);
		SpinBoxPosY->setSingleStep(0.01);

		gridLayout_4->addWidget(SpinBoxPosY, 2, 1, 1, 1);

		label_11 = new QLabel(groupBox_2);
		label_11->setObjectName(QStringLiteral("label_11"));
		QFont font3;
		font3.setFamily(QStringLiteral("Arial"));
		font3.setPointSize(13);
		label_11->setFont(font3);

		gridLayout_4->addWidget(label_11, 0, 3, 1, 1);

		label_12 = new QLabel(groupBox_2);
		label_12->setObjectName(QStringLiteral("label_12"));
		label_12->setFont(font3);

		gridLayout_4->addWidget(label_12, 0, 2, 1, 1);

		label_13 = new QLabel(groupBox_2);
		label_13->setObjectName(QStringLiteral("label_13"));
		label_13->setFont(font3);

		gridLayout_4->addWidget(label_13, 0, 1, 1, 1);

		SpinBoxPosX = new QDoubleSpinBox(groupBox_2);
		SpinBoxPosX->setObjectName(QStringLiteral("SpinBoxPosX"));
		SpinBoxPosX->setMinimum(-30);
		SpinBoxPosX->setMaximum(30);
		SpinBoxPosX->setSingleStep(0.01);

		gridLayout_4->addWidget(SpinBoxPosX, 1, 1, 1, 1);

		SpinBoxPosZ = new QDoubleSpinBox(groupBox_2);
		SpinBoxPosZ->setObjectName(QStringLiteral("SpinBoxPosZ"));
		SpinBoxPosZ->setMinimum(-30);
		SpinBoxPosZ->setMaximum(30);
		SpinBoxPosZ->setSingleStep(0.01);

		gridLayout_4->addWidget(SpinBoxPosZ, 3, 1, 1, 1);

		SpinBoxEyeX = new QDoubleSpinBox(groupBox_2);
		SpinBoxEyeX->setObjectName(QStringLiteral("SpinBoxEyeX"));
		SpinBoxEyeX->setMinimum(-30);
		SpinBoxEyeX->setMaximum(30);
		SpinBoxEyeX->setSingleStep(0.01);
		SpinBoxEyeX->setValue(0);

		gridLayout_4->addWidget(SpinBoxEyeX, 1, 2, 1, 1);

		SpinBoxEyeY = new QDoubleSpinBox(groupBox_2);
		SpinBoxEyeY->setObjectName(QStringLiteral("SpinBoxEyeY"));
		SpinBoxEyeY->setMinimum(-30);
		SpinBoxEyeY->setMaximum(30);
		SpinBoxEyeY->setSingleStep(0.01);

		gridLayout_4->addWidget(SpinBoxEyeY, 2, 2, 1, 1);

		SpinBoxEyeZ = new QDoubleSpinBox(groupBox_2);
		SpinBoxEyeZ->setObjectName(QStringLiteral("SpinBoxEyeZ"));
		SpinBoxEyeZ->setMinimum(-30);
		SpinBoxEyeZ->setMaximum(30);
		SpinBoxEyeZ->setSingleStep(0.01);

		gridLayout_4->addWidget(SpinBoxEyeZ, 3, 2, 1, 1);

		SpinBoxUpX = new QDoubleSpinBox(groupBox_2);
		SpinBoxUpX->setObjectName(QStringLiteral("SpinBoxUpX"));
		SpinBoxUpX->setMinimum(-30);
		SpinBoxUpX->setMaximum(30);
		SpinBoxUpX->setSingleStep(0.01);

		gridLayout_4->addWidget(SpinBoxUpX, 1, 3, 1, 1);

		SpinBoxUpY = new QDoubleSpinBox(groupBox_2);
		SpinBoxUpY->setObjectName(QStringLiteral("SpinBoxUpY"));
		SpinBoxUpY->setMinimum(-30);
		SpinBoxUpY->setMaximum(30);
		SpinBoxUpY->setSingleStep(0.01);

		gridLayout_4->addWidget(SpinBoxUpY, 2, 3, 1, 1);

		SpinBoxUpZ = new QDoubleSpinBox(groupBox_2);
		SpinBoxUpZ->setObjectName(QStringLiteral("SpinBoxUpZ"));
		SpinBoxUpZ->setMinimum(-30);
		SpinBoxUpZ->setMaximum(30);
		SpinBoxUpZ->setSingleStep(0.01);

		gridLayout_4->addWidget(SpinBoxUpZ, 3, 3, 1, 1);


		gridLayout->addLayout(gridLayout_4, 0, 0, 1, 1);


		gridLayout_7->addWidget(groupBox_2, 1, 0, 1, 1);

		scrollArea->setWidget(scrollAreaWidgetContents_2);

		gridLayout_2->addWidget(scrollArea, 2, 0, 1, 2);

		label_5 = new QLabel(tab_2);
		label_5->setObjectName(QStringLiteral("label_5"));
		QFont font4;
		font4.setFamily(QStringLiteral("Arial"));
		font4.setPointSize(14);
		label_5->setFont(font4);

		gridLayout_2->addWidget(label_5, 0, 0, 1, 1);

		checkBoxTracked = new QCheckBox(tab_2);
		checkBoxTracked->setObjectName(QStringLiteral("checkBoxTracked"));
		checkBoxTracked->setCheckable(false);
		checkBoxTracked->setChecked(false);

		gridLayout_2->addWidget(checkBoxTracked, 0, 1, 1, 1);

		tabWidget->addTab(tab_2, QString());
		tab_3 = new QWidget();
		tab_3->setObjectName(QStringLiteral("tab_3"));
		tabWidget->addTab(tab_3, QString());

		horizontalLayout->addWidget(tabWidget);

		ConfigUIClass->setCentralWidget(centralWidget);
		menuBar = new QMenuBar(ConfigUIClass);
		menuBar->setObjectName(QStringLiteral("menuBar"));
		menuBar->setGeometry(QRect(0, 0, 319, 21));
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
		menuAbout->addAction(actionHelp);
		menuAbout->addAction(actionCredits);

		retranslateUi(ConfigUIClass);

		tabWidget->setCurrentIndex(1);


		QMetaObject::connectSlotsByName(ConfigUIClass);
	} // setupUi

	void retranslateUi(QMainWindow *ConfigUIClass)
	{
		ConfigUIClass->setWindowTitle(QApplication::translate("ConfigUIClass", "ConfigUI", Q_NULLPTR));
		actionExit->setText(QApplication::translate("ConfigUIClass", "Exit", Q_NULLPTR));
		actionHelp->setText(QApplication::translate("ConfigUIClass", "Help", Q_NULLPTR));
		actionCredits->setText(QApplication::translate("ConfigUIClass", "Credits", Q_NULLPTR));
		groupBox->setTitle(QApplication::translate("ConfigUIClass", "Operations", Q_NULLPTR));
		pushButton_4->setText(QApplication::translate("ConfigUIClass", "Scan", Q_NULLPTR));
		pushButton->setText(QApplication::translate("ConfigUIClass", "Triangulate", Q_NULLPTR));
		pushButton_2->setText(QApplication::translate("ConfigUIClass", "reset", Q_NULLPTR));
		tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ConfigUIClass", "Action", Q_NULLPTR));
		groupBox_3->setTitle(QApplication::translate("ConfigUIClass", "One Euro Filter", Q_NULLPTR));
		label_3->setText(QApplication::translate("ConfigUIClass", "Min. cutoff", Q_NULLPTR));
		label_4->setText(QApplication::translate("ConfigUIClass", "Beta", Q_NULLPTR));
		groupBox_2->setTitle(QApplication::translate("ConfigUIClass", "Camera", Q_NULLPTR));
		label_11->setText(QApplication::translate("ConfigUIClass", "Up", Q_NULLPTR));
		label_12->setText(QApplication::translate("ConfigUIClass", "Eye", Q_NULLPTR));
		label_13->setText(QApplication::translate("ConfigUIClass", "Position", Q_NULLPTR));
		SpinBoxPosX->setSuffix(QString());
		label_5->setText(QApplication::translate("ConfigUIClass", "Status", Q_NULLPTR));
		checkBoxTracked->setText(QApplication::translate("ConfigUIClass", "Tracked Joints", Q_NULLPTR));
		tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ConfigUIClass", "Kinect", Q_NULLPTR));
		tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("ConfigUIClass", "Utility", Q_NULLPTR));
		menuFile->setTitle(QApplication::translate("ConfigUIClass", "File", Q_NULLPTR));
		menuAbout->setTitle(QApplication::translate("ConfigUIClass", "About", Q_NULLPTR));
	} // retranslateUi

};

namespace Ui {
	class ConfigUIClass : public Ui_ConfigUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGUI_H
