#ifndef SPRITECREATOR_H
#define SPRITECREATOR_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include <QDir>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QColormap>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QStandardItemModel>
#include <QUrl>
#include <QStandardItem>
#include <QSet>

namespace Ui {
    class SpriteCreator;
}

class SpriteCreator : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SpriteCreator(QMainWindow *parent = 0);
    ~SpriteCreator();

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    
private slots:
    void on_pushButton_clicked();

    void on_actionClear_triggered();

    void on_actionExit_X_triggered();

private:
    Ui::SpriteCreator *ui;
    QString m_saveDir;
    QSet<QString> existingFiles;
    QStandardItemModel *m_fileModel;

    void addItem (const QString & fileName);
};

#endif // SPRITECREATOR_H
