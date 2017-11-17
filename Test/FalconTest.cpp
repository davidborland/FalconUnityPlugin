#include <stdio.h>

#include "Falcon.h"

#include <hdlu/hdlu.h>

double pos[3];
double force[3];

// On-demand servo callback function
HDLServoOpExitCode PosCB(void* userData) {
    hdlToolPosition(pos);
    return HDL_SERVOOP_EXIT;
}

// On-demand servo callback function
HDLServoOpExitCode ForCB(void* userData) {
    hdlSetToolForce(force);
    return HDL_SERVOOP_EXIT;
}

int main() {
    Falcon* falcon = new Falcon();
    falcon->Initialize();

    Vector3 center;
    center.x = center.y = center.z = 0.0;

    Vector3 size;
    size.x = size.y = size.z = 10.0;

    falcon->SetGraphicsWorkspace(center, size);
    
    // Constant force
    /*
    Vector3 f;
    f.x = f.z = 0.0;
    f.y = -5.0;

    falcon->AddConstantForce(f);
    */

    // Viscosity
    //falcon->AddViscosity(0.5, 0.1);

    // Surface
    /*
    Vector3 p;
    p.x = p.y = p.z = 0.0;

    Vector3 n;
    n.x = n.z = 0.0;
    n.y = 1.0;

    falcon->AddSurface(2.0, 0.01, p, n);
    */

    // Spring
    /*
    Vector3 p;
    p.x = p.y = p.z = 0.0;

    falcon->AddSpring(2.0, 0.01, 0.0, 2.5, p);
    */

    // Intermolecular force
    /*
    Vector3 p;
    p.x = p.y = p.z = 0.0f;

    falcon->AddIntermolecularForce(10.0f, 0.01f, 2.0f, 4.0f, p);
    */
    

    // Random force
    //falcon->AddRandomForce(1.0, 5.0, 0.01, 0.1);


    Vector3 v;
    v.x = 5;
    v.y = 0;
    v.z = 0;

    int id = falcon->AddConstantForce(v);

    while (1) {
        v.x *= -1.0;
        falcon->SetConstantForce(id, v);
        Vector3 f = falcon->GetForce();
        printf("%f %f %f\n", f.x, f.y, f.z);
    }

    return 0;
}