// #ifndef MAINWINDOW_H
// #define MAINWINDOW_H

// #include <QMainWindow>

// class MainWindow : public QMainWindow
// {
//     Q_OBJECT

// public:
//     MainWindow(QWidget *parent = nullptr);
//     ~MainWindow();
// };
// #endif // MAINWINDOW_H
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include "codeeditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SyntaxHighlighter; // Предварительное объявление класса подсветки синтаксиса

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newFile();      // Создать новый файл
    void openFile();     // Открыть файл
    bool saveFile();     // Сохранить файл
    bool saveFileAs();   // Сохранить файл как...
    void exit_app();      // Выход из приложения
    void about();        // О программе
    void is_document_modified(); // Документ был изменен

    void updateStatusBar();

private:
    void createActions();   // Создание действий
    void createMenus();     // Создание меню
    void createToolBars();  // Создание панели инструментов
    void createStatusBar(); // Создание строки состояния
    bool maybeSave();       // Проверка, нужно ли сохранить
    void loadFile(const QString &fileName); // Загрузка файла
    bool saveFileToDisk(const QString &fileName); // Сохранение файла на диск
    void setCurrentFile(const QString &fileName); // Установка текущего файла

    //QTextEdit *textEdit;
    QString currentFile;
    bool isUntitled;

    SyntaxHighlighter *highlighter;

    // Меню и действия
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *cutAct;
    QAction *copyAct;
    QAction *pasteAct;
    QAction *aboutAct;

    QLabel *statusInfoLabel;

    CodeEditor *codeEditor;
};
#endif // MAINWINDOW_H
