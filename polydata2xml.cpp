#include <vtkPolyData.h>
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
    xml = new QXmlStreamWriter(file);
    vtkPolyData *data = poly->GetOutput();
    vtkCellArray *cells = data->GetPolys();

    return true;
}
