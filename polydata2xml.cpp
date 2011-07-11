#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkTriangleFilter.h>

#include <QTextStream>

#include "polydata2xml.h"

PolyData2Xml::PolyData2Xml(QFile *file)
{
    poly = vtkPolyDataReader::New();
    poly->SetFileName(file->fileName().toAscii().constData());

    if (!poly->IsFilePolyData())
        qFatal("Not valid PolyData file!");
}

PolyData2Xml::~PolyData2Xml()
{
    poly->Delete();
    delete xml;
}

bool PolyData2Xml::WriteXml(QFile *file)
{
    QTextStream out(stdout);

    vtkPolyData *polyData = poly->GetOutput();
    poly->Update();

    vtkIdType numPoints = polyData->GetNumberOfPoints();
    out << "Input -- " << numPoints << " points, ";
    vtkIdType numStrips = polyData->GetNumberOfStrips();
    out << numStrips << " triangle strips." << endl;

    // Convert strips to triangels
    vtkTriangleFilter *triangleFilter = vtkTriangleFilter::New();
    triangleFilter->SetInput(polyData);
    vtkPolyData *polyDataTri = triangleFilter->GetOutput();
    triangleFilter->Update();

    numPoints = polyDataTri->GetNumberOfPoints();
    out << "Convert to -- " << numPoints << " points, ";
    vtkIdType numTri = polyDataTri->GetNumberOfPolys();
    out << numTri << " triangles." << endl;
    vtkDataArray *dataArray = polyDataTri->GetPointData()->GetScalars();

    // Get triangle cells.
    vtkCellArray *cells;
    cells = polyDataTri->GetPolys();

    xml = new QXmlStreamWriter(file);
    xml->setAutoFormatting(true);

    xml->writeStartDocument();
    xml->writeStartElement("VurtigoFile");
    xml->writeStartElement("FileInfo");
    xml->writeTextElement("type", "5");
    xml->writeTextElement("name", "itk-snap polyData");
    xml->writeEndElement(); // </FileInfo>
    xml->writeTextElement("numPhases", "1");
    xml->writeStartElement("Phase");
    xml->writeAttribute("Number", "0");
    xml->writeAttribute("Trigger", "0");

    xml->writeStartElement("PointList");
    // Write points.
    double coordinate[3];
    double label;
    double colorTable[8][3] = { {1, 0, 0},
                                {0, 1, 0},
                                {0, 0, 1},
                                {1, 1, 0},
                                {0, 1, 1},
                                {1, 0, 1},
                                {1, 1, 1},
                                {0, 0, 0.5} };

    for (vtkIdType i = 0; i < numPoints; i++) {
        polyDataTri->GetPoint(i, coordinate);
        label = dataArray->GetTuple1(i);
        xml->writeStartElement("Point");
        xml->writeAttribute("X", QString::number(coordinate[0], 'f', 4));
        xml->writeAttribute("Y", QString::number(coordinate[1], 'f', 4));
        xml->writeAttribute("Z", QString::number(coordinate[2], 'f', 4));
        xml->writeAttribute("R", QString::number(colorTable[int(label-1)][0], 'f', 4));
        xml->writeAttribute("G", QString::number(colorTable[int(label-1)][1], 'f', 4));
        xml->writeAttribute("B", QString::number(colorTable[int(label-1)][2], 'f', 4));
        xml->writeAttribute("ID", QString::number(i));
        xml->writeEndElement();
    }
    xml->writeEndElement(); // </PointList>

    xml->writeStartElement("CellList");
    // Write triangle data.
    vtkIdType npts, *pts;
    cells->InitTraversal();
    for (vtkIdType i = 0; i < numTri; i++) {
        cells->GetNextCell(npts, pts);
        xml->writeStartElement("CellPoint");
        xml->writeAttribute("Cell", QString::number(i));
        xml->writeAttribute("P0", QString::number(pts[0]));
        xml->writeAttribute("P1", QString::number(pts[1]));
        xml->writeAttribute("P2", QString::number(pts[2]));
        xml->writeEndElement();
    }
    xml->writeEndElement(); // </CellList>

    xml->writeEndElement(); // </Phase>
    xml->writeEndElement(); // </VurtigoFile>
    xml->writeEndDocument();

    triangleFilter->Delete();
    return true;
}
