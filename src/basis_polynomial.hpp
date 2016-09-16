#ifndef BASIS_POLYNOMIAL_HPP
#define BASIS_POLYNOMIAL_HPP

#include "Omega_h_r3d.hpp"
#include "size.hpp"

#include <iostream> //REMOVE THIS NOW

namespace Omega_h {

/* Convert one of the linear simplex basis
 * functions (vertex basis functions) into
 * a linear polynomial in global (x,y,z) coordinates.
 * Here we distinguish between (xi) which is the
 * vector of 3 independent barycentric coordinates,
 * and (b), which represents one of the four basis functions.
 * as the (db_dxi) logic suggests, (b) may be equal
 * to one of the three (xi) components or it is the
 * leftover barycentric coordinate defined by
 *   b = 1 - xi[0] - xi[1] - xi[2];
 */

template <Int dim>
INLINE r3d::Polynomial<dim, 1> get_basis_polynomial(
    Few<Vector<dim>, dim + 1> elem_pts,
    Int elem_vert) {
  auto dx_dxi = simplex_basis<dim, dim>(elem_pts);
  Vector<dim> db_dxi;
  if (elem_vert) {
    db_dxi = zero_vector<dim>();
    db_dxi[elem_vert - 1] = 1;
  } else {
    for (Int i = 0; i < dim; ++i)
      db_dxi[i] = -1;
  }
  std::cout << "db_dxi";
  for (Int i = 0; i < dim; ++i)
    std::cout << ' ' << db_dxi[i];
  std::cout << '\n';
  auto dxi_db = pseudo_invert(db_dxi);
  std::cout << "dxi_db";
  for (Int i = 0; i < dim; ++i)
    std::cout << ' ' << dxi_db[i];
  std::cout << '\n';
  auto dx_db = dx_dxi * dxi_db;
  std::cout << "dx_db";
  for (Int i = 0; i < dim; ++i)
    std::cout << ' ' << dx_db[i];
  std::cout << '\n';
  auto db_dx = pseudo_invert(dx_db);
  std::cout << "db_dx";
  for (Int i = 0; i < dim; ++i)
    std::cout << ' ' << db_dx[i];
  std::cout << '\n';
  auto other_vert = (elem_vert + 1) % (dim + 1);
  std::cout << "other vert " << other_vert << '\n';
  auto origin = elem_pts[other_vert];
  std::cout << "origin";
  for (Int i = 0; i < dim; ++i)
    std::cout << ' ' << origin[i];
  std::cout << '\n';
  auto origin_val = db_dx * (-origin);
  std::cout << "origin val " << origin_val << '\n';
  r3d::Polynomial<dim, 1> poly;
  poly.coeffs[0] = origin_val;
  for (Int i = 0; i < dim; ++i)
    poly.coeffs[i + 1] = db_dx[i];
  return poly;
}

}

#endif
