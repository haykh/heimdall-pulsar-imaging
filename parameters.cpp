#include <string>
#include <math.h>
#include "headers/io.hpp"
#include "headers/parameters.hpp"

// speed of light in cm per second
const double constants::c = 3.0e10;
// stellar radius in cm
const double constants::me = 9.1094e-28;
// electron charge in CGS
const double constants::e = 4.8e-10;
// electron mass in CGS
const double constants::Rstar = 1.0e6;

const double constants::Rstar_over_c = constants::Rstar / constants::c;

double
        // period in sec
        params::P,
        // surface magnetic field in units of 10^12 G
        params::B12_surf,
        // inclination angle in degrees (otherwise called `chi`)
        params::alpha_deg,
        // inclination to the observer in degrees
        params::beta_deg,
        // wave frequency in GHz
        params::freq_GHz,
        // particle Lorentz-factor (for beaming)
        params::gamma0,
        // plasma multiplicity (n / nGJ)
        params::multiplicity,
        // polar cap opening parameter
        params::f0,
        // height from the surface parameter
        params::A,
        // radius of max intensity (in stellar radii)
        params::Rm;
// propagation mode (O-mode = 1, X-mode = 0)
int params::mode;


// derived definitions
double
    //  rotation frequency in 1/s
    params::Omega,
    //  light cylinder in stellar radii
    params::RLC,
    //  B-field in G
    params::B_surf,
    //  frequency in Hz
    params::freq;

double params::alpha, params::beta, params::dzeta;

// simulation parameters
std::string sim::input_file, sim::output_path;
std::string sim::run_id;

double
      // distance of the picture plane in units of Rstar (perpendicular to the line of sight)
      sim::Rmax,
      // starting phase
      sim::phi_min,
      // finishing phase
      sim::phi_max,
      // phase change step
      sim::phi_step,
      // screen size in `p1` direction (in stellar radii)
      sim::a1_min,
      // screen size in `p1` direction
      sim::a1_max,
      // screen size in `p2` direction (in stellar radii)
      sim::a2_min,
      // screen size in `p2` direction
      sim::a2_max,
      // spatial resolution of the screen
      sim::a_step,
      // integration step size in units of stellar radii
      sim::dr_step;
// maximum number of steps along the radius
unsigned int sim::nst_max;

// line of sight
double sim::o_vec_x, sim::o_vec_y, sim::o_vec_z;

// 1st vector perpendicular to the line of sight
double sim::p1_vec_x, sim::p1_vec_y, sim::p1_vec_z;

// 2nd vector perpendicular to the line of sight
double sim::p2_vec_x, sim::p2_vec_y, sim::p2_vec_z;

double temp::a1;
double temp::a2;
double temp::phi0;
double temp::rr;
double temp::base_vec_x;
double temp::base_vec_y;
double temp::base_vec_z;

void defineParameters() {
  params::B12_surf = read_from_file(sim::input_file, "B12");
  params::P = read_from_file(sim::input_file, "Period");
  params::freq_GHz = read_from_file(sim::input_file, "freqGHz");

  params::multiplicity = read_from_file(sim::input_file, "lambda");
  params::gamma0 = read_from_file(sim::input_file, "gamma0");
  params::f0 = read_from_file(sim::input_file, "f0");
  params::A = read_from_file(sim::input_file, "A");
  params::Rm = read_from_file(sim::input_file, "Rm");

  params::mode = (int)read_from_file(sim::input_file, "mode");

  params::alpha_deg = read_from_file(sim::input_file, "alpha_deg");
  params::beta_deg = read_from_file(sim::input_file, "beta_deg");

  //  rotation frequency in 1/s
  params::Omega = 2.0 * M_PI / params::P;
  //  light cylinder in stellar radii
  params::RLC = constants::c / (params::Omega * constants::Rstar);
  //  B-field in G
  params::B_surf = params::B12_surf * 1.0e12;
  // frequency in Hz
  params::freq = 2.0 * M_PI * 1.0e9 * params::freq_GHz;

  params::alpha = params::alpha_deg * M_PI / 180.0;
  params::beta = params::beta_deg * M_PI / 180.0;
  params::dzeta = params::alpha - params::beta;

  sim::run_id = read_from_file_str(sim::input_file, "RUN_ID", "id01");

  sim::phi_min = read_from_file(sim::input_file, "phi_min");
  sim::phi_max = read_from_file(sim::input_file, "phi_max");
  sim::phi_step = read_from_file(sim::input_file, "phi_step");

  sim::a1_min = read_from_file(sim::input_file, "a1_min");
  sim::a1_max = read_from_file(sim::input_file, "a1_max");
  sim::a2_min = read_from_file(sim::input_file, "a2_min");
  sim::a2_max = read_from_file(sim::input_file, "a2_max");
  sim::a_step = read_from_file(sim::input_file, "a_step");

  sim::o_vec_x = sin(params::dzeta);
  sim::o_vec_y = 0.0;
  sim::o_vec_z = cos(params::dzeta);

  sim::p1_vec_x = cos(params::dzeta);
  sim::p1_vec_y = 0.0;
  sim::p1_vec_z = -sin(params::dzeta);

  sim::p2_vec_x = 0.0;
  sim::p2_vec_y = 1.0;
  sim::p2_vec_z = 0.0;

  sim::Rmax = read_from_file(sim::input_file, "Rmax");
  sim::dr_step = read_from_file(sim::input_file, "dr");

  sim::nst_max = (unsigned int)read_from_file(sim::input_file, "nst_max");
}
