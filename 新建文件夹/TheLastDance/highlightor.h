#ifndef HIGHLIGHTOR_H
#define HIGHLIGHTOR_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

class HighLightor : public QSyntaxHighlighter
{
  public:
    explicit HighLightor(QTextDocument* parent = nullptr, QString fontFamily="Consolas", int fontSize = 14);
  protected:
    void highlightBlock(const QString &text);
  private:
    QString fontFamily;
    int fontSize;

    struct HighLightRule {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighLightRule> highLightRules;
    void addNormalTextFormat();
    void addNumberFormat();
    void addStringFormat();
    void addCommentFormat();
    void addMultLineCommentFormat(const QString & text);
    void addKeyWordFormat();
    void addFunctionFormat();
    void addClassNameFormat();
};

#endif // HIGHLIGHTOR_H
