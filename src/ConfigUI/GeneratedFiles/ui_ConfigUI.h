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
#include <QtWidgets/QFrame>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
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
	QWidget *tab_2;
	QMenuBar *menuBar;
	QMenu *menuFile;
	QMenu *menuAbout;
	QToolBar *mainToolBar;
	QStatusBar *statusBar;

	void setupUi(QMainWindow *ConfigUIClass)
	{
		if (ConfigUIClass->objectName().isEmpty())
			ConfigUIClass->setObjectName(QStringLiteral("ConfigUIClass"));
		ConfigUIClass->resize(367, 551);
		actionExit = new QAction(ConfigUIClass);
		actionExit->setObjectName(QStringLiteral("actionExit"));
		centralWidget = new QWidget(ConfigUIClass);
		centralWidget->setObjectName(QStringLiteral("centralWidget"));
		tabWidget = new QTabWidget(centralWidget);
		tabWidget->setObjectName(QStringLiteral("tabWidget"));
		tabWidget->setGeometry(QRect(0, 0, 361, 521));
		tab = new QWidget();
		tab->setObjectName(QStringLiteral("tab"));
		pushButton = new QPushButton(tab);
		pushButton->setObjectName(QStringLiteral("pushButton"));
		pushButton->setGeometry(QRect(30, 100, 75, 23));
		pushButton_2 = new QPushButton(tab);
		pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
		pushButton_2->setGeometry(QRect(260, 20, 75, 23));
		pushButton_4 = new QPushButton(tab);
		pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
		pushButton_4->setGeometry(QRect(130, 100, 75, 23));
		label = new QLabel(tab);
		label->setObjectName(QStringLiteral("label"));
		label->setGeometry(QRect(30, 20, 121, 21));
		QFont font;
		font.setFamily(QStringLiteral("Arial"));
		font.setPointSize(12);
		font.setItalic(false);
		label->setFont(font);
		line = new QFrame(tab);
		line->setObjectName(QStringLiteral("line"));
		line->setGeometry(QRect(10, 140, 331, 21));
		line->setFrameShape(QFrame::HLine);
		line->setFrameShadow(QFrame::Sunken);
		tabWidget->addTab(tab, QString());
		tab_2 = new QWidget();
		tab_2->setObjectName(QStringLiteral("tab_2"));
		tabWidget->addTab(tab_2, QString());
		ConfigUIClass->setCentralWidget(centralWidget);
		menuBar = new QMenuBar(ConfigUIClass);
		menuBar->setObjectName(QStringLiteral("menuBar"));
		menuBar->setGeometry(QRect(0, 0, 367, 21));
		menuFile = new QMenu(menuBar);
		menuFile->setObjectName(QStringLiteral("menuFile"));
		menuAbout = new QMenu(menuBar);
		menuAbout->setObjectName(QStringLiteral("menuAbout"));
		ConfigUIClass->setMenuBar(menuBar);
		mainToolBar = new QToolBar(ConfigUIClass);
		mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
		ConfigUIClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
		statusBar = new QStatusBar(ConfigUIClass);
		statusBar->setObjectName(QStringLiteral("statusBar"));
		ConfigUIClass->setStatusBar(statusBar);

		menuBar->addAction(menuFile->menuAction());
		menuBar->addAction(menuAbout->menuAction());
		menuFile->addAction(actionExit);

		retranslateUi(ConfigUIClass);

		QMetaObject::connectSlotsByName(ConfigUIClass);
	} // setupUi

	void retranslateUi(QMainWindow *ConfigUIClass)
	{
		ConfigUIClass->setWindowTitle(QApplication::translate("ConfigUIClass", "ConfigUI", Q_NULLPTR));
		actionExit->setText(QApplication::translate("ConfigUIClass", "Exit", Q_NULLPTR));
		pushButton->setText(QApplication::translate("ConfigUIClass", "scan", Q_NULLPTR));
		pushButton_2->setText(QApplication::translate("ConfigUIClass", "reset", Q_NULLPTR));
		pushButton_4->setText(QApplication::translate("ConfigUIClass", "triangulate", Q_NULLPTR));
		label->setText(QApplication::translate("ConfigUIClass", "PC operations", Q_NULLPTR));
		tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("ConfigUIClass", "Tab 1", Q_NULLPTR));
		tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("ConfigUIClass", "Tab 2", Q_NULLPTR));
		menuFile->setTitle(QApplication::translate("ConfigUIClass", "File", Q_NULLPTR));
		menuAbout->setTitle(QApplication::translate("ConfigUIClass", "About", Q_NULLPTR));
	} // retranslateUi

};

namespace Ui {
	class ConfigUIClass : public Ui_ConfigUIClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGUI_H
