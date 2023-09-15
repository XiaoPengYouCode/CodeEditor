#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QWidget>

namespace Ui {
class MyTextEdit;
}

class MyTextEdit : public QWidget
{
    Q_OBJECT

        public:
                 explicit MyTextEdit(QWidget *parent = nullptr);
    ~MyTextEdit();

  private slots:
    void textEditHorizontalScrollBarChanged();
    void ScrollBarChanged();
    void onTextChanged();
    void textBrowserVerticalScrollBarChanged();
    void textEditVerticalScrollBarChanged();
  private:
    Ui::MyTextEdit *ui;
    void initConnect();
    void initfont();
    void initHighLight();
};

#endif // MYTEXTEDIT_H
