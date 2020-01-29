#include <math.h>
#include <string>
#include "../headers/parameters.hpp"
#include "../headers/aux.hpp"
#include "../headers/algorithm.hpp"

// kinematics
void integrateRvec(double &rx, double &ry, double &rz, double kx, double ky, double kz) {
  rx -= sim::dr_step * RHS_ri(rx, ry, rz, kx, ky, kz, 0);
  ry -= sim::dr_step * RHS_ri(rx, ry, rz, kx, ky, kz, 1);
  rz -= sim::dr_step * RHS_ri(rx, ry, rz, kx, ky, kz, 2);
}
void integrateKvec(double rx, double ry, double rz, double &kx, double &ky, double &kz) {
  kx += sim::dr_step * RHS_ki(rx, ry, rz, kx, ky, kz, 0);
  ky += sim::dr_step * RHS_ki(rx, ry, rz, kx, ky, kz, 1);
  kz += sim::dr_step * RHS_ki(rx, ry, rz, kx, ky, kz, 2);
}

// intensity calculation
// polar cap parameter
double intensityF1(double rx, double ry, double rz) {
  return gFunc(rx, ry, rz);
}

// height from the surface parameter
double intensityF2(double rx, double ry, double rz) {
  double rr = sqrt(rx * rx + ry * ry + rz * rz);
  return exp(-params::A * pow(rr / params::Rm - 1.0, 2));
}

// gamma beaming parameter
double intensityF3(double rx, double ry, double rz, double kx, double ky, double kz) {
  double thetakb = thetaKB(rx, ry, rz, kx, ky, kz);
  return exp(-params::gamma0*params::gamma0 * thetakb*thetakb);
}

double computeIntensity(double rx, double ry, double rz, double kx, double ky, double kz) {
  double i1 = intensityF1(rx, ry, rz);
  double i2 = intensityF2(rx, ry, rz);
  double i3 = intensityF3(rx, ry, rz, kx, ky, kz);
  return i1 * i2 * i3;
}
