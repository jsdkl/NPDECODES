#ifndef PROPAGATOR_H_
#define PROPAGATOR_H_

/**
 * @file propagator.h
 * @brief NPDE homework NonLinSchroedingerEquation code
 * @author Oliver Rietmann
 * @date 03.05.2020
 * @copyright Developed at ETH Zurich
 */

#include <complex>
#include <memory>
#include <utility>

#include <Eigen/Core>
#include <Eigen/SparseCore>
#include <Eigen/SparseLU>

namespace NonLinSchroedingerEquation {

/** @brief Abstract interface for non-copyable propagator
 */
class Propagator {
public:
  Propagator() = default;
  virtual ~Propagator() = default;
  virtual Eigen::VectorXcd operator()(const Eigen::VectorXcd &mu) const = 0;

private:
  Propagator(const Propagator &) = delete;
  Propagator(Propagator &&) = delete;
  Propagator &operator=(const Propagator &) = delete;
  Propagator &operator=(const Propagator &&) = delete;
};

/** @brief Class for propagation according to the kinetic
 *  (i.e. Laplace) part if the NLSE
 */
class KineticPropagator : public Propagator {
  using SparseMatrixXcd = Eigen::SparseMatrix<std::complex<double>>;
  using SparseMatrixXd = Eigen::SparseMatrix<double>;

public:
  /** @brief Computes and caches the data necessary to
   *  perform a kinetic timestep of length tau using
   *  the implicit trapezoidal rule
   *  @param A stiffness matrix of shape $N \times N$
   *  @param M complex mass matrix of shape $N \times N$
   *  @param tau size of the timestep to perform
   */
  KineticPropagator(const SparseMatrixXd &A, const SparseMatrixXcd &M,
                    double tau);
  /** @brief Performs a kinetic timestep of length tau
   *  @param mu vector of length $N$ containing nodal values
   *  before the timestep
   *  @return vector of length $N$ containg the nodal values
   *  after the timestep
   */
  Eigen::VectorXcd operator()(const Eigen::VectorXcd &mu) const override;

private:
#if SOLUTION
  SparseMatrixXcd B_plus_;
  Eigen::SparseLU<SparseMatrixXcd> solver_;
#else
  //====================
  // Your code goes here
  // Add needed member variables here
  //====================
#endif
};

/** @brief Class for propagation according to the interaction
 *  (i.e. non-linear) part if the NLSE
 */
class InteractionPropagator : public Propagator {
public:
  /** @brief Computes and caches the data necessary to
   *  perform an interaction timestep of length tau using
   *  the analytic solution
   *  @param tau size of the timestep to perform
   */
  InteractionPropagator(double tau);
  /** @brief Performs an interaction timestep of length tau
   *  @param mu vector of length $N$ containing nodal values
   *  before the timestep
   *  @return vector of length $N$ containg the nodal values
   *  after the timestep
   */
  Eigen::VectorXcd operator()(const Eigen::VectorXcd &mu) const override;

private:
#if SOLUTION
  // Componentwise Function that performs a timestep tau
  // when applied to mu.
  std::function<std::complex<double>(std::complex<double>)> phase_multiplier_;
#else
  //====================
  // Your code goes here
  // Add needed member variables here
  //====================
#endif
};

/** @brief Class for propagation according Strang splitting between two
  * propagators.
 */
class SplitStepPropagator : public Propagator {
public:
  /** @brief Assigns the member variables by forwarding the passed pointers.
   *  @param semi_step propagator for timestep $\frac{\tau}{2}$ to be
   *  executed as first and last step in the Strang splitting
   *  @param full_step propagator for timestep $\tau$ to be executed
   *  as intermediate step in the Strang splitting
   */
  SplitStepPropagator(std::unique_ptr<Propagator> semi_step,
                      std::unique_ptr<Propagator> full_step);
  /** @brief Performs the Strang splitting.
   *  @param mu vector of length $N$ containing nodal values
   *  before the timestep
   *  @return vector of length $N$ containg the nodal values
   *  after the timestep
   */
  Eigen::VectorXcd operator()(const Eigen::VectorXcd &mu) const override;

private:
#if SOLUTION
  std::unique_ptr<Propagator> semi_step_;
  std::unique_ptr<Propagator> full_step_;
#else
  //====================
  // Your code goes here
  // Add needed member variables here
  //====================
#endif
};

}  // namespace NonLinSchroedingerEquation

#endif // PROPAGATOR_H_
