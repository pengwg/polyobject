#ifndef POLYDATA2XML_H
#define POLYDATA2XML_H

#include <QFile>

#include <QXmlStreamWriter>
#include <vtkPolyDataReader.h>
#include <vtkTriangleFilter.h>

class PolyData2Xml
{
public:
    PolyData2Xml(QFile *file);
    ~PolyData2Xml();
    bool WriteXml(QFile *file);

private:
    vtkTriangleFilter *triangleFilter;
    vtkPolyData *polyDataTri;
    vtkIdType numPoints;
    vtkIdType numTri;
};

#endif // POLYDATA2XML_H
