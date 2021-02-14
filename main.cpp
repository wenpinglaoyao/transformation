#include "widget.h"
#include <QApplication>
#include <QWidget>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("数字进制转换器");
    w.show();

    return a.exec();
}
