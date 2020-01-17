#ifndef ALGORITHM
#define ALGORITHM

void integrateRvec(double &rx, double &ry, double &rz, double kx, double ky, double kz);
void integrateKvec(double rx, double ry, double rz, double &kx, double &ky, double &kz);
double computeIntensity(double rx, double ry, double rz, double kx, double ky, double kz);

#endif
