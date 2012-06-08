#include "spritecreator.h"
#include "ui_spritecreator.h"
#include "csshighlighter.h"

#define SPRITE_TEMPLATE \
    ".sprite_%1 { \n" \
    "   background: url('images/sprite_autogen.png') no-repeat %2px -%3px; \n" \
    "   width: %4px; \n" \
    "   height: %5px; \n" \
    "}; \n"

SpriteCreator::SpriteCreator(QMainWindow *parent) :
    QMainWindow(parent),
    ui(new Ui::SpriteCreator),
    m_fileModel (new QStandardItemModel(this))
{
    ui->setupUi(this);
    ui->listView->setModel(m_fileModel);
    new CssHighlighter(ui->plainTextEdit->document());
}

SpriteCreator::~SpriteCreator()
{
    delete ui;
}

void SpriteCreator::addItem(const QString &fileName)
{
    if ( existingFiles.contains(fileName) )
    {
        return;
    }
    else
    {
        existingFiles.insert(fileName);
    }

    const QString & baseName = fileName.right(fileName.length() - 1 - fileName.lastIndexOf(QDir::separator()));

    QStandardItem *item = new QStandardItem (baseName);
    item->setData(fileName , Qt::UserRole);

    m_fileModel->appendRow(item);
}

void SpriteCreator::dropEvent(QDropEvent *event)
{
    const QList<QUrl> & urls = event->mimeData()->urls();
    foreach (const QUrl & url , urls)
    {
        addItem(url.toLocalFile());
    }
}

void SpriteCreator::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat("text/uri-list"))
        event->acceptProposedAction();
}

void SpriteCreator::on_pushButton_clicked()
{
    const QString & outputFileName = QFileDialog::getSaveFileName(this , tr("Save sprite image to")
                                                                  , m_saveDir , tr("Images (*.png *.xpm *.jpg);;All Files (*)"));

    if ( outputFileName.isEmpty() )
    {
        return;
    }

    ///
    m_saveDir = outputFileName.left(outputFileName.lastIndexOf(QDir::separator()));

    int x = 0;
    int y = 0;
    static int padding = 5; // padding between each image (vertical)

    ui->plainTextEdit->clear();

    QList<QImage*> images;
    QStringList fileNames;
    for ( int i = 0 ; i < m_fileModel->rowCount() ; ++ i )
    {
        const QString & absolutePath = m_fileModel->data(m_fileModel->index(i , 0) , Qt::UserRole).toString();
        const QString & basename = m_fileModel->data(m_fileModel->index(i , 0)).toString();

        QImage *image = new QImage (absolutePath);
        if ( ! image->isNull() )
        {
            fileNames.push_back(basename);
            images.push_back(image);

            y = y + image->height() + padding;

            if ( image->width() > x )
            {
                x = image->width();
            }
        }
        else
        {
            delete image;
        }
    }

    ///
    if ( x == 0 )
    {
        return;
    }

    x += padding * images.size();

    QImage output = QImage (x , y , QImage::Format_ARGB32);
    output.fill(Qt::transparent);
    QPainter painter (&output);
//    QString outputHtml;
//    outputHtml.resize(100 * fileNames.size());

    y = 0;
    foreach (const QImage *img , images)
    {
        ///
        const QString & fileName = fileNames.first();
        fileNames.pop_front();

        static QRegExp characters ("^([a-zA-Z0-9]+)");
        if ( characters.indexIn(fileName) == -1 )
            continue;

        ui->plainTextEdit->appendPlainText(QString(SPRITE_TEMPLATE)
                                           .arg(characters.cap(1))
                                           .arg(0)
                                           .arg(y)
                                           .arg(img->width())
                                           .arg(img->height()));

//        outputHtml.append("<div class=\"%1\"></div>").arg(haracters.cap(1));

        ///
        painter.drawImage(0 , y , *img);
        y = y + img->height() + padding;
    }

    if ( ! output.save(outputFileName) )
    {
        QMessageBox::warning(this , tr("Error saving file") , "can't write file: " + outputFileName ,
                             QMessageBox::Ok);
    }
}

void SpriteCreator::on_actionClear_triggered()
{
    existingFiles.clear();
    m_fileModel->clear();
}

void SpriteCreator::on_actionExit_X_triggered()
{
    close();
}
