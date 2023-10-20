#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "mytextedit.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QFontDialog>
#include <QRegularExpression>
#include <QSettings>
#include <QList>
#include <QSet>

static QRegularExpression rx("^\\d+\\. ");

QSettings* settings;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
      , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 文本框居中显示
    this->setCentralWidget(ui->tabWidget);
    if (settings == nullptr)
        settings = new QSettings("settings.ini", QSettings::IniFormat);

    initMenu();
    getHistory();
}

MainWindow::~MainWindow() {
    delete ui;
}


void MainWindow::initMenu() {
    // 获取 recent Menu 下的所有 QAction 子对象并删除
    QMenu* recent = this->findChild<QMenu*>("recent");
    QObjectList chlists = recent->children();
    foreach (QObject* ch, chlists) {
        QAction* action = qobject_cast<QAction*>(ch);
        recent->removeAction(action);
    }
    // 获取历史记录
    // 重建所有子 QAction 对象
    QList<QString> lists = getHistory();

    for (int i = (lists.size() - 1); i >= 0; i--) {
        recent->addAction(QString::number(lists.size() - i) + ". " + lists[i], this, &MainWindow::on_recentFile_triggered);
    }
    if (!lists.isEmpty()) {
        recent->addAction("清除历史记录", this, &MainWindow::on_clearHistory_triggered);
    }
}


QList<QString> getHistory() {
    int size = settings->beginReadArray("history");

    QList<QString> lists;
    for (int i = 0; i < size; i++) {
        settings->setArrayIndex(i);
        QString path = settings->value("path").toString();
        lists.append(path);
    }
    settings->endArray();
    return lists;
}


void saveHistory(QString path) {
    // 读取历史并去重
    QList<QString> history = getHistory();
    foreach (QString historyPath, history) {
        if (path == historyPath)
            history.removeOne(path);
    }
    history.append(path);

    // 打开配置文件, 开始重新写入数组 history
    settings->beginWriteArray("history");
    for (int i = 0; i < history.size(); i++) {
        settings->setArrayIndex(i);
        settings->setValue("path", history[i]);
    }
    settings->endArray();
}


void MainWindow::on_newFile_triggered() {
    MyTextEdit* myTextEdit = new MyTextEdit(this);
    ui->tabWidget->addTab(myTextEdit, "New Tab");
//    currentFile.clear();
//    ui->textEdit->clear();
}


void MainWindow::on_openFile_triggered() {
    QString fileName = QFileDialog::getOpenFileName(this, "打开文件");
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法打开此文件:" + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(currentFile);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    saveHistory(currentFile);
    initMenu();
}

void MainWindow::on_recentFile_triggered() {
    QAction* action = (QAction*)sender();
    QString fileName = action->text();
    fileName.remove(rx);

    qDebug() << fileName;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法打开此文件:" + file.errorString());
        return;
    }
    currentFile = fileName;
    setWindowTitle(currentFile);
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->setText(text);
    file.close();
    saveHistory(currentFile);

    initMenu();
}


void MainWindow::on_saveFile_triggered() {
    QString fileName;
    if (currentFile.isEmpty()) {
        fileName = QFileDialog::getSaveFileName(this, "保存文件");
        currentFile = fileName;
    } else {
        fileName = currentFile;
    }

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法写入此文件:" + file.errorString());
        return;
    }
    setWindowTitle(currentFile);
    QTextStream out(&file);
    QString text = ui->textEdit->toHtml();
    out << text;
    file.close();
    saveHistory(currentFile);
    initMenu();
}


void MainWindow::on_saveAs_triggered() {
    QString fileName = QFileDialog::getSaveFileName(this, "另存为");
    currentFile = fileName;
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "警告", "无法写入此文件:" + file.errorString());
        return;
    }
    setWindowTitle(currentFile);
    QTextStream out(&file);
    QString text = ui->textEdit->toHtml();
    out << text;
    file.close();
    saveHistory(currentFile);
    initMenu();
}


void MainWindow::on_undo_triggered() {
    ui->textEdit->undo();
}


void MainWindow::on_redo_triggered() {
    ui->textEdit->redo();
}


void MainWindow::on_bold_triggered(bool bBold) {
    if (bBold)
        ui->textEdit->setFontWeight(QFont::Bold);
    else
        ui->textEdit->setFontWeight(QFont::Normal);
}


void MainWindow::on_underLine_triggered(bool bUnderLine) {
    ui->textEdit->setFontUnderline(bUnderLine);
}


void MainWindow::on_italic_triggered(bool bItalic) {
    ui->textEdit->setFontItalic(bItalic);
}


void MainWindow::on_copy_triggered() {
    ui->textEdit->copy();
}


void MainWindow::on_paste_triggered() {
    ui->textEdit->paste();
}


void MainWindow::on_cut_triggered() {
    ui->textEdit->cut();
}


void MainWindow::on_about_triggered() {
    QMessageBox::about(this, "关于", "这是我的一个个人项目, 详情请看我的 github 主页");
}


void MainWindow::on_font_triggered() {
    bool bFontSelected;
    QFont font = QFontDialog::getFont(&bFontSelected, this);
    if (bFontSelected) {
        ui->textEdit->setFont(font);
    }
}

void MainWindow::on_clearHistory_triggered() {
    settings->remove("history");
    initMenu();
}

