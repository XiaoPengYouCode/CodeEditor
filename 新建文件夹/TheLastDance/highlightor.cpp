#include "highlightor.h"

#include <QFile>

HighLightor::HighLightor(QTextDocument *parent, QString fontFamily, int fontSize)
    :QSyntaxHighlighter(parent) {
    this->fontFamily = fontFamily;
    this->fontSize = fontSize;
    addNormalTextFormat();
    addNumberFormat();
    addStringFormat();
    addCommentFormat();
    addKeyWordFormat();
    addFunctionFormat();
    addClassNameFormat();
}

void HighLightor::highlightBlock(const QString &text) {
    foreach (const HighLightRule &rule, highLightRules) {
        QRegularExpression regExp(rule.pattern);
        QRegularExpressionMatch match = regExp.match(text);
        int index = match.capturedStart();
        while (index >= 0) {
            int length = match.capturedLength();
            setFormat(index, length, rule.format);
            match = regExp.match(text, index + length);
            index = match.capturedStart();
        }
    }
    addMultLineCommentFormat(text);
}

void HighLightor::addNormalTextFormat() {
    HighLightRule rule;
    rule.pattern = QRegularExpression("[a-z0-9A-Z]+");

    QTextCharFormat normalTextFormat;

    normalTextFormat.setFont(QFont(fontFamily, fontSize));
    normalTextFormat.setForeground(QColor(0, 0, 0));

    rule.format = normalTextFormat;
    highLightRules.append(rule);
}

void HighLightor::addNumberFormat() {
    HighLightRule rule;
    rule.pattern = QRegularExpression("\\b\\d+(?:\\.\\d+)?\\b");

    QTextCharFormat numberFormat;

    numberFormat.setFont(QFont(fontFamily, fontSize));
    numberFormat.setForeground(QColor(250, 80, 50));

    rule.format = numberFormat;
    highLightRules.append(rule);
}

void HighLightor::addStringFormat() {
    QTextCharFormat stringFormat;
    HighLightRule rule;
    // ''匹配字符, .* 表示任意字符重复任意次, 不包含 [^']
    rule.pattern = QRegularExpression("'[^']*'");
    rule.pattern = QRegularExpression("\"[^\"]*\"");

    stringFormat.setFont(QFont(fontFamily, fontSize));
    stringFormat.setForeground(QColor(74, 207, 90));

    rule.format = stringFormat;
    highLightRules.append(rule);
}

void HighLightor::addCommentFormat() {
    QTextCharFormat commentFormat;
    HighLightRule rule;
    // 匹配 //, $ 表示行末尾
    rule.pattern = QRegularExpression("\\/\\/.*$");
    // 匹配 /**/, s 匹配非空白字符, S 匹配空白字符, 可以换行
    // rule.pattern = QRegularExpression("\\/*[\\s\\S]*?/\\");
    // \\/\\* 匹配 /*, [^*]* 匹配不包括 * 的任意字符, \\*+保证注释中可包含 * 号
    // (?:[^/*][^*]*\\*+)*\\/保证最后的 */
//    rule.pattern = QRegularExpression("\\/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/");

    commentFormat.setFont(QFont(fontFamily, fontSize));
    commentFormat.setForeground(Qt::darkGreen);
    commentFormat.setFontItalic(true);

    rule.format = commentFormat;
    highLightRules.append(rule);
}

void HighLightor::addMultLineCommentFormat(const QString & text) {
    setCurrentBlockState(0);

    QRegularExpression commentStartRegExp("\\/\\*");
    QRegularExpression commentEndRegExp("\\/\\*");

           // 高亮格式
    QTextCharFormat multLineCommentFormat;
    multLineCommentFormat.setFont(QFont(fontFamily, fontSize));
    multLineCommentFormat.setForeground(Qt::darkGreen);
    multLineCommentFormat.setFontItalic(true);

    QRegularExpressionMatch commentStartMatch = commentStartRegExp.match(text);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartMatch.capturedStart();

    while (startIndex >= 0) {
        QRegularExpressionMatch commentEndMatch = commentEndRegExp.match(text, startIndex);
        int endIndex = commentEndMatch.capturedStart();
        int commnetLength = 0;
        if (endIndex == -1) { // 无结束位置
            setCurrentBlockState(1);
            commnetLength = text.length() - startIndex;
        } else {
            commnetLength = endIndex - startIndex + commentEndMatch.capturedLength();
        }
        setFormat(startIndex,
                  commnetLength,
                  multLineCommentFormat);
        commentStartMatch = commentStartRegExp.match(text, commnetLength + startIndex);
        startIndex = commentStartMatch.capturedStart();
    }
}

void HighLightor::addKeyWordFormat() {
    QFile file(":/config/config/keywords.txt");
    QTextStream keywordsStream(&file);

    HighLightRule rule;
    QTextCharFormat keywordsFormat;

    keywordsFormat.setFont(QFont(fontFamily, fontSize));
    keywordsFormat.setForeground(Qt::blue);
    rule.format = keywordsFormat;

    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        keywordsStream.seek(0);
        QString line;
        while (!keywordsStream.atEnd()) {
            line = keywordsStream.readLine();
            if (line != "") {
                rule.pattern = QRegularExpression("\\b"+line+"\\b");
                highLightRules.append(rule);
            }
        }
        file.close();
    }
}

void HighLightor::addFunctionFormat() {
    HighLightRule rule;

    QTextCharFormat fuctionFormat;

    fuctionFormat.setFont(QFont(fontFamily, fontSize));
    fuctionFormat.setForeground(Qt::darkCyan);

    rule.format = fuctionFormat;
    rule.pattern = QRegularExpression("\\w+\\(");
    highLightRules.append(rule);
    rule.pattern = QRegularExpression("\\)");
    highLightRules.append(rule);
}

void HighLightor::addClassNameFormat() {
    HighLightRule rule;
    rule.pattern = QRegularExpression("\\b[A-Z]+\\w*");

    QTextCharFormat classNameFormat;

    classNameFormat.setFont(QFont(fontFamily, fontSize));
    classNameFormat.setForeground(QColor(255, 128, 128));
    classNameFormat.setFontWeight(99);

    rule.format = classNameFormat;
    highLightRules.append(rule);
}

