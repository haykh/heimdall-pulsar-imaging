#include <math.h>
#include <string>
#include "headers/parameters.hpp"
#include "headers/algorithm.hpp"

// kinematics
void integrateRvec(double &rx, double &ry, double &rz, double kx, double ky, double kz) {
  rx -= sim::dr_step * kx;
  ry -= sim::dr_step * ky;
  rz -= sim::dr_step * kz;
}
void integrateKvec(double rx, double ry, double rz, double &kx, double &ky, double &kz) {
}

// intensity calculation
Vector3D mu_vec(double rx, double ry, double rz) {
  double phi0 = temp::phi0 * M_PI / 180.0;
  double omega_t = sqrt((rx - temp::base_vec_x)*(rx - temp::base_vec_x) +
                        (ry - temp::base_vec_y)*(ry - temp::base_vec_y) +
                        (rz - temp::base_vec_z)*(rz - temp::base_vec_z));
  omega_t *= constants::Rstar_over_c * params::Omega;
  Vector3D vec(sin(params::alpha) * cos(phi0 + omega_t),
               sin(params::alpha) * sin(phi0 + omega_t),
               cos(params::alpha));
  return vec;
}

Vector3D B_vec(double rx, double ry, double rz) {
  double nx = rx / temp::rr;
  double ny = ry / temp::rr;
  double nz = rz / temp::rr;
  Vector3D muvec = mu_vec(rx, ry, rz);
  double n_dot_mu = nx * muvec.x + ny * muvec.y + nz * muvec.z;

  Vector3D vec(3.0 * nx * n_dot_mu - muvec.x,
               3.0 * ny * n_dot_mu - muvec.y,
               3.0 * nz * n_dot_mu - muvec.z);
  return vec;
}

double thetaKB(double rx, double ry, double rz, double kx, double ky, double kz) {
  Vector3D bvec = B_vec(rx, ry, rz);
  double k_dot_b = kx * bvec.x + ky * bvec.y + kz * bvec.z;
  return acos(-k_dot_b / bvec.norm());
}

double sinPsiM(double rx, double ry, double rz) {
  Vector3D muvec = mu_vec(rx, ry, rz);
  double n_dot_mu = (rx * muvec.x + ry * muvec.y + rz * muvec.z) / temp::rr;
  return sqrt(1.0 - n_dot_mu * n_dot_mu);
}

// polar cap parameter
double intensityF1(double rx, double ry, double rz) {
  double rperp_over_R0 = sinPsiM(rx, ry, rz) / sqrt(temp::rr / params::RLC);
  return exp(-pow(rperp_over_R0, 2)) / (1.0 + pow(params::f0 / rperp_over_R0, 2));
}

// height from the surface parameter
double intensityF2() {
  return exp(-params::A * pow(temp::rr / params::Rm - 1.0, 2));
}

// gamma beaming parameter
double intensityF3(double rx, double ry, double rz, double kx, double ky, double kz) {
  double thetakb = thetaKB(rx, ry, rz, kx, ky, kz);
  return exp(-params::gamma0*params::gamma0 * thetakb*thetakb);
}

double computeIntensity(double rx, double ry, double rz, double kx, double ky, double kz) {
  double i1 = intensityF1(rx, ry, rz);
  double i2 = intensityF2();
  double i3 = intensityF3(rx, ry, rz, kx, ky, kz);
  return i1 * i2 * i3;
}
