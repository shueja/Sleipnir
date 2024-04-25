// Copyright (c) Sleipnir contributors

#pragma once

#include <concepts>

#include <Eigen/Core>
#include <Eigen/SparseCore>
#include <fmt/format.h>

#include "sleipnir/autodiff/Variable.hpp"
#include "sleipnir/autodiff/VariableBlock.hpp"
#include "sleipnir/autodiff/VariableMatrix.hpp"

// FIXME: Doxygen gives internal inconsistency errors:
//   scope for class sleipnir::fmt::formatter< Derived, CharT > not found!
//   scope for class sleipnir::fmt::formatter< sleipnir::Variable > not found!
//   scope for class sleipnir::fmt::formatter< T > not found!

//! @cond Doxygen_Suppress

/**
 * Formatter for classes derived from Eigen::DenseBase<Derived> or
 * Eigen::SparseCompressedBase<Derived>.
 */
template <typename Derived, typename CharT>
  requires std::derived_from<Derived, Eigen::DenseBase<Derived>> ||
           std::derived_from<Derived, Eigen::SparseCompressedBase<Derived>>
struct fmt::formatter<Derived, CharT> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return m_underlying.parse(ctx);
  }

  template <typename FmtContext>
  auto format(const Derived& mat, FmtContext& ctx) const {
    auto out = ctx.out();

    for (int row = 0; row < mat.rows(); ++row) {
      for (int col = 0; col < mat.cols(); ++col) {
        out = fmt::format_to(out, "  ");
        out = m_underlying.format(mat.coeff(row, col), ctx);
      }

      if (row < mat.rows() - 1) {
        out = fmt::format_to(out, "\n");
      }
    }

    return out;
  }

 private:
  fmt::formatter<typename Derived::Scalar, CharT> m_underlying;
};

/**
 * Formatter for sleipnir::Variable.
 */
template <>
struct fmt::formatter<sleipnir::Variable> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return m_underlying.parse(ctx);
  }

  template <typename FmtContext>
  auto format(const sleipnir::Variable& variable, FmtContext& ctx) const {
    return m_underlying.format(variable.Value(), ctx);
  }

 private:
  fmt::formatter<double> m_underlying;
};

/**
 * Formatter for sleipnir::VariableBlock or sleipnir::VariableMatrix.
 */
template <typename T>
  requires std::same_as<T, sleipnir::VariableBlock<sleipnir::VariableMatrix>> ||
           std::same_as<T, sleipnir::VariableMatrix>
struct fmt::formatter<T> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return m_underlying.parse(ctx);
  }

  template <typename FmtContext>
  auto format(const T& mat, FmtContext& ctx) const {
    auto out = ctx.out();

    for (int row = 0; row < mat.Rows(); ++row) {
      for (int col = 0; col < mat.Cols(); ++col) {
        out = fmt::format_to(out, "  ");
        out = m_underlying.format(mat(row, col).Value(), ctx);
      }

      if (row < mat.Rows() - 1) {
        out = fmt::format_to(out, "\n");
      }
    }

    return out;
  }

 private:
  fmt::formatter<double> m_underlying;
};

//! @endcond
