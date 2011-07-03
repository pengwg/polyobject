#include <vtkCellArray.h>
#include <vtkPolyData.h>
#include <vtkTriangleFilter.h>

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
    xml = new QXmlStreamWriter(file);

    vtkPolyData *polyData = poly->GetOutput();
    poly->Update();

    // Get points.
    vtkPoints *points;
    vtkIdType numPoints = polyData->GetNumberOfPoints();
    points = GetPoints();

    // Get strip cells.
    vtkCellArray *cells;
    vtkIdType numStrips = polyData->GetNumberOfStrips();
    cells = data->GetStrips();

    vtkTriangleFilter *triangleFilter = vtkTriangleFilter::New();
    triangleFilter->SetInput(polyData);


    // Get Strip data.
    vtkIdType npts, *pts;
    int n;
    cells->InitTraversal();
    for (vtkIdType i = 0; i < numStrips; i++) {
        n = cells->GetNextCell(npts, pts);
    }

    return true;
}
