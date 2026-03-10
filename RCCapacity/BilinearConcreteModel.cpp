#include "StdAfx.h"
#include "BilinearConcreteModel.h"

using namespace WBFL::Materials;

BilinearConcreteModel::BilinearConcreteModel(const std::_tstring& strName, Float64 fc, Float64 ec, Float64 coefficient) :
   WBFL::Materials::StressStrainModel(strName), m_fc(fc), m_Ec(ec), m_K(coefficient)
{
}

BilinearConcreteModel::BilinearConcreteModel(const std::_tstring& strName) :
   WBFL::Materials::StressStrainModel(strName)
{
}

void BilinearConcreteModel::SetFc(Float64 fc)
{
   PRECONDITION(0 < fc);
   m_fc = fc;
}

Float64 BilinearConcreteModel::GetFc() const
{
   return m_fc;
}

void BilinearConcreteModel::SetEc(Float64 ec)
{
   m_Ec = ec;
}

Float64 BilinearConcreteModel::GetEc() const
{
   return m_Ec;
}

void BilinearConcreteModel::SetCoefficient(Float64 k)
{
   m_K = k;
}

Float64 BilinearConcreteModel::GetCoefficient() const
{
   return m_K;
}

std::unique_ptr<StressStrainModel> BilinearConcreteModel::Clone() const
{
   return std::make_unique<BilinearConcreteModel>(*this);
}

Float64 BilinearConcreteModel::GetYieldStrain() const
{
   return -m_K*m_fc/m_Ec;
}

Float64 BilinearConcreteModel::GetModulusOfElasticity() const
{
   return m_Ec;
}

std::pair<Float64, bool> BilinearConcreteModel::ComputeStress(Float64 strain) const
{
   Float64 stress = 0;
   bool bWithinStrainLimits = true;
   if (0 < strain)
   {
      stress = 0.0; // ksi
   }
   else
   {
      // compression
      if (GetYieldStrain() < strain)
         stress = m_Ec * strain;
      else
         stress = -1.0 * m_K * m_fc;
   }

   return std::make_pair(stress, bWithinStrainLimits);
}

std::pair<Float64, Float64> BilinearConcreteModel::GetStrainLimits() const
{
   return { -0.003, 0.0 };
}

Float64 BilinearConcreteModel::GetStrainAtPeakStress() const
{
   return -0.003;
}
