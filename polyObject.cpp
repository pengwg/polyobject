#include <QFile>
#include <QTextStream>

#include "polydata2xml.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3) {
        qWarning("Usage: argv[0] <vtkPolyData File> [output]");
        return 0;
    }

    QString inputName(argv[1]);
    QFile input(inputName);

    if (!input.exists()) {
        qWarning("%s does not exist!", argv[1]);
        return 0;
    }

    QString outputName;

    if (argc == 3)
        outputName = QString(argv[2]);
    else {
        outputName = inputName + ".xml";
    }

    QFile output(outputName);

    PolyData2Xml poly2xml(&input);
    if (poly2xml.WriteXml(&output)) {
        QTextStream out(stdout);
        out << outputName << " successfully generated." << endl;
    }

    return 0;
}

