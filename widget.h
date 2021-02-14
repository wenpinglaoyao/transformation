#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QGridLayout>
#include <QSpinBox>
#include <QMessageBox>


#define MAXWEI 400 //二进制数据的最多位数
#define MAX 200 //输入源数据可以输入的最多位数
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

private:
    bool _resultArr[MAXWEI];
    bool _multiplyArr[MAXWEI];
    bool _tempArr[MAXWEI];
    bool _radixArr[MAXWEI];
    bool _levelArr[MAXWEI];
    bool _targetRadixArr[MAXWEI];
    bool _targetLevelArr[MAX][MAXWEI];

    QLabel *_sourceNumLabel, *_sourceMaxLabel, *_targetSpinLabel;
    QLineEdit *_sourceNumLineEdit;
    QSpinBox *_sourceMaxSpinBox, *_targetMaxSpinBox;
    QPushButton *_translationBtn;
    QPushButton *_helpBtn;
    QLineEdit *_resultNumText;
    QGridLayout *_mainLayout;

    bool compare1IsBigOrEqual(bool *,bool *);
    void changeAdd(bool*, bool*, int);
    void constAdd(bool [],bool [], bool[], int);
    void subtractFun(bool*, bool*);
    void charConvertToNum(bool *, char c);
    void sourceToBinary();
    void clearArr(bool *);
    void copyArr(bool*,bool*);
    void constMultiply(bool[], bool[], bool[], int size);
    void changeMultiply(bool [], bool[], int size);
    void binaryToStr();
    QChar numToChar(char c);
    bool compareStrNotBig();

private slots:
    void convert();
    void showHelp();

};

#endif // WIDGET_H
