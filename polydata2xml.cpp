#include "polydata2xml.h"

PolyData2Xml::PolyData2Xml(QFile *file)
{
    poly = vtkPolyDataReader::New();
    poly->SetFileName(file->fileName().toAscii().constData());

    if (!poly->IsFilePolyData())
        qFatal("Not valid PolyData file!");
}

bool PolyData2Xml::WriteXml(QFile *file)
{
    qWarning("%s", file->fileName().toAscii().constData());
    xml = new QXmlStreamWriter(file);
    return true;
}
