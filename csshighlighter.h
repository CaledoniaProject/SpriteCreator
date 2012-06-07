#ifndef CSSHIGHLIGHTER_H
#define CSSHIGHLIGHTER_H

#include <QSyntaxHighlighter>

class CssHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit CssHighlighter(QTextDocument *document);

protected:
    void highlightBlock(const QString&);
    void highlight(const QString&, int, int, int/*State*/);

private:
    enum State { Selector, Property, Value, Pseudo, Pseudo1, Pseudo2, Quote,
                 MaybeComment, Comment, MaybeCommentEnd };

};

#endif // CSSHIGHLIGHTER_H
