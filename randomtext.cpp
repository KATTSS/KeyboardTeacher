#include "randomtext.h"

RandomText::RandomText(const QString &filePath){
    file=filePath;
}

QString RandomText::getRandomText(const int &index)
{
    //qDebug() << "in getting text. before";
    QFile loadedFile(file);
    QString randomTextString;
    QStringList list;
    if (loadedFile.open(QIODevice::ReadOnly| QIODevice::Text)) {
        QTextStream in(&loadedFile);
        //qDebug() << "in getRandomText in randomText";
        for (int i = 0; i < 20*index; ++i) {
            if (in.readLine().isNull()) {
                return "" ;
            }
        }
        for (int i = 0; i < 20; ++i) {
            QString line = in.readLine();
            if (line.isNull()) {
                break;
            }
            list.append(line);
        }
        loadedFile.close();
    }
    if(list.isEmpty()) return "";
    quint32 array[15];
    QRandomGenerator::global()->fillRange(array);
    for (int i=0; i<15; ++i) {
        randomTextString.append(list[array[i] % list.size()]);
        randomTextString.append(" ");
    }
    //qDebug() << randomTextString;
    return randomTextString.trimmed();
}
