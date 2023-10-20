#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

  private slots:
    void on_newFile_triggered();

    void on_openFile_triggered();

    void on_saveAs_triggered();

    void on_saveFile_triggered();

    void on_undo_triggered();

    void on_redo_triggered();

    void on_bold_triggered(bool bBold);

    void on_underLine_triggered(bool bUnderLine);

    void on_italic_triggered(bool bItalic);

    void on_copy_triggered();

    void on_paste_triggered();

    void on_cut_triggered();

    void on_about_triggered();

    void on_font_triggered();

    void on_clearHistory_triggered();

  private:
    Ui::MainWindow *ui;
    QString currentFile;
    void initMenu();
    void on_recentFile_triggered();
};

QList<QString> getHistory();
void saveHistory(QString path);
#endif // MAINWINDOW_H
