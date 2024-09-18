#include "mainwindow.h"
#include "qapplication.h"
#include "syntaxhighlighter.h"

#include <QAction>
#include <QMenuBar>
#include <QToolBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QStatusBar>
#include <QCloseEvent>
#include <QTextStream>
#include <QSettings>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), textEdit(new QTextEdit), isUntitled(true)
{
    setCentralWidget(textEdit);

    createActions();
    createMenus();
    createToolBars();
    createStatusBar();

    highlighter = new SyntaxHighlighter(textEdit->document()); // Инициализация подсветки синтаксиса

    connect(textEdit->document(), &QTextDocument::contentsChanged,
            this, &MainWindow::is_document_modified);

    // Подключение сигнала изменения текста к слоту обновления статус бара
    connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::updateStatusBar);

    // Создание QLabel и добавление в статус бар
    statusInfoLabel = new QLabel(this);
    statusBar()->addPermanentWidget(statusInfoLabel);

    // Инициализация статус бара
    updateStatusBar();


    setCurrentFile(QString());
    setWindowIcon(QIcon("./media/icon.png"));
    resize(800, 600);
}

MainWindow::~MainWindow()
{
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile(QString());
    }
    updateStatusBar();
}

void MainWindow::openFile()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }

}

bool MainWindow::saveFile()
{
    if (isUntitled) {
        return saveFileAs();
    } else {
        return saveFileToDisk(currentFile);
    }
}

bool MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;

    return saveFileToDisk(fileName);
}

void MainWindow::exit_app()
{
    close();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("шо цэ таке"),
                       tr("<b>Текстовый редактор</b> с подсветкой синтаксиса."));
}

void MainWindow::is_document_modified()
{
    setWindowModified(textEdit->document()->isModified());
}

void MainWindow::updateStatusBar()
{
    QString text = textEdit->toPlainText();

    // Удаляем все пробелы для подсчета символов без пробелов
    QString textWithoutSpaces = text;
    textWithoutSpaces.remove(QRegularExpression("\\s+"));

    // Подсчет количества слов
    QStringList wordList = text.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    int wordCount = wordList.count();

    // Подсчет количества символов без пробелов
    int charCount = textWithoutSpaces.length();

    // Обновление метки в статус баре
    statusInfoLabel->setText(tr("Слов(без пробелов): %1 | Символов: %2").arg(wordCount).arg(charCount));
}

void MainWindow::createActions()
{
    newAct = new QAction(tr("&Новый"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Создать новый документ"));
    connect(newAct, &QAction::triggered, this, &MainWindow::newFile);

    openAct = new QAction(tr("&Открыть..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Открыть существующий файл"));
    connect(openAct, &QAction::triggered, this, &MainWindow::openFile);

    saveAct = new QAction(tr("&Сохранить"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Сохранить документ"));
    connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);

    saveAsAct = new QAction(tr("Сохранить &как..."), this);
    saveAsAct->setStatusTip(tr("Сохранить документ под новым именем"));
    connect(saveAsAct, &QAction::triggered, this, &MainWindow::saveFileAs);

    exitAct = new QAction(tr("&Выход"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Выйти из приложения"));
    connect(exitAct, &QAction::triggered, this, &MainWindow::exit_app);

    cutAct = new QAction(tr("&Вырезать"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    cutAct->setStatusTip(tr("Вырезать выделенный текст"));
    connect(cutAct, &QAction::triggered, textEdit, &QTextEdit::cut);

    copyAct = new QAction(tr("&Копировать"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    copyAct->setStatusTip(tr("Копировать выделенный текст"));
    connect(copyAct, &QAction::triggered, textEdit, &QTextEdit::copy);

    pasteAct = new QAction(tr("&Вставить"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    pasteAct->setStatusTip(tr("Вставить из буфера обмена"));
    connect(pasteAct, &QAction::triggered, textEdit, &QTextEdit::paste);

    aboutAct = new QAction(tr("&О программе"), this);
    aboutAct->setStatusTip(tr("Информация о приложении"));
    connect(aboutAct, &QAction::triggered, this, &MainWindow::about);

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    connect(textEdit, &QTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
    connect(textEdit, &QTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&Файл"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Правка"));
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);

    helpMenu = menuBar()->addMenu(tr("&Помощь"));
    helpMenu->addAction(aboutAct);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("Файл"));
    fileToolBar->addAction(newAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Готово"));
}


bool MainWindow::maybeSave()
{
    if (textEdit->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Текстовый редактор"),
                                   tr("Документ был изменен.\n"
                                      "Вы хотите сохранить изменения?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return saveFile();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::loadFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Текстовый редактор"),
                             tr("Не удалось открыть файл %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }

    QTextStream in(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    textEdit->setPlainText(in.readAll());
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Файл загружен"), 2000);
    updateStatusBar();
}

bool MainWindow::saveFileToDisk(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Текстовый редактор"),
                             tr("Не удалось сохранить файл %1:\n%2.")
                                 .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toPlainText();
    QApplication::restoreOverrideCursor();

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("Файл сохранен"), 2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
    isUntitled = currentFile.isEmpty();

    textEdit->document()->setModified(false);
    setWindowModified(false);

    QString shownName = isUntitled ? tr("Безымянный.txt") : QFileInfo(currentFile).fileName();
    setWindowFilePath(shownName);
}

