#ifndef objects
#define objects


void Sphere(double x, double y, double z, double r);
void Ground(int texture);
void Tree(double x, double y, double z, double rotation, bool canopy);
void Canopy(double x, double y, double z, double sx, double sy, double sz, int color);
void FarmHouse(double x, double y, double z, double rotation);
void Fence(double x, double y, double z, double rotation);
void Car(double x, double y, double z);

#endif