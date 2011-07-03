#ifndef POLYDATA2XML_H
#define POLYDATA2XML_H

#include <QFile>

#include <QXmlStreamWriter>
#include <vtkPolyDataReader.h>

class PolyData2Xml
{
public:
    PolyData2Xml(QFile *file);
    ~PolyData2Xml();
    bool WriteXml(QFile *file);

private:
    QFile *input;
    vtkPolyDataReader *poly;
    QXmlStreamWriter *xml;
};

#endif // POLYDATA2XML_H
