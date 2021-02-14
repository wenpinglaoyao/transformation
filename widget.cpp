#include "widget.h"
#include "bdialog.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    _sourceNumLabel = new QLabel(tr("请输入源数据："));
    _sourceNumLineEdit = new QLineEdit;
    QRegExp regExp("[A-Z1-9]+[A-Z0-9]*");
    _sourceNumLineEdit->setValidator(new QRegExpValidator(regExp));
    _sourceMaxLabel = new QLabel(tr("请选择源数据的进制："));
    _sourceMaxSpinBox = new QSpinBox;
    _sourceMaxSpinBox->setRange(2,36);
    _sourceMaxSpinBox->setValue(10);
    _translationBtn = new QPushButton(tr("开始转换"));
    _targetSpinLabel = new QLabel(tr("请选择目标转换数的进制："));
    _targetMaxSpinBox = new QSpinBox;
    _targetMaxSpinBox->setRange(2,36);
    connect(_translationBtn,SIGNAL(clicked()),  this,SLOT(convert()));
    _helpBtn = new QPushButton(tr("使用说明"));
    connect(_helpBtn,SIGNAL(clicked()),  this,SLOT(showHelp()));
    _resultNumText = new QLineEdit;
    _resultNumText->setEnabled(false);
    _resultNumText->setMinimumWidth(500);
    _resultNumText->setText(tr("在这个框里显示结果"));
    _resultNumText->setStyleSheet("color:blue");

    _mainLayout = new QGridLayout(this);
    _mainLayout->addWidget(_sourceNumLabel,0,0);
    _mainLayout->addWidget(_sourceNumLineEdit,0,1);
    _mainLayout->addWidget(_sourceMaxLabel,1,0);
    _mainLayout->addWidget(_sourceMaxSpinBox,1,1);
    _mainLayout->addWidget(_translationBtn,2,2);
    _mainLayout->addWidget(_targetSpinLabel, 2, 0);
    _mainLayout->addWidget(_targetMaxSpinBox,2,1);
    _mainLayout->addWidget(_helpBtn,1,2);
    _mainLayout->addWidget(_resultNumText,3,0,1,0);
}

Widget::~Widget()
{}

void Widget::convert(){
    if(compareStrNotBig()){
        if(_sourceNumLineEdit->text().length() && _sourceNumLineEdit->text().length() < MAX){
            for(int i=0; i<MAXWEI; i++){ //先清空所有的数组
                _resultArr[i] = _multiplyArr[i] = _radixArr[i] = _levelArr[i] = _tempArr[i] = _targetRadixArr[i] = false;
            }
            sourceToBinary(); //把字符串转换成了二进制数组中，保存在了_resultArr数组中


            /*********/
            clearArr(_targetLevelArr[1]);
            clearArr(_targetLevelArr[0]);
            charConvertToNum(_targetLevelArr[0], 1);
            charConvertToNum(_targetRadixArr, _targetMaxSpinBox->value());
            copyArr(_targetLevelArr[1], _targetRadixArr);
            for(int i=2; i<MAX; i++) {
                clearArr(_targetLevelArr[i]);
                constMultiply(_targetLevelArr[i-1], _targetRadixArr, _targetLevelArr[i], MAXWEI);
            }
            binaryToStr();
        }

    }else{
        QMessageBox *warning = new QMessageBox;
        warning->addButton(tr("我知道了"),QMessageBox::ActionRole);
        warning->setWindowTitle(tr("警告"));
        warning->setText(tr("字符串输入非法，请检查"));
        warning->exec();

    }

}

void Widget::binaryToStr(){
    QString str;
    int index=0;int num=0;
    for(index=0; compare1IsBigOrEqual(_resultArr,_targetLevelArr[index]); index++)
        ; //这一个循环，是找出大于结果数组的维数数组，为了以后从小一层的维数数组开始套出字符
    clearArr(_tempArr);
    for(int i=index-1; i>=0; i--){
        int num=0; //这个num就是字符的数字形式
        //这个循环是为了找出大于结果数组的累加数组
        for(charConvertToNum(_tempArr,0); compare1IsBigOrEqual(_resultArr,_tempArr); num++){
            changeAdd(_tempArr, _targetLevelArr[i], MAXWEI);
        }
        subtractFun(_tempArr,_targetLevelArr[i]);//因为上面循环最后找出的累加数组大于结果数组，所以累加数组要小一层
        subtractFun(_resultArr,_tempArr); //然后结果数组减去小一层的累加数组，得到新的结果数组，方便下一层处理
        clearArr(_tempArr);
        str.append(numToChar(num-1)); //别忘了num减一，然后交给str字符串，最后的那一句就是把str设置为目标字符串
    }
    _resultNumText->setText(str);
}

QChar Widget::numToChar(char c){
    if(c<=9){
        return c+'0';
    }else {

        return c-10+'A';
    }
}

/*****将字符转换成零一数组****/
void Widget::charConvertToNum(bool *result, char c){
    int num = 0;
    if(c < '0') num = c;
    else if(c >= 'A') num = 10 + c - 'A';
    else num = c -'0';
    int i = MAXWEI-1;
    while(num > 0){
        result[i] = (num & 1);
        num /= 2;
        i--;
    }

}

/*****将字符串转换成零一的数组*****/
void Widget::sourceToBinary(){
    charConvertToNum(_radixArr, _sourceMaxSpinBox->value());//先把进制转换成二进制数组
    copyArr(_multiplyArr, _radixArr);
    QString str = _sourceNumLineEdit->text();
    int count = str.length();
    QChar c = str[str.length()-1];
    charConvertToNum(_resultArr, c.unicode()); //先把个位字符写进结果数组中
    for(int i=1; i<count; i++){
        c = str[str.length()-1-i];
        if(c != '0'){
            charConvertToNum(_levelArr,c.unicode());
            constMultiply(_multiplyArr,_levelArr,_tempArr, MAXWEI);
            changeAdd(_resultArr, _tempArr, MAXWEI);
        }
        clearArr(_tempArr);clearArr(_levelArr);
        constMultiply(_radixArr,_multiplyArr,_tempArr,MAXWEI);
        copyArr(_multiplyArr,_tempArr);
        clearArr(_tempArr);
    }
}

/*****比较两个字符数组的大小,如果一参较大或两叁相等返回真，否则返假******/
bool Widget::compare1IsBigOrEqual(bool *arr1,bool *arr2){
    int i=0;
    for(i=0;arr1[i] == arr2[i]; i++){
        if(MAXWEI == i+1) //两字符数组完全相等，返回真
            return true;
    }
    return arr1[i]; //如果是真返回真，说明第一个参数大
}
/*****将两个字符数组相加,结果保存在第一个参数字符串里****/
void Widget::changeAdd(bool *arr1, bool *arr2, int size){
    bool flag = false;
    for(int i=size-1; i>=0; i--){
        if(arr1[i] && arr2[i]){
            arr1[i] = flag;
            flag = true;
        }
        else if(!arr1[i] && !arr2[i]){
            arr1[i] = flag;
            flag = false;
        }else{
            arr1[i] = !flag;
        }
    }
}
/****将两个字符串相加，结果保存在第3个字符串参数里***/
void Widget::constAdd(bool arr1[], bool arr2[], bool result[], int size){
    bool flag = false;
    for(int i=size-1; i>=0; i--){
        if(arr1[i] && arr2[i]){
            result[i] = flag;
            flag = true;
        }
        else if(!arr1[i] && !arr2[i]){
            result[i] = flag;
            flag = false;
        }else{
            result[i] = !flag;
        }
    }
}

/****将两个字符数组相减****/
void Widget::subtractFun(bool arr1[],bool arr2[]){
    bool flag=false;
    for(int i=MAXWEI-1; i>=0; i--){
        if(arr1[i] && !arr2[i]){
            arr1[i] = !flag;
            flag = false;
        }
        else if(!arr1[i] && arr2[i]){
            arr1[i] = !flag;
            flag = true;
        }
        else{
            arr1[i] = flag;
        }
    }
}

void Widget::clearArr(bool arr[]){
    for(int i=0; i<MAXWEI; i++)
        arr[i] = false;
}
void Widget::copyArr(bool target[], bool source[]){
    for(int i=0; i<MAXWEI; i++)
        target[i] = source[i];
}

void Widget::constMultiply(bool arr1[], bool arr2[], bool target[], int size){
    for(int i=0; i<size; i++){
        if(arr2[size-i-1]){
            changeAdd(&target[MAXWEI/2-i],&arr1[MAXWEI/2],MAXWEI/2);
        }
    }
}
void Widget::changeMultiply(bool result[], bool mode[], int size){

}

bool Widget::compareStrNotBig(){
    QString str = _sourceNumLineEdit->text();
    for(int count=str.length(); count>0; count--){
        if(numToChar(_sourceMaxSpinBox->value()) <= str[count-1].unicode())
            return false;
    }
    return true;
}

void Widget::showHelp(){
    QMessageBox *box = new QMessageBox;
    box->setWindowTitle(tr("萌萌对你说："));
    box->setText(tr("只能输入数字和大写字母，而字符A表示十进制的10，B表示11，C表示12，依次类推直到Z。。。暂不支持小数点，需要再补充。最大数据支持到36^36，需要再扩充"));
    box->addButton(tr("我知道了"),QMessageBox::ActionRole);
    box->exec();
}
