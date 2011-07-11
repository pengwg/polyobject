#include <vtkPointData.h>
#include <vtkCellArray.h>
#include <vtkPolyData.h>

#include <QTextStream>

#include "polydata2xml.h"

PolyData2Xml::PolyData2Xml(QFile *file)
{
    QTextStream out(stdout);

    vtkPolyDataReader *poly = vtkPolyDataReader::New();
    poly->SetFileName(file->fileName().toAscii().constData());

    if (!poly->IsFilePolyData())
        qFatal("Not valid PolyData file!");

    vtkPolyData *polyData = poly->GetOutput();
    poly->Update();

    numPoints = polyData->GetNumberOfPoints();
    out << "Input -- " << numPoints << " points, ";
    vtkIdType numStrips = polyData->GetNumberOfStrips();
    out << numStrips << " triangle strips." << endl;

    // Convert strips to triangels
    triangleFilter = vtkTriangleFilter::New();
    triangleFilter->SetInput(polyData);
    polyDataTri = triangleFilter->GetOutput();
    triangleFilter->Update();

    numPoints = polyDataTri->GetNumberOfPoints();
    out << "Convert to -- " << numPoints << " points, ";
    numTri = polyDataTri->GetNumberOfPolys();
    out << numTri << " triangles." << endl;

    poly->Delete();
}

PolyData2Xml::~PolyData2Xml()
{
    triangleFilter->Delete();
}

bool PolyData2Xml::WriteXml(QFile *file)
{
    vtkDataArray *dataArray = polyDataTri->GetPointData()->GetScalars();

    // Get triangle cells.
    vtkCellArray *cells;
    cells = polyDataTri->GetPolys();

    QXmlStreamWriter *xml = new QXmlStreamWriter(file);
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
    double label = 1;
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
        if (dataArray)
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
    delete xml;
    return true;
}
