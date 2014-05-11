#ifndef QCODETEXTEDIT_H
#define QCODETEXTEDIT_H

#include <QPlainTextEdit>
#include <QSyntaxHighlighter>

class CPPSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    public:
        CPPSyntaxHighlighter(QTextDocument* Parent = 0);

    protected:
        void highlightBlock(const QString& text);

    private:
        struct HighlightingRule
        {
            QRegExp Pattern;
            QTextCharFormat Format;
        };

        QVector<HighlightingRule> Rules;
        QRegExp CommentStartExpression;
        QRegExp CommentEndExpression;

        QTextCharFormat KeywordFormat;
        QTextCharFormat ClassFormat;
        QTextCharFormat SingleLineCommentFormat;
        QTextCharFormat MultiLineCommentFormat;
        QTextCharFormat QuotationFormat;
        QTextCharFormat FunctionFormat;
};

class LLVMSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

    public:
        LLVMSyntaxHighlighter(QTextDocument* Parent = 0);

    protected:
        void highlightBlock(const QString& text);

    private:
        struct HighlightingRule
        {
            QRegExp Pattern;
            QTextCharFormat Format;
        };

        QVector<HighlightingRule> Rules;
        QRegExp CommentStartExpression;
        QRegExp CommentEndExpression;

        QTextCharFormat KeywordFormat;
        QTextCharFormat ClassFormat;
        QTextCharFormat SingleLineCommentFormat;
        QTextCharFormat MultiLineCommentFormat;
        QTextCharFormat QuotationFormat;
        QTextCharFormat FunctionFormat;
};

class QCodeTextEdit : public QPlainTextEdit
{
        Q_OBJECT
    public:
        explicit QCodeTextEdit(QWidget* parent = nullptr);
        void LineNumberAreaPaintEvent(QPaintEvent* event);
        int LineNumberAreaWidth() const;

        inline QColor setTextColor(const QColor& color)
        {
            QPalette p = palette();
            QColor old = p.color(QPalette::Text);
            p.setColor(QPalette::Text,color);
            setPalette(p);
            return old;
        }

        inline QColor setTextBackgroundColor(const QColor& color)
        {
            QPalette p = palette();
            QColor old = p.color(QPalette::Background);
            p.setColor(QPalette::Text,color);
            setPalette(p);
            return old;
        }

    protected:
        void resizeEvent(QResizeEvent *event);

    private slots:
        void UpdateLineNumberAreaWidth(int NewBlockCount);
        void HighlightCurrentLine();
        void UpdateLineNumberArea(const QRect&, int);

    private:
        QWidget* LineNumberArea;
};

class QLineNumberArea : public QWidget
{
    public:
        QLineNumberArea(QCodeTextEdit* editor) : QWidget(editor)
        {
            Editor = editor;
        }

        QSize sizeHint() const
        {
            return QSize(Editor->LineNumberAreaWidth(),0);
        }

    protected:
        void paintEvent(QPaintEvent* event)
        {
            Editor->LineNumberAreaPaintEvent(event);
        }

    private:
        QCodeTextEdit* Editor;
};

#endif // QCODETEXTEDIT_H
