#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

class FindWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FindWidget(QWidget *parent = nullptr);
    QLineEdit* getLineEdit();

private:
    QLineEdit *findInput;

signals:
    void findButtonClicked(QString text);

public slots:
};

#endif // FINDWIDGET_H
