/**
 * @file
 * @brief NPDE homework ElementMatrixComputation code
 * @author Janik Schüttler
 * @date 06.03.2019
 * @copyright Developed at ETH Zurich
 */

#include "solve.h"

#include <iostream>

#include <Eigen/Core>

#include <lf/uscalfe/uscalfe.h>

#include "MyLinearFEElementMatrix.h"
#include "MyLinearLoadVector.h"

namespace ElementMatrixComputation {

/* SAM_LISTING_BEGIN_2 */
Eigen::VectorXd solvePoissonBVP() {
  // Convert tPoissonda function f to a LehrFEM++ mesh function object
  lf::uscalfe::MeshFunctionGlobal mf_f{f};

  // Define the solution vector
  Eigen::VectorXd solution = Eigen::VectorXd::Zero(1);

#if SOLUTION
  // Define the element matrix and element vector builders and solve the system
  lf::uscalfe::LinearFELaplaceElementMatrix elmat_builder;
  lf::uscalfe::LinearFELocalLoadVector<double, decltype(mf_f)> elvec_builder(
      mf_f);

  std::cout << "===================" << std::endl;
  std::cout << "solvePoissonBVP" << std::endl;

  solution = solve(elmat_builder, elvec_builder);
#else
  //====================
  // Your code goes here
  //====================
#endif

  return solution;
}
/* SAM_LISTING_END_2 */

Eigen::VectorXd solveNeumannEq() {
  // Define the solution vector
  Eigen::VectorXd solution;

#if SOLUTION
  // Define the element matrix and element vector builders and solve the system
  MyLinearFEElementMatrix elmat_builder;
  MyLinearLoadVector elvec_builder(f);

  std::cout << "===================" << std::endl;
  std::cout << "solveNeumannEq" << std::endl;
  // The actual computation
  solution = solve(elmat_builder, elvec_builder);
#else
  //====================
  // Your code goes here
  //====================
#endif

  return solution;
}

}  // namespace ElementMatrixComputation
