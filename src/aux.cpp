#include <math.h>
#include <iostream>
#include <string>
#include "../headers/parameters.hpp"
#include "../headers/aux.hpp"

void showVector(const char *msg, double x, double y, double z) {
  std::cout << msg << "x: " << x << " | "
            << msg << "y: " << y << " | "
            << msg << "z: " << z << "\n";
}

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
  double rr = sqrt(rx * rx + ry * ry + rz * rz);
  double nx = rx / rr;
  double ny = ry / rr;
  double nz = rz / rr;
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
  return acos(k_dot_b / bvec.norm());
}

double sinPsiM(double rx, double ry, double rz) {
  Vector3D muvec = mu_vec(rx, ry, rz);
  double rr = sqrt(rx * rx + ry * ry + rz * rz);
  double n_dot_mu = (rx * muvec.x + ry * muvec.y + rz * muvec.z) / rr;
  return sqrt(1.0 - n_dot_mu * n_dot_mu);
}

double gFunc(double rx, double ry, double rz) {
  double rr = sqrt(rx * rx + ry * ry + rz * rz);
  double rperp_over_R0 = sinPsiM(rx, ry, rz) * sqrt(params::RLC / rr);
  return exp(-pow(rperp_over_R0, 2)) / (1.0 + pow(params::f0 / rperp_over_R0, 5));
}

/* ------------------------------------------------------------------------------ */

double nRef(double theta_kB, double thetaStar_fourth) {
  double theta_kB_sqr = theta_kB * theta_kB;
  return 1.0 + 0.25 * theta_kB_sqr - sqrt(0.0625 * theta_kB_sqr * theta_kB_sqr + thetaStar_fourth);
}

double GJDensity(double rx, double ry, double rz) { // in CGS
  double rr = sqrt(rx * rx + ry * ry + rz * rz);
  double Bz = B_vec(rx, ry, rz).z * params::B_surf / pow(rr, 3); // in [Gauss]
  return params::Omega * Bz / (2.0 * M_PI * constants::c * constants::e);
}

double plasmaDensity(double rx, double ry, double rz) {
  return params::multiplicity * GJDensity(rx, ry, rz) * gFunc(rx, ry, rz);
}

double omegaPlasma_sqr(double rx, double ry, double rz) {
  return 4.0 * M_PI * plasmaDensity(rx, ry, rz) * constants::e * constants::e / constants::me;
}

double thetaStar_fourth(double rx, double ry, double rz) {
  return omegaPlasma_sqr(rx, ry, rz) / (pow(params::freq, 2) * pow(params::gamma0, 3));
}

double nRefInv(double rx, double ry, double rz, double kx, double ky, double kz) {
  double theta_kB = thetaKB(rx, ry, rz, kx, ky, kz);
  double theta_star_4 = thetaStar_fourth(rx, ry, rz);
  return 1.0 / nRef(theta_kB, theta_star_4);
}

double d_nrefInv_d_ri(double rx, double ry, double rz, double kx, double ky, double kz, int i) {
  double l_step = 0.001;
  if (i == 0) {
    return (nRefInv(rx + 0.5 * l_step, ry, rz, kx, ky, kz) - nRefInv(rx - 0.5 * l_step, ry, rz, kx, ky, kz)) / l_step;
  } else if (i == 1) {
    return (nRefInv(rx, ry + 0.5 * l_step, rz, kx, ky, kz) - nRefInv(rx, ry - 0.5 * l_step, rz, kx, ky, kz)) / l_step;
  } else {
    return (nRefInv(rx, ry, rz + 0.5 * l_step, kx, ky, kz) - nRefInv(rx, ry, rz - 0.5 * l_step, kx, ky, kz)) / l_step;
  }
}

double d_nrefInv_d_ki(double rx, double ry, double rz, double kx, double ky, double kz, int i) {
  double k_step = 0.00000001;
  if (i == 0) {
    return (nRefInv(rx, ry, rz, kx + 0.5 * k_step, ky, kz) - nRefInv(rx, ry, rz, kx - 0.5 * k_step, ky, kz)) / k_step;
  } else if (i == 1) {
    return (nRefInv(rx, ry, rz, kx, ky + 0.5 * k_step, kz) - nRefInv(rx, ry, rz, kx, ky - 0.5 * k_step, kz)) / k_step;
  } else {
    return (nRefInv(rx, ry, rz, kx, ky, kz + 0.5 * k_step) - nRefInv(rx, ry, rz, kx, ky, kz - 0.5 * k_step)) / k_step;
  }
}

double RHS_ri(double rx, double ry, double rz, double kx, double ky, double kz, int i) {
  double ki;
  if (i == 0) {
    ki = kx;
  } else if (i == 1) {
    ki = ky;
  } else {
    ki = kz;
  }
  if (params::mode == 0)
    return ki;
  else {
    double kk = sqrt(kx * kx + ky * ky + kz * kz);
    return nRefInv(rx, ry, rz, kx, ky, kz) * ki / kk + kk * d_nrefInv_d_ki(rx, ry, rz, kx, ky, kz, i);
  }
}

double RHS_ki(double rx, double ry, double rz, double kx, double ky, double kz, int i) {
  double kk = sqrt(kx * kx + ky * ky + kz * kz);
  return kk * d_nrefInv_d_ri(rx, ry, rz, kx, ky, kz, i);
}

// double oneOVERnref(double rx, double ry, double rz, double kx, double ky, double kz) {
//   double theta_kB = thetaKB(rx, ry, rz, kx, ky, kz);
//   double theta_star_4 = thetaStar_fourth(rx, ry, rz);
//   double n_ref = nRef(theta_kB, theta_star_4);
//   double k_mag = sqrt(kx * kx + ky * ky + kz * kz);
//   return k_mag / n_ref;
// }
//
// double d_kOVERnref_d_ri(double rx, double ry, double rz, double kx, double ky, double kz, int i) {
//   double l_step = 0.001;
//   if (i == 0) {
//     return (kOVERnref(rx + 0.5 * l_step, ry, rz, kx, ky, kz) - kOVERnref(rx - 0.5 * l_step, ry, rz, kx, ky, kz)) / l_step;
//   } else if (i == 1) {
//     return (kOVERnref(rx, ry + 0.5 * l_step, rz, kx, ky, kz) - kOVERnref(rx, ry - 0.5 * l_step, rz, kx, ky, kz)) / l_step;
//   } else {
//     return (kOVERnref(rx, ry, rz + 0.5 * l_step, kx, ky, kz) - kOVERnref(rx, ry, rz - 0.5 * l_step, kx, ky, kz)) / l_step;
//   }
// }
//
// double d_kOVERnref_d_ki(double rx, double ry, double rz, double kx, double ky, double kz, int i) {
//   double k_step = 0.00000001;
//   if (i == 0) {
//     return (kOVERnref(rx, ry, rz, kx + 0.5 * k_step, ky, kz) - kOVERnref(rx, ry, rz, kx - 0.5 * k_step, ky, kz)) / k_step;
//   } else if (i == 1) {
//     return (kOVERnref(rx, ry, rz, kx, ky + 0.5 * k_step, kz) - kOVERnref(rx, ry, rz, kx, ky - 0.5 * k_step, kz)) / k_step;
//   } else {
//     return (kOVERnref(rx, ry, rz, kx, ky, kz + 0.5 * k_step) - kOVERnref(rx, ry, rz, kx, ky, kz - 0.5 * k_step)) / k_step;
//   }
// }
