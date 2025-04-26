#ifndef RANDOMTEXT_H
#define RANDOMTEXT_H

#include <QFile>
#include <QFileSelector>
#include <QFileDialog>
#include <QTextStream>
#include <QRandomGenerator>

#include <QDebug>
class RandomText
{
public:
    RandomText(const QString &filePath);
    //QString getPath() {return file;}
    QString getRandomText(const int &index);

private :
    QString file;
};

#endif // RANDOMTEXT_H
