#ifndef AUX
#define AUX

double thetaKB(double rx, double ry, double rz, double kx, double ky, double kz);
double gFunc(double rx, double ry, double rz);

double RHS_ri(double rx, double ry, double rz, double kx, double ky, double kz, int i);
double RHS_ki(double rx, double ry, double rz, double kx, double ky, double kz, int i);

#endif
