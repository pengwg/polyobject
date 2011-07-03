#include <vtkCellArray.h>
#include <vtkPolyData.h>
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

    vtkPolyData *data = poly->GetOutput();
    poly->Update();

    vtkCellArray *cells;
    vtkIdType numPoints = data->GetNumberOfPoints();
    vtkIdType numStrips = data->GetNumberOfStrips();

    if (numStrips == 0) {
        qWarning("0 strips");
        return false;
    }
    else
        cells = data->GetStrips();

    vtkIdType npts, *pts;
    int n;

    cells->InitTraversal();
    for (vtkIdType i = 0; i < numStrips; i++) {
        n = cells->GetNextCell(npts, pts);
    }

    return true;
}
