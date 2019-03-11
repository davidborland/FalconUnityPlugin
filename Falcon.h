/*=========================================================================

  Name:        Falcon.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: Class to encapsulate the Novint Falcon device.

=========================================================================*/


#ifndef FALCON_H
#define FALCON_H


#include <hdl/hdl.h>

#include <unordered_map>
#include <vector>

#include "ForceContainer.h"


// Struct to use for sending info between the plugin and Unity
struct Vector3 {
    float x;
    float y;
    float z;
};


// Struct for simple force
struct SimpleForce {
    double f[3];
};

// Struct for viscosity
struct Viscosity {
    // Parameters
    double c;
    double w;

    // State
    double oldForce[3];
};

// Struct for surface
struct Surface {
    // Parameters
    double k;
    double c;
    double p[3];
    double n[3];
};

// Struct for spring
struct Spring {
    // Parameters
    double k;
    double c;
    double r;
    double m;
    double p[3];
};

// Struct for intermolecular force
struct IntermolecularForce {
    // Parameters
    double k;
    double c;
    double r;
    double m;
    double p[3];
};

// Struct for random force
struct RandomForce {
    // Parameters
    double minMag;
    double maxMag;
    double minTime;
    double maxTime;

    // State
    double f[3];
    double t;
    double tStart;
};

// The class encapsulating the Falcon device
class Falcon {
public:
    Falcon();
    virtual ~Falcon();

    // Initialize HDL library
    bool Initialize();

    // Set the workspace of the graphics scene that will be mapped to the device workspace
    void SetGraphicsWorkspace(Vector3 center, Vector3 size);

	// Reset forces
	void ResetForces();


    // Get the device position
    Vector3 GetPosition();

    // Get the displayed force
    Vector3 GetForce();

    // Get the device buttons
    bool GetButton(int button);


    // Use force feedback or not
    void UseForceFeedback(bool use);

    // Set the proxy position, to use for calculating forces when not using force feedback
    void SetProxyPosition(Vector3 p);


    // Simple forces
    int AddSimpleForce(Vector3 f);
    void UpdateSimpleForce(int i, Vector3 f);
    void RemoveSimpleForce(int i);
    void RemoveSimpleForces();

    // Viscosities
    // c: Damping coefficient
    // w: Heuristic weighting factor in the range 0 to 1 used to interpolate
    //    between the previous viscous force and the current viscous force to reduce vibration. 
    //    A value of 0 will give no weight to the current viscous force, and 1 will give full weight.
    int AddViscosity(float c, float w = 0.25f);
    void UpdateViscosity(int i, float c, float w = 0.25f);
    void RemoveViscosity(int i);
    void RemoveViscosities();

    // Contact surfaces
    // p: Surface contact point. Using the center of the haptic proxy results in a dilation of the surface by the proxy radius.
    // n: Surface normal
    // k: Spring constant
    // c: Damping coefficient
    int AddSurface(Vector3 p, Vector3 n, float k, float c);
    void UpdateSurface(int i, Vector3 p, Vector3 n, float k, float c);
    void RemoveSurface(int i);
    void RemoveSurfaces();

    // Springs
	// p: Position
    // k: Spring constant
    // c: Damping coefficient
    // r: Rest length
    // m: Maximum length. Negative value for no maximum.
    int AddSpring(Vector3 p, float k, float c, float r = 0.0f, float m = -1.0f);
    void UpdateSpring(int i, Vector3 p, float k, float c, float r = 0.0f, float m = -1.0f);
    void RemoveSpring(int i);
    void RemoveSprings();

    // Intermolecular forces
    // Simulating using a linear spring with bond equilibrium length equal to rest length. Instead of breaking at maximum length, however, 
    // the force curve is mirrored to achieve a continuous falloff, and thus a linear approximation of an intermolecular force curve:
    //
    // \
    //  \
    // __\_______
    //   |\    /
    //   | \  /
    //   |  \/
    //   |   | 
    //   r   m
    //
	// p: Position
    // k: Spring constant
    // c: Damping coefficient
    // r: Bond length
    // m: Maximum length, beyond which bond is "broken"
    int AddIntermolecularForce(Vector3 p, float k, float c, float r, float m);
    void UpdateIntermolecularForce(int i, Vector3 p, float k, float c, float r, float m);
    void RemoveIntermolecularForce(int i);
    void RemoveIntermolecularForces();

    // Random forces
    // minMag: Minimum force magnitude
    // maxMag: Maximum force magnitude
    // minTime: Minimum time interval to apply force
    // maxTime: Maximum time interval to apply force
    int AddRandomForce(float minMag, float maxMag, float minTime, float maxTime);
    void UpdateRandomForce(int i, float minMag, float maxMag, float minTime, float maxTime);
    void RemoveRandomForce(int i);
    void RemoveRandomForces();

protected:    
    // Define callback functions as friends
    friend HDLServoOpExitCode ForceCB(void* userData);
    friend HDLServoOpExitCode SynchronizeCB(void* userData);


    // Device information
    double pos[3];
    double force[3];
    int buttons;


    // Non-force-feedback mode
    bool useForceFeedback;
    double proxyPos[3];


    // For velocity calculation
    double oldPos[3]; 
    double oldTime;


    // Haptic effects 
    ForceContainer<SimpleForce> simpleForces;
    ForceContainer<Viscosity> viscosities;
    ForceContainer<Surface> surfaces;
    ForceContainer<Spring> springs;
    ForceContainer<IntermolecularForce> intermolecularForces;
    ForceContainer<RandomForce> randomForces;


    // Device workspace dimensions
    double workspace[6];

    // Transforms
    double haptics2graphics[16];
	double graphics2haptics[16];


    // Handle to device
    HDLDeviceHandle deviceHandle;

    // Handle to haptic callback 
    HDLServoOpExitCode servoOp;


    // Compute device force, called from force callback
    void ComputeForce();

    // Synchronize device state
    void SynchronizeState();


    // Compute viscous force
    void ComputeViscousForce(double force[3], Viscosity& v, const double velocity[3]);

    // Compute surface force
    void ComputeSurfaceForce(double force[3], const Surface s, const double velocity[3]);
    
    // Compute spring force
    void ComputeSpringForce(double force[3], const Spring s, const double velocity[3]);

    // Compute intermolecular force
    void ComputeIntermolecularForce(double force[3], const IntermolecularForce imf, const double velocity[3]);

    // Compute random force
    void ComputeRandomForce(double force[3], RandomForce& r, double t);
};

#endif