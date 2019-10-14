#include "stdafx.h"

#include "ValidationShapeFactory.h"
#include "Helpers.h"

static Float64 side = 10;

void ValidationShapeFactory::CreateBeam(ValidationShapeType type, IShape** ppShape)
{
   if (type == ValidationShapeType::Square)
   {
      CComPtr<IRectangle> square;
      square.CoCreateInstance(CLSID_Rect);
      square->put_Height(side);
      square->put_Width(side);
      CComQIPtr<IShape> shape(square);
      shape.CopyTo(ppShape);
   }
   else if (type == ValidationShapeType::Circle)
   {
      CComPtr<ICircle> circle;
      circle.CoCreateInstance(CLSID_Circle);
      circle->put_Radius(side / 2);
      CComQIPtr<IShape> shape(circle);
      shape.CopyTo(ppShape);
   }
   else if (type == ValidationShapeType::Triangle)
   {
      CComPtr<IPolyShape> polygon;
      polygon.CoCreateInstance(CLSID_PolyShape);
      polygon->AddPoint(0, 0);
      polygon->AddPoint(-side / 2, -sqrt(3)*side / 2);
      polygon->AddPoint(side / 2, -sqrt(3)*side / 2);
      CComQIPtr<IShape> shape(polygon);
      shape.CopyTo(ppShape);
   }
   else
   {
      *ppShape = nullptr;
   }
}


static std::_tstring gs_ShapeNames[] = {
   _T("Square"),
   _T("Circle"),
   _T("Equalateral Triangle"),
};

LPCTSTR ValidationShapeFactory::GetName(ValidationShapeType type)
{
   return gs_ShapeNames[(int)type].c_str();
}

int ValidationShapeFactory::GetApproxMethods(ValidationShapeType type)
{
   return AM_NONE;
}

Float64 ValidationShapeFactory::GetJApprox1(ValidationShapeType type)
{
   // the approximate equation doesn't really fit with this type
   return 0;
}
