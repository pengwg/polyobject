#include <QFile>

#include "polydata2xml.h"

int main(int argc, char *argv[])
{
    QString fileName;

    QFile file;
    PolyData2Xml poly2xml(&file);
    poly2xml.WriteXml(&file);

    return 0;
}

