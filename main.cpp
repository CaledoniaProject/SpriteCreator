#include <QApplication>
#include "spritecreator.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SpriteCreator w;
    w.show();
    
    return a.exec();
}
