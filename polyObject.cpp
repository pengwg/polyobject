#include <QFile>
#include <QTextStream>

#include "polydata2xml.h"

int main(int argc, char *argv[])
{
    if (argc < 2 || argc > 3) {
        qWarning("Usage: vtk2xml <vtkPolyData File> [output]");
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
    if (!output.open(QFile::WriteOnly | QFile::Text))
        qWarning("Cannot write to file %s", outputName.toAscii().constData());

    PolyData2Xml poly2xml(&input);

    QTextStream out(stdout);
    out << "Export to " << outputName << " ...";
    out.flush();
    if (poly2xml.WriteXml(&output)) {
        out << "\33[32m" << " done!" << "\33[0m" << endl;
    }

    output.close();
    return 0;
}

