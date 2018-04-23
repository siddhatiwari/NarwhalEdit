#include "findwidget.h"
#include <QGridLayout>
#include "globals.h"

FindWidget::FindWidget(QWidget *parent) : QWidget(parent)
{
    QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    // Create the find menu input box
    findInput = new QLineEdit();
    findInput->setPlaceholderText("Find");
    findInput->setFont(globalFont);
    findInput->setFixedWidth(200);

    // Connects the input box text changed to mimic a find button being pressed
    connect(findInput, &QLineEdit::textChanged, [this]() {
        emit findButtonClicked(findInput->text());
    });

    layout->addStretch(1);
    layout->addWidget(findInput);

    setStyleSheet("background-color: rgb(96, 96, 96);"
                  "color: white;");
}

QLineEdit* FindWidget::getLineEdit()
{
    return findInput;
}
