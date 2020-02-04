#ifndef AUX
#define AUX

void showVector(const char *msg, double x, double y, double z);
double thetaKB(double rx, double ry, double rz, double kx, double ky, double kz);
double gFunc(double rx, double ry, double rz);

double RHS_ri(double rx, double ry, double rz, double kx, double ky, double kz, int i);
double RHS_ki(double rx, double ry, double rz, double kx, double ky, double kz, int i);

#endif
