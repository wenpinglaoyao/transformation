#ifndef BDIALOG_H
#define BDIALOG_H
#include <QDialog>
#include <QLabel>

class BDialog : public QDialog
{
public:
    BDialog(QWidget *parent=0);
    QLabel *_label;
};

#endif // BDIALOG_H
