#include "mainwindow.h"

#include <QProcess>
#include <QAction>
#include <QToolButton>
#include <QPalette>

QCodeTextEdit* MainWindow::AddTab(const QString& Name, bool ReadOnly)
{
    QCodeTextEdit* Text = new QCodeTextEdit(Tabs);
    Text->setReadOnly(ReadOnly);
    Tabs->addTab(Text,Name);
    return Text;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("LLVM Explorer");

    Settings = new QSettings("LLVMExplorer",QSettings::IniFormat);

    Tabs = new QTabWidget(this);
    CreateToolbar();
    setCentralWidget(Tabs);

    CPPCode = AddTab("&C++");
    LLVMCode = AddTab("&LLVM IR");
    LLVMAPICode = AddTab("LLVM C++ &API");

    resize(1024,768);
}

MainWindow::~MainWindow()
{

}

void MainWindow::CreateToolbar()
{
    Toolbar = addToolBar("Actions");
    QAction* Run = Toolbar->addAction("&C++ -> LLVM API");
    Run->setShortcut(QKeySequence(Qt::Key_F1));
    connect(Run,SIGNAL(triggered()),this,SLOT(Run()));

    QAction* RunIR = Toolbar->addAction("&LLVM IR -> LLVM API");
    RunIR->setShortcut(QKeySequence(Qt::Key_F2));
    connect(RunIR,SIGNAL(triggered()),this,SLOT(RunIR()));
}

void MainWindow::Run()
{
    QString Path = "C:\\LLVM\\bin\\";

    QProcess Clang(this);
    Clang.setWorkingDirectory(Path);
    Clang.start("clang++",QStringList() << "-S" << "-x" << "c++" << "-emit-llvm" << "-" << "-o" << "-");
    Clang.waitForStarted();
    Clang.write(CPPCode->toPlainText().toLocal8Bit().constData());
    Clang.write("\n\0");
    Clang.closeWriteChannel();
    Clang.waitForFinished();
    QString ClangError(Clang.readAllStandardError());
    if(ClangError.size())
    {
        LLVMCode->setTextColor(Qt::red);
        LLVMCode->setPlainText(ClangError);
        Tabs->setCurrentWidget(LLVMCode);
        return;
    }
    else
        LLVMCode->setTextColor(Qt::black);

    QString BitCode(Clang.readAllStandardOutput());
    LLVMCode->setPlainText(BitCode);

    QProcess LLC(this);
    LLC.setWorkingDirectory(Path);
    LLC.start("llc",QStringList() << "-march=cpp" << "-" << "-o" << "-");
    LLC.waitForStarted();
    LLC.write(BitCode.toLocal8Bit().constData());
    LLC.closeWriteChannel();
    LLC.waitForFinished();
    QString LLCError(LLC.readAllStandardError());
    if(LLCError.size())
    {
        LLVMAPICode->setTextColor(Qt::red);
        LLVMAPICode->setPlainText(LLCError);
        Tabs->setCurrentWidget(LLVMAPICode);
        return;
    }
    else
        LLVMAPICode->setTextColor(Qt::black);

    LLVMAPICode->setPlainText(QString(LLC.readAllStandardOutput()));
    Tabs->setCurrentWidget(LLVMCode);
}

void MainWindow::RunIR()
{
    QString Path = "C:\\LLVM\\bin\\";

    QString BitCode = LLVMCode->toPlainText();
    QProcess LLC(this);
    LLC.setWorkingDirectory(Path);
    LLC.start("llc",QStringList() << "-march=cpp" << "-" << "-o" << "-");
    LLC.waitForStarted();
    LLC.write(BitCode.toLocal8Bit().constData());
    LLC.closeWriteChannel();
    LLC.waitForFinished();
    QString LLCError(LLC.readAllStandardError());
    if(LLCError.size())
    {
        LLVMAPICode->setTextColor(Qt::red);
        LLVMAPICode->setPlainText(LLCError);
        return;
    }
    else
        LLVMAPICode->setTextColor(Qt::black);

    LLVMAPICode->setPlainText(QString(LLC.readAllStandardOutput()));
    Tabs->setCurrentWidget(LLVMAPICode);
}

void MainWindow::Open()
{

}

void MainWindow::Save()
{

}
