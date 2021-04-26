#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QFileSystemModel>
#include <QStringListModel>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_calculatorBottom_clicked();

    void on_btnOpen_clicked();

private:
    Ui::Widget *ui;
    QFileSystemModel *model;
    QStringListModel *theModel;
};
#endif // WIDGET_H
