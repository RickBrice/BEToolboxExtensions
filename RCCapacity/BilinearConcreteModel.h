#pragma once

#include <Materials/StressStrainModel.h>

class BilinearConcreteModel : public WBFL::Materials::StressStrainModel
{
public:
   BilinearConcreteModel() = default;
   BilinearConcreteModel(const std::_tstring& strName, Float64 fc, Float64 ec, Float64 coefficient);
   BilinearConcreteModel(const std::_tstring& strName);
   BilinearConcreteModel(const BilinearConcreteModel&) = default;
   virtual ~BilinearConcreteModel() = default;

   BilinearConcreteModel& operator=(const BilinearConcreteModel&) = delete;

   /// Sets the concrete strength
   void SetFc(Float64 fc);

   /// Returns the concrete strength
   Float64 GetFc() const;

   void SetEc(Float64 ec);
   Float64 GetEc() const;

   void SetCoefficient(Float64 k);
   Float64 GetCoefficient() const;

   /// Creates a clone of the stress-strain model
   virtual std::unique_ptr<StressStrainModel> Clone() const override;

   /// Returns the strain associated with yielding of the material
   virtual Float64 GetYieldStrain() const override;

   /// Returns the modulus of elasticity
   virtual Float64 GetModulusOfElasticity() const override;

   /// Returns the stress at a given level of strain
   virtual std::pair<Float64, bool> ComputeStress(Float64 strain) const override;

   /// Returns the range of strain values applicable to the model
   virtual std::pair<Float64, Float64> GetStrainLimits() const override;

   /// Returns the strain that corresponds to the peak stress
   virtual Float64 GetStrainAtPeakStress() const override;

private:
   Float64 m_fc{ 0.0 };
   Float64 m_Ec{ 0.0 };
   Float64 m_K{ 0.85 };
};

