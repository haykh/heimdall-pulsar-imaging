#ifndef PARAMETERS
#define PARAMETERS

namespace constants {
  extern const double c, me, e, Rstar, Rstar_over_c;
}

namespace params {
  extern double P, B12_surf, alpha_deg, beta_deg, freq_GHz, gamma0, multiplicity, f0, A, Rm;
  extern double Omega, RLC, B_surf, freq;
  extern double alpha, beta, dzeta;
  extern int mode;
}

namespace sim {
  extern std::string input_file, output_path, run_id;
  extern double Rmax, phi_min, phi_max, phi_step, a1_min, a1_max, a2_min, a2_max, a_step, dr_step;
  extern unsigned int nst_max;
  extern double o_vec_x, o_vec_y, o_vec_z;
  extern double p1_vec_x, p1_vec_y, p1_vec_z;
  extern double p2_vec_x, p2_vec_y, p2_vec_z;
}

namespace temp {
  extern double a1, a2, phi0, rr;
  extern double base_vec_x, base_vec_y, base_vec_z;
}

struct Vector3D {
  double x, y, z;
  Vector3D(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
  double norm() {
    return sqrt(x*x + y*y + z*z);
  }
};

void defineParameters();

#endif
