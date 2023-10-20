#include "highlightor.h"
#include "mytextedit.h"
#include "ui_mytextedit.h"

#include <QDebug>

MyTextEdit::MyTextEdit(QWidget *parent) :
      QWidget(parent),
      ui(new Ui::MyTextEdit)
{
    ui->setupUi(this);
    // 绑定滚动条
    initConnect();
    // 修改字体
    initfont();

    initHighLight();
}


void MyTextEdit::initConnect() {
    // 行数检测
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));

    // 横向滚动条绑定
    connect(ui->textEdit->horizontalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(textEditHorizontalScrollBarChanged()));
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)),
            this, SLOT(ScrollBarChanged()));

    // 纵向滚动条绑定
    connect(ui->textEdit->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(textBrowserVerticalScrollBarChanged()));
    connect(ui->textBrowser->verticalScrollBar(), SIGNAL(valueChanged(int)),
            this, SLOT(textEditVerticalScrollBarChanged()));
}


void MyTextEdit::initfont() {
    ui->textEdit->setFont(QFont("Consolas", 14));
    ui->textBrowser->setFont(QFont("Consolas", 14));
    ui->textEdit->setTabStopDistance(40);
}

void MyTextEdit::initHighLight() {
    new HighLightor(ui->textEdit->document());
}


void MyTextEdit::textEditHorizontalScrollBarChanged() {
    ui->horizontalScrollBar->setMinimum(ui->textEdit->horizontalScrollBar()->minimum());
    ui->horizontalScrollBar->setMaximum(ui->textEdit->horizontalScrollBar()->maximum());
    ui->horizontalScrollBar->setPageStep(ui->textEdit->horizontalScrollBar()->pageStep());
    ui->horizontalScrollBar->setValue(ui->textEdit->horizontalScrollBar()->value());
}

void MyTextEdit::ScrollBarChanged() {
    ui->textEdit->horizontalScrollBar()->setValue(ui->horizontalScrollBar->value());
}

void MyTextEdit::onTextChanged() {
    int iTextEditLineCount = ui->textEdit->document()->lineCount();
//    int iTextBrowserLineCount = ui->textBrowser->document()->lineCount();

    QString text = ui->textBrowser->toPlainText();
    int iTextBrowserLineCount = text.trimmed().split('\n').length();
    if (iTextBrowserLineCount > iTextEditLineCount) {
        for (int i = iTextBrowserLineCount; i > iTextEditLineCount; i--) {
            text.chop((QString::number(i) + '\n').length());
        }
    } else if ((iTextEditLineCount == 1) && (text.length() < 1)) {
                text += "1\n";
    } else if (iTextBrowserLineCount < iTextEditLineCount) {
        for (int i = iTextBrowserLineCount; i < iTextEditLineCount; i++) {
            text += QString::number(i + 1) + '\n';
        }
    }

    ui->textBrowser->setMaximumWidth(25 + QString::number(iTextEditLineCount).length() * 8);
    ui->textBrowser->setText(text);
}

void MyTextEdit::textBrowserVerticalScrollBarChanged() {
    ui->textBrowser->verticalScrollBar()->setValue(ui->textEdit->verticalScrollBar()->value());
}

void MyTextEdit::textEditVerticalScrollBarChanged() {
    ui->textEdit->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->value());
}

MyTextEdit::~MyTextEdit() {
    delete ui;
}
