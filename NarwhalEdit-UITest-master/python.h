#ifndef PYTHON_H
#define PYTHON_H
#include "language.h"

class Python : public Language
{
public:
    Python();
    QString name = "Python";
};

#endif // PYTHON_H
