#include <QPainter>
#include <QTextBlock>

#include "qcodetextedit.h"

QCodeTextEdit::QCodeTextEdit(QWidget* parent) : QPlainTextEdit(parent)
{
    LineNumberArea = new QLineNumberArea(this);

    connect(this,SIGNAL(blockCountChanged(int)),this,SLOT(UpdateLineNumberAreaWidth(int)));
    connect(this,SIGNAL(updateRequest(QRect,int)),this,SLOT(UpdateLineNumberArea(QRect,int)));
    connect(this,SIGNAL(cursorPositionChanged()),this,SLOT(HighlightCurrentLine()));

    UpdateLineNumberAreaWidth(0);
    HighlightCurrentLine();

    setTabStopWidth(20);
}

int QCodeTextEdit::LineNumberAreaWidth() const
{
    int digits = 1;
    int max = qMax(1,blockCount());
    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    return 3 + fontMetrics().width(QLatin1Char('9')) * digits;
}

void QCodeTextEdit::UpdateLineNumberAreaWidth(int NewBlockCount)
{
    Q_UNUSED(NewBlockCount);
    setViewportMargins(LineNumberAreaWidth(),0,0,0);
}

void QCodeTextEdit::UpdateLineNumberArea(const QRect& rect, int dy)
{
    if(dy)
        LineNumberArea->scroll(0,dy);
    else
        LineNumberArea->update(0,rect.y(),LineNumberArea->width(),rect.height());

    if(rect.contains(viewport()->rect()))
        UpdateLineNumberAreaWidth(0);
}

void QCodeTextEdit::resizeEvent(QResizeEvent* e)
{
    QPlainTextEdit::resizeEvent(e);
    QRect cr = contentsRect();
    LineNumberArea->setGeometry(QRect(cr.left(),cr.top(),LineNumberAreaWidth(),cr.height()));
}

void QCodeTextEdit::HighlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;
    if(!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(QColor(Qt::gray).lighter(150));
        selection.format.setProperty(QTextFormat::FullWidthSelection,true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}

void QCodeTextEdit::LineNumberAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(LineNumberArea);
    painter.fillRect(event->rect(),Qt::lightGray);
    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();
    while(block.isValid() && top <= event->rect().bottom())
    {
        if(block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0,top,LineNumberArea->width() - 2,fontMetrics().height(),Qt::AlignRight,number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}
