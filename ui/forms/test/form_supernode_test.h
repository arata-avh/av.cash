#ifndef FORM_SUPERNODE_TEST_H
#define FORM_SUPERNODE_TEST_H

#include <QWidget>

namespace Ui {
class Form_SuperNode_test;
}

class Form_SuperNode_test : public QWidget
{
    Q_OBJECT

public:
    explicit Form_SuperNode_test(QWidget *parent = 0);
    ~Form_SuperNode_test();

private slots:
    void on_btn_getblockchaininfo_clicked();

    void on_btn_generate_clicked();

    void on_btn_key_test_clicked();

private:
    Ui::Form_SuperNode_test *ui;
};

#endif // FORM_SUPERNODE_TEST_H
