#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QTabWidget>
#include <QPlainTextEdit>
#include <QSettings>
#include "qcodetextedit.h"

class MainWindow : public QMainWindow
{
        Q_OBJECT
    private:
        QSettings* Settings;
        QTabWidget* Tabs;
        QToolBar* Toolbar;
        QCodeTextEdit* CPPCode;
        QCodeTextEdit* LLVMCode;
        QCodeTextEdit* LLVMAPICode;

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

        QCodeTextEdit* AddTab(const QString& Name, bool ReadOnly = false);

    private:
        void CreateToolbar();

    private slots:
        void Run();
        void RunIR();
        void Open();
        void Save();
};

#endif // MAINWINDOW_H
