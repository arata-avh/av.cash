#include "form_supernode_test.h"
#include "ui_form_supernode_test.h"

Form_SuperNode_test::Form_SuperNode_test(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form_SuperNode_test)
{
    ui->setupUi(this);

    this->setAttribute(Qt::WA_DeleteOnClose,true);
}

Form_SuperNode_test::~Form_SuperNode_test()
{
    delete ui;
}
