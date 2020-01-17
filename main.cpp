#include <iostream>
#include <stdio.h>
#include <sys/stat.h>
#include <fstream>
#include <string>
#include <math.h>

#include "headers/io.hpp"
#include "headers/parameters.hpp"
#include "headers/algorithm.hpp"

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress (double percentage);

void initialize(int argc, char* argv[]);

int main(int argc, char* argv[]) {
  initialize(argc, argv);

  // loop through all initial phases
  int iter = 0;
  for (temp::phi0 = sim::phi_min; temp::phi0 <= sim::phi_max; temp::phi0 += sim::phi_step) {
    printProgress((temp::phi0 - sim::phi_min) / (sim::phi_max - sim::phi_min));
    std::string iter_str = std::to_string(iter);
    int nzeros = 0;
    if (iter < 10) {
      nzeros = 2;
    } else if (iter < 100) {
      nzeros = 1;
    }
    iter_str = std::string(nzeros, '0').append(iter_str);

    std::ofstream output(sim::output_path + "/" + sim::run_id + "_" + iter_str + "_phi_" + std::to_string(temp::phi0) + ".dat");
    iter++;
    // loop through all the points on the picture surface
    for (temp::a1 = sim::a1_min; temp::a1 <= sim::a1_max; temp::a1 += sim::a_step) {
      for (temp::a2 = sim::a2_min; temp::a2 <= sim::a2_max; temp::a2 += sim::a_step) {
        // // // length units are in stellar radii
        // base vector where the X-mode would approach (without refraction)
        temp::base_vec_x = temp::a1 * sim::p1_vec_x + temp::a2 * sim::p2_vec_x;
        temp::base_vec_y = temp::a1 * sim::p1_vec_y + temp::a2 * sim::p2_vec_y;
        temp::base_vec_z = temp::a1 * sim::p1_vec_z + temp::a2 * sim::p2_vec_z;

        // std::cout << temp::a1 << " " << temp::a2 << "\n";
        // std::cout << sim::p1_vec_x << " " << cos(params::dzeta) << " " << sim::p2_vec_x << "\n";
        // return 1;

        // initial point on the picture surface
        double r_vec_x = 4.0 * params::Rm * sim::o_vec_x + temp::base_vec_x;
        double r_vec_y = 4.0 * params::Rm * sim::o_vec_y + temp::base_vec_y;
        double r_vec_z = 4.0 * params::Rm * sim::o_vec_z + temp::base_vec_z;

        // initial photon momentum direction [unitless]
        double k_vec_x = sim::o_vec_x;
        double k_vec_y = sim::o_vec_y;
        double k_vec_z = sim::o_vec_z;

        double intensity = 0.0;
        bool good_exit = false;
        for (unsigned int i = 0; i < sim::nst_max; ++i) {
          temp::rr = sqrt(r_vec_x*r_vec_x + r_vec_y*r_vec_y + r_vec_z*r_vec_z);

          // check if closest position to star (or if closer than 2 radii)
          double dot_product = (r_vec_x * k_vec_x + r_vec_y * k_vec_y + r_vec_z * k_vec_z) / temp::rr;
          if (dot_product <= 1e-6 || temp::rr < 2) {
            good_exit = true;
            break;
          }
          integrateRvec(r_vec_x, r_vec_y, r_vec_z, k_vec_x, k_vec_y, k_vec_z);
          integrateKvec(r_vec_x, r_vec_y, r_vec_z, k_vec_x, k_vec_y, k_vec_z);
          intensity += computeIntensity(r_vec_x, r_vec_y, r_vec_z, k_vec_x, k_vec_y, k_vec_z);
        }
        if (!good_exit) {
          std::cout << "Number of steps too small, closest point not reached.\n Consider increasing `nst_max` or stepsize `dr_step`.\n";
        }
        output << temp::a1 << "," << temp::a2 << "," << intensity << "\n";
      }
    }
    output.close();
  }

  return 0;
}

void initialize(int argc, char* argv[]) {
  read_in_out(sim::input_file, sim::output_path, argc, argv);
  // Create output directory if doesn't exist
  struct stat st = {0};
  if (stat(sim::output_path.c_str(), &st) == -1) {
    mkdir(sim::output_path.c_str(), 0700);
  }
  defineParameters();

  std::string MODE;
  if (params::mode == 0) MODE = "X-mode";
  else MODE = "O-mode";

  std::ofstream outputParams(sim::output_path + "/" + sim::run_id + ".dat");
  outputParams
      << MODE
      << "\nR_LC = " << params::RLC
      << "\nalpha = " << params::alpha_deg
      << "\nbeta = " << params::beta_deg
      << "\n\nPeriod = " << params::P
      << "\nB12 = " << params::B12_surf
      << "\nfGHz = " << params::freq_GHz
      << "\n\nlambda = " << params::multiplicity
      << "\ngamma0 = " << params::gamma0
      << "\nf0 = " << params::f0
      << "\nR_em = " << params::Rm
      << "\nA = " << params::A
      << "\n\n\npicture plane limits: [" << sim::a1_min << "," << sim::a1_max << "][" << sim::a2_min << "," << sim::a2_max << "] step=" << sim::a_step
      << "\nphases: [" << sim::phi_min << "," << sim::phi_max << "] step=" << sim::phi_step
      << "\nradial step: " << sim::dr_step;
  outputParams.close();
}

void printProgress (double percentage) {
  int val = (int) (percentage * 100);
  int lpad = (int) (percentage * PBWIDTH);
  int rpad = PBWIDTH - lpad;
  printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
  fflush (stdout);
}
