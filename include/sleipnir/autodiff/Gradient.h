// Copyright (c) Joshua Nichols and Tyler Veness

#pragma once

#include <vector>

#include <Eigen/SparseCore>

#include "sleipnir/SymbolExports.h"
#include "sleipnir/autodiff/Expression.h"
#include "sleipnir/autodiff/Profiler.h"
#include "sleipnir/autodiff/Variable.h"

namespace sleipnir::autodiff {

/**
 * This class calculates the Jacobian of a vector of variables with respect to a
 * vector of variables.
 *
 * The Jacobian is only recomputed if the variable expression is quadratic or
 * higher order.
 */
class SLEIPNIR_DLLEXPORT Gradient {
 public:
  /**
   * Constructs a Gradient object.
   *
   * @param variable Variable of which to compute the gradient.
   * @param wrt Variable with respect to which to compute the gradient.
   */
  Gradient(Variable variable, Variable wrt) noexcept;

  /**
   * Constructs a Gradient object.
   *
   * @param variable Variable of which to compute the gradient.
   * @param wrt Variables with respect to which to compute the gradient.
   */
  Gradient(Variable variable, Eigen::Ref<VectorXvar> wrt) noexcept;

  /**
   * Calculates the gradient.
   */
  const Eigen::SparseVector<double>& Calculate();

  /**
   * Updates the value of the variable.
   */
  void Update();

  /**
   * Returns the profiler.
   */
  Profiler& GetProfiler();

 private:
  Variable m_variable;
  VectorXvar m_wrt;

  Eigen::SparseVector<double> m_g;

  Profiler m_profiler;

  void CalculateImpl();
};

}  // namespace sleipnir::autodiff
