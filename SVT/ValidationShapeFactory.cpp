#include "stdafx.h"

#include "ValidationShapeFactory.h"
#include "Helpers.h"

static Float64 side = 10;

void ValidationShapeFactory::CreateBeam(ValidationShapeType type, IUnitConvert* pConvert, IShape** ppShape)
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
   else if (type == ValidationShapeType::Rectangle101)
   {
      CComPtr<IRectangle> square;
      square.CoCreateInstance(CLSID_Rect);
      square->put_Height(5);
      square->put_Width(50);
      CComQIPtr<IShape> shape(square);
      shape.CopyTo(ppShape);
   }
   else if (type == ValidationShapeType::Rectangle201)
   {
      CComPtr<IRectangle> square;
      square.CoCreateInstance(CLSID_Rect);
      square->put_Height(5);
      square->put_Width(100);
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


std::unique_ptr<WBFL::Geometry::Shape> ValidationShapeFactory::CreateBeam(ValidationShapeType type)
{
   std::unique_ptr<WBFL::Geometry::Shape> shape;
   if (type == ValidationShapeType::Square)
   {
      auto square = std::make_unique<WBFL::Geometry::Rectangle>();
      square->SetHeight(side);
      square->SetWidth(side);
      shape = std::move(square);
   }
   else if (type == ValidationShapeType::Rectangle101)
   {
      auto square = std::make_unique<WBFL::Geometry::Rectangle>();
      square->SetHeight(5);
      square->SetWidth(50);
      shape = std::move(square);
   }
   else if (type == ValidationShapeType::Rectangle201)
   {
      auto square = std::make_unique<WBFL::Geometry::Rectangle>();
      square->SetHeight(5);
      square->SetWidth(100);
      shape = std::move(square);
   }
   else if (type == ValidationShapeType::Circle)
   {
      auto circle = std::make_unique<WBFL::Geometry::Circle>();
      circle->SetRadius(side / 2);
      shape = std::move(circle);
   }
   else if (type == ValidationShapeType::Triangle)
   {
      auto polygon = std::make_unique<WBFL::Geometry::Polygon>();
      polygon->AddPoint(0, 0);
      polygon->AddPoint(-side / 2, -sqrt(3) * side / 2);
      polygon->AddPoint(side / 2, -sqrt(3) * side / 2);
      shape = std::move(polygon);
   }

   return shape;
}


static std::_tstring gs_ShapeNames[] = {
   _T("Square"),
   _T("Rectangle 10:1"),
   _T("Rectangle 20:1"),
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

Float64 ValidationShapeFactory::GetJApprox1(ValidationShapeType type, IUnitConvert* pConvert)
{
   // the approximate equation doesn't really fit with this type
   return 0;
}

Float64 ValidationShapeFactory::GetJApprox1(ValidationShapeType type)
{
   // the approximate equation doesn't really fit with this type
   return 0;
}

Float64 ValidationShapeFactory::GetJApprox3(ValidationShapeType type)
{
   // the approximate equation doesn't really fit with this type
   return 0;
}
