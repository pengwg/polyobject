#ifndef POLYDATA2XML_H
#define POLYDATA2XML_H

#include <QFile>

#include <QXmlStreamWriter>
#include <vtkPolyDataReader.h>

class PolyData2Xml
{
public:
    PolyData2Xml(QFile *file);
    bool WriteXml(QFile *file);

private:
    vtkPolyDataReader *poly;
    QXmlStreamWriter xml;
};

#endif // POLYDATA2XML_H
