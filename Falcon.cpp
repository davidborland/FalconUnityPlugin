/*=========================================================================

  Name:        Falcon.cpp

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Class to encapsulate the Novint Falcon device.

=========================================================================*/


#include "Falcon.h"

#include <iostream>

#include <hdlu/hdlu.h>


// Utility functions
void VectorSet(double result[3], double x, double y, double z) {
    result[0] = x;
    result[1] = y;
    result[2] = z;
}

void VectorCopy(double result[3], const double v[3]) {
    result[0] = v[0];
    result[1] = v[1];
    result[2] = v[2];
}

void VectorAdd(double result[3], const double v1[3], const double v2[3]) {
    result[0] = v1[0] + v2[0];
    result[1] = v1[1] + v2[1];
    result[2] = v1[2] + v2[2];
}

void VectorSubtract(double result[3], const double v1[3], const double v2[3]) {
    result[0] = v1[0] - v2[0];
    result[1] = v1[1] - v2[1];
    result[2] = v1[2] - v2[2];
}

void VectorScale(double result[3], const double v[3], double s) {
    result[0] = v[0] * s;
    result[1] = v[1] * s;
    result[2] = v[2] * s;
}

double VectorMagnitudeSquared(const double v[3]) {
    return v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
}

double VectorMagnitude(const double v[3]) {
    return sqrt(VectorMagnitudeSquared(v));
}

void VectorNormalize(double result[3], const double v[3]) {
    // Not checking for divide by zero...
    VectorScale(result, v, 1.0 / VectorMagnitude(v));
}

double VectorDotProduct(const double v1[3], const double v2[3]) {
    return v1[0]*v2[0] + v1[1]*v2[1] + v2[2]*v2[2];
}

void VectorPrint(const double v[3]) {
    printf("%f, %f, %f\n", v[0], v[1], v[2]);
}

void MatrixVectorMultiply(double result[3], const double m[16], const double v[3]) {
    result[0] = m[0]  * v[0] +
                m[4]  * v[1] +
                m[8]  * v[2] +
                m[12];
    
    result[1] = m[1]  * v[0] +
                m[5]  * v[1] +
                m[9]  * v[2] +
                m[13];

    result[2] = m[2]  * v[0] +
                m[6]  * v[1] +
                m[10] * v[2] +
                m[14];
}

double PointPlaneDistance(const double p1[3], const double p2[3], const double n[3]) {
    double v[3];
    VectorSubtract(v, p1, p2);

    return VectorDotProduct(v, n);
}


// Random number generator
double Random(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}


// Continuous servo callback function
HDLServoOpExitCode ForceCB(void* userData) {
    // Get pointer to falcon object
    Falcon* falcon = static_cast<Falcon*>(userData);

    // Compute the device force
    falcon->ComputeForce();

    // Make sure to continue processing
    return HDL_SERVOOP_CONTINUE;
}

// Device synchronization callback function
HDLServoOpExitCode SynchronizeCB(void* userData) {
    // Get pointer to falcon object
    Falcon* falcon = static_cast<Falcon*>(userData);

    // Synchronize device state
    falcon->SynchronizeState();

    // Stop
    return HDL_SERVOOP_EXIT;
}


Falcon::Falcon() {
    // Initialize values
    deviceHandle = HDL_INVALID_HANDLE;
    servoOp = HDL_INVALID_HANDLE;

    VectorSet(pos, 0.0, 0.0, 0.0);
    VectorSet(force, 0.0, 0.0, 0.0);
    buttons = 0;

    useForceFeedback = true;
    VectorSet(proxyPos, 0.0, 0.0, 0.0);

    VectorSet(oldPos, 0.0, 0.0, 0.0);
    oldTime = hdluGetSystemTime();
}

Falcon::~Falcon() {    
    // Shutdown HDL
    if (servoOp != HDL_INVALID_HANDLE) {
        hdlDestroyServoOp(servoOp);
        servoOp = HDL_INVALID_HANDLE;
    }

    hdlStop();

    if (deviceHandle != HDL_INVALID_HANDLE) {
        hdlUninitDevice(deviceHandle);
        deviceHandle = HDL_INVALID_HANDLE;
    }
}

bool Falcon::Initialize() {
    // Initialize the device
    deviceHandle = hdlInitNamedDevice("DEFAULT");

    if (deviceHandle == HDL_INVALID_HANDLE) {
        std::cout << "Could not open device" << std::endl;
        return false;
    }

        
    // Now that the device is initialized, start the servo thread.
    hdlStart();

    if (hdlGetError() != HDL_NO_ERROR) {
        std::cout << "Could not start the servo thread" << std::endl;
        return false;
    }

        
    // Set up callback function
    servoOp = hdlCreateServoOp(ForceCB, this, false);
    if (servoOp == HDL_INVALID_HANDLE) {
        std::cout << "Invalid servo op handle" << std::endl;
    }

    if (hdlGetError() != HDL_NO_ERROR) {
        std::cout << "Could not create servo op" << std::endl;
        return false;
    }


    // Make the device current.  All subsequent calls will be directed to the current device.
    hdlMakeCurrent(deviceHandle);

    if (hdlGetError() != HDL_NO_ERROR) {
        std::cout << "Could not make device current" << std::endl;
        return false;
    }


    // Get the extents of the device workspace
    hdlDeviceWorkspace(workspace);

    if (hdlGetError() != HDL_NO_ERROR) {
        std::cout << "Could not get device workspace" << std::endl;
        return false;
    }

    
    // Set default graphics workspace
    Vector3 center = { 0.0, 0.0, 0.0 };
    Vector3 size = { 2.0, 2.0, 2.0 };
    SetGraphicsWorkspace(center, size);


    // Synchronize state
    hdlCreateServoOp(SynchronizeCB, this, true);


    return true;
}


void Falcon::SetGraphicsWorkspace(Vector3 center, Vector3 size) {
    // Graphics workspace should be given as (minx, miny, minz, maxx, maxy, maxz)
    // Flip z here to match Unity
    double graphicsWorkspace[6];
    graphicsWorkspace[0] = center.x - size.x / 2.0;
    graphicsWorkspace[1] = center.y - size.y / 2.0;
    graphicsWorkspace[2] = center.z + size.z / 2.0;
    graphicsWorkspace[3] = center.x + size.x / 2.0;
    graphicsWorkspace[4] = center.y + size.y / 2.0;
    graphicsWorkspace[5] = center.z - size.z / 2.0;

    // Generate the transform from haptic space to graphics space
    bool useUniformScale = false;
    hdluGenerateHapticToAppWorkspaceTransform(workspace,
                                              graphicsWorkspace,
                                              useUniformScale,
                                              haptics2graphics);

	// Create tranform to match direction of graphics space to haptic space for forces
	
	std::cout << std::endl;
	for (int i = 0; i < 16; i++) {
		if (i % 4 == 0) std::cout << std::endl;
		std::cout << haptics2graphics[i] << "\t";
	}

	for (int i = 0; i < 16; i++) {
		// Only use diagonal for scale
		if (i % 4 == i / 4) {
			double v = haptics2graphics[i];
			graphics2haptics[i] = v == 0.0 ? 0.0 : v / fabs(v);
		}
		else {
			graphics2haptics[i] = 0.0;
		}
	}

	std::cout << std::endl;
	for (int i = 0; i < 16; i++) {
		if (i % 4 == 0) std::cout << std::endl;
		std::cout << graphics2haptics[i] << "\t";
	}

    // Synchronize state
    hdlCreateServoOp(SynchronizeCB, this, true);
}


void Falcon::ResetForces() {
	// Remove all force effects
	RemoveSimpleForces();
	RemoveViscosities();
	RemoveSurfaces();
	RemoveSprings();
	RemoveIntermolecularForces();
	RemoveRandomForces();
}


Vector3 Falcon::GetPosition() {
    Vector3 p = { (float)pos[0], (float)pos[1], (float)pos[2] };
    return p;
}

bool Falcon::GetButton(int button) {
    // Bit-shift and mask to get button state
    int b = 1 << button;
    return (buttons & b) == b;
}

Vector3 Falcon::GetForce() {
    Vector3 f = { (float)force[0], (float)force[1], (float)force[2] };
    return f;
}


void Falcon::UseForceFeedback(bool use) { 
    useForceFeedback = use;
}

void Falcon::SetProxyPosition(Vector3 p) {
    VectorSet(proxyPos, p.x, p.y, p.z);
}


// Simple forces
int Falcon::AddSimpleForce(Vector3 f) {
	SimpleForce sf;
    VectorSet(sf.f, f.x, f.y, f.z);

    return simpleForces.Add(sf);
}

void Falcon::UpdateSimpleForce(int i, Vector3 f) {
	SimpleForce* sf = simpleForces.Get(i);
    VectorSet(sf->f, f.x, f.y, f.z);
}

void Falcon::RemoveSimpleForce(int i) {
	simpleForces.Remove(i);
}

void Falcon::RemoveSimpleForces() {
	simpleForces.RemoveAll();
}

// Viscosities
int Falcon::AddViscosity(float c, float w) {
    Viscosity v;
    v.c = c;
    v.w = w;
	VectorSet(v.oldForce, 0.0, 0.0, 0.0);

    return viscosities.Add(v);
}

void Falcon::UpdateViscosity(int i, float c, float w) {
    Viscosity* v = viscosities.Get(i);
    v->c = c;
    v->w = w;
}

void Falcon::RemoveViscosity(int i) {
    viscosities.Remove(i);
}

void Falcon::RemoveViscosities() {
    viscosities.RemoveAll();
}

// Surfaces
int Falcon::AddSurface(Vector3 p, Vector3 n, float k, float c) {
    Surface s;
    s.k = k;
    s.c = c;
    VectorSet(s.p, p.x, p.y, p.z);
    VectorSet(s.n, n.x, n.y, n.z);

    return surfaces.Add(s);
}

void Falcon::UpdateSurface(int i, Vector3 p, Vector3 n, float k, float c) {
    Surface* s = surfaces.Get(i);
    s->k = k;
    s->c = c;
    VectorSet(s->p, p.x, p.y, p.z);
    VectorSet(s->n, n.x, n.y, n.z);
}

void Falcon::RemoveSurface(int i) {
    surfaces.Remove(i);
}

void Falcon::RemoveSurfaces() {
    surfaces.RemoveAll();
}

// Springs
int Falcon::AddSpring(Vector3 p, float k, float c, float r, float m) {
    Spring s;
    s.k = k;
    s.c = c;
    s.r = r;
    s.m = m;
    VectorSet(s.p, p.x, p.y, p.z);

    return springs.Add(s);
}

void Falcon::UpdateSpring(int i, Vector3 p, float k, float c, float r, float m) {
    Spring* s = springs.Get(i);
    s->k = k;
    s->c = c;
    s->r = r;
    s->m = m;
    VectorSet(s->p, p.x, p.y, p.z);
}

void Falcon::RemoveSpring(int i) {
    springs.Remove(i);
}

void Falcon::RemoveSprings() {
    springs.RemoveAll();
}

// Intermolecular forces
int Falcon::AddIntermolecularForce(Vector3 p, float k, float c, float r, float m) {
    IntermolecularForce imf;
    imf.k = k;
    imf.c = c;
    imf.r = r;
    imf.m = m;
    VectorSet(imf.p, p.x, p.y, p.z);

    return intermolecularForces.Add(imf);
}

void Falcon::UpdateIntermolecularForce(int i, Vector3 p, float k, float c, float r, float m) {
    IntermolecularForce* imf = intermolecularForces.Get(i);
    imf->k = k;
    imf->c = c;
    imf->r = r;
    imf->m = m;
    VectorSet(imf->p, p.x, p.y, p.z);
}

void Falcon::RemoveIntermolecularForce(int i) {
    intermolecularForces.Remove(i);
}

void Falcon::RemoveIntermolecularForces() {
    intermolecularForces.RemoveAll();
}

// Random forces
int Falcon::AddRandomForce(float minMag, float maxMag, float minTime, float maxTime) {
    RandomForce rf;
    rf.minMag = minMag;
    rf.maxMag = maxMag;
    rf.minTime = minTime;
    rf.maxTime = maxTime;

    return randomForces.Add(rf);
}

void Falcon::UpdateRandomForce(int i, float minMag, float maxMag, float minTime, float maxTime) {
    RandomForce* rf = randomForces.Get(i);
    rf->minMag = minMag;
    rf->maxMag = maxMag;
    rf->minTime = minTime;
    rf->maxTime = maxTime;
}

void Falcon::RemoveRandomForce(int i) {
    randomForces.Remove(i);
}

void Falcon::RemoveRandomForces() {
    randomForces.RemoveAll();
}


void Falcon::ComputeForce() {
    // Get current state
    SynchronizeState();

    // Get time delta
    double time = hdluGetSystemTime();
    double dt = time - oldTime;

    // Adjusting this affects "kicking" when changing viscosity
 //   if (dt <= 1e-4) return;

    // Set position to use for force calculations
    // If using force feedback, use device position
    // Else use proxy position
    double p[3];
    if (useForceFeedback) {
        VectorCopy(p, pos);
    }
    else {
        VectorCopy(p, proxyPos);
    }

    // Compute current velocity
    double velocity[3];
    VectorSubtract(velocity, p, oldPos);
    VectorScale(velocity, velocity, 1.0 / dt);

    // Initialize force
    VectorSet(force, 0.0, 0.0, 0.0);
    
    // Add simple forces
    for (auto it = simpleForces.Begin(); it != simpleForces.End(); ++it) {
        VectorAdd(force, force, it->second.f);
    }

    // Add viscous forces
    for (auto it = viscosities.Begin(); it != viscosities.End(); ++it) {
        double vf[3];
        ComputeViscousForce(vf, it->second, velocity);
        VectorAdd(force, force, vf);
    }

    // Add surface forces
    for (auto it = surfaces.Begin(); it != surfaces.End(); ++it) {
        double sf[3];
        ComputeSurfaceForce(sf, it->second, velocity);
        VectorAdd(force, force, sf);
    }

    // Add spring forces
    for (auto it = springs.Begin(); it != springs.End(); ++it) {
        double sf[3];
        ComputeSpringForce(sf, it->second, velocity);
        VectorAdd(force, force, sf);
    }
    
    // Add intermolecular forces
    for (auto it = intermolecularForces.Begin(); it != intermolecularForces.End(); ++it) {
        double imf[3];
        ComputeIntermolecularForce(imf, it->second, velocity);
        VectorAdd(force, force, imf);
    }
    
    // Add random forces
    for (auto it = randomForces.Begin(); it != randomForces.End(); ++it) {
        double rf[3];
        ComputeRandomForce(rf, it->second, time);
        VectorAdd(force, force, rf);
    }

	// Tranform force
	MatrixVectorMultiply(force, graphics2haptics, force);
  

    // Set force
    if (useForceFeedback) {
        // Set force
        hdlSetToolForce(force);
    }
    else {
        double f[3];
        VectorSet(f, 0.0, 0.0, 0.0);
        hdlSetToolForce(f);
    }

        
    // Save state
    VectorCopy(oldPos, p);
    oldTime = time;
}


void Falcon::SynchronizeState() {
    // Get current state
    double toolPos[3];
    hdlToolPosition(toolPos);
    MatrixVectorMultiply(pos, haptics2graphics, toolPos);

    hdlToolButtons(&(buttons));
}


void Falcon::ComputeViscousForce(double force[3], Viscosity& v, const double velocity[3]) {
    // Compute viscous force
    VectorScale(force, velocity, -v.c);

    // Interpolate between previous force magnitude and current force magnitude to reduce vibrations
    VectorScale(force, force, v.w);
    VectorScale(v.oldForce, v.oldForce, 1.0 - v.w);
    VectorAdd(force, force, v.oldForce);

    // Save old viscous force
    VectorCopy(v.oldForce, force);
}


void Falcon::ComputeSurfaceForce(double force[3], const Surface s, const double velocity[3]) {
    // Compute distance to proxy position
    double d = PointPlaneDistance(pos, s.p, s.n);

    // Check above or below plane
    if (d > 0.0) {
        return;
    }

    // Compute spring force along surface normal
    VectorScale(force, s.n, -d * s.k);

    // Add damping
    double fd[3];
    VectorScale(fd, velocity, -s.c);

    VectorAdd(force, force, fd);
}

void Falcon::ComputeSpringForce(double force[3], const Spring s, const double velocity[3]) { 
    // Get direction vector from spring to probe
    double dv[3];
    VectorSubtract(dv, pos, s.p);

    // Get distance from spring
    double d = VectorMagnitude(dv);

    // Check for max length
    if (s.m > 0.0 && d > s.m) {
        // Break
        return;
    }

    // Normalize
    VectorNormalize(dv, dv);

    // Distance from rest point
    double dRest = d - s.r;

    // Compute spring force
    VectorScale(force, dv, -dRest * s.k);

    // Add damping
    double fd[3];
    VectorScale(fd, velocity, -s.c);

    VectorAdd(force, force, fd);
}

void Falcon::ComputeIntermolecularForce(double force[3], const IntermolecularForce imf, const double velocity[3]) { 
    // Get direction vector from molecule to probe
    double dv[3];
    VectorSubtract(dv, pos, imf.p);
    dv[2] = 0.0;

    // Get distance from spring
    double d = VectorMagnitude(dv);

    // Normalize
    VectorNormalize(dv, dv);

    // Distance from rest point
    double dRest;

    // Check for max length
    if (d > imf.m) {
        // Fall off linearly by flipping around max length
        dRest = imf.m + imf.m - d - imf.r;
        dRest = dRest < 0.0 ? 0.0 : dRest;
    }
    else {
        // Inside max length
        dRest = d - imf.r;
    }

    // Compute intermolecular force
    VectorScale(force, dv, -dRest * imf.k);

    // Add damping
    double fd[3];
    VectorScale(fd, velocity, -imf.c);

    VectorAdd(force, force, fd);

    force[2] = 0.0;
}

void Falcon::ComputeRandomForce(double force[3], RandomForce& rf, double t) {
    // Check elapsed time
    if (t - rf.tStart > rf.t) {
        // Generate new force
        VectorSet(rf.f, Random(-1.0, 1.0), Random(-1.0, 1.0), Random(-1.0, 1.0));
        VectorNormalize(rf.f, rf.f);
        VectorScale(rf.f, rf.f, Random(rf.minMag, rf.maxMag));

        // Generate new time 
        rf.t = Random(rf.minTime, rf.maxTime);
        rf.tStart = t;
    }

    // Apply force
    VectorCopy(force, rf.f);
}