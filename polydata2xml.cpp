#include "polydata2xml.h"

PolyData2Xml::PolyData2Xml(QFile *file)
    :xml(file)
{
    poly = vtkPolyDataReader::New();
}

bool PolyData2Xml::WriteXml(QFile *file)
{
}
