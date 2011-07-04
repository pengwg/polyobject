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
}

bool PolyData2Xml::WriteXml(QFile *file)
{
    QTextStream out(stdout);

    vtkPolyData *polyData = poly->GetOutput();
    poly->Update();

    vtkIdType numPoints = polyData->GetNumberOfPoints();
    out << "Input: " << numPoints << " Points, ";
    vtkIdType numStrips = polyData->GetNumberOfStrips();
    out << numStrips << " triangle strips." << endl;

    // Convert strips to triangels
    vtkTriangleFilter *triangleFilter = vtkTriangleFilter::New();
    triangleFilter->SetInput(polyData);
    vtkPolyData *polyDataTri = triangleFilter->GetOutput();
    triangleFilter->Update();

    numPoints = polyDataTri->GetNumberOfPoints();
    out << "Convert to: " << numPoints << " Points, ";
    vtkIdType numTri = polyDataTri->GetNumberOfPolys();
    out << numTri << " triangles." << endl;

    // Get points.
    vtkPoints *points;
    points = polyDataTri->GetPoints();

    // Get triangle cells.
    vtkCellArray *cells;
    cells = polyDataTri->GetPolys();

    // Get triangle data.
    vtkIdType npts, *pts;
    int n;
    cells->InitTraversal();
    for (vtkIdType i = 0; i < numTri; i++) {
        n = cells->GetNextCell(npts, pts);
    }

    xml = new QXmlStreamWriter(file);
    triangleFilter->Delete();
    return true;
}
