/*=========================================================================

  Name:        FalconUnityPlugin.h

  Author:      David Borland, The Renaissance Computing Institute (RENCI)

  Copyright:   The Renaissance Computing Institute (RENCI)

  Description: DLL to enable use of the Novint Falcon within the Unity
               game engine.

=========================================================================*/


#define EXPORT_API __declspec(dllexport)


#include "Falcon.h"


Falcon* falcon = nullptr;


extern "C" {

    bool EXPORT_API Initialize() {  
        if (falcon) {
            // Just in case CleanUp() wasn't called...
            delete falcon;
        }
        
        falcon = new Falcon();
        return falcon->Initialize();
    }

    void EXPORT_API CleanUp() {
        if (falcon) {
            delete falcon;
            falcon = nullptr;
        }
    }

    void EXPORT_API SetGraphicsWorkspace(Vector3 center, Vector3 size) {
        if (falcon) {
            falcon->SetGraphicsWorkspace(center, size);
        }
    }

    Vector3 EXPORT_API GetPosition() {
        if (falcon) {
            return falcon->GetPosition();
        }
        else {
            Vector3 v = { 0.0, 0.0, 0.0 };
            return v;
        }
    }

    Vector3 EXPORT_API GetForce() {
        if (falcon) {
            return falcon->GetForce();
        }
        else {
            Vector3 v = { 0.0, 0.0, 0.0 };
            return v;
        }
    }

    bool EXPORT_API GetButton(int button) {
        if (falcon) {
            return falcon->GetButton(button);
        }
        else {
            return false;
        }
    }

    void EXPORT_API UseForceFeedback(bool use) {
        if (falcon) {
            falcon->UseForceFeedback(use);
        }
    }

    void EXPORT_API SetProxyPosition(Vector3 p) {
        if (falcon) {
            falcon->SetProxyPosition(p);
        }
    }


    // Constant forces
    int EXPORT_API AddConstantForce(Vector3 f) {
        if (falcon) {
            return falcon->AddConstantForce(f);
        }

        return -1;
    }

    void EXPORT_API SetConstantForce(int i, Vector3 f) {
        if (falcon) {
            falcon->SetConstantForce(i, f);
        }
    }

    void EXPORT_API RemoveConstantForce(int i) {
        if (falcon) {
            falcon->RemoveConstantForce(i);
        }
    }

    void EXPORT_API RemoveConstantForces() {
        if (falcon) {
            falcon->RemoveConstantForces();
        }
    }

    // Viscosities
    int EXPORT_API AddViscosity(float c, float w) {
        if (falcon) {
            return falcon->AddViscosity(c, w);
        }

        return -1;
    }

    void EXPORT_API SetViscosity(int i, float c, float w) {
        if (falcon) {
            falcon->SetViscosity(i, c, w);
        }
    }

    void EXPORT_API RemoveViscosity(int i) {
        if (falcon) {
            falcon->RemoveViscosity(i);
        }
    }

    void EXPORT_API RemoveViscosities() {
        if (falcon) {
            falcon->RemoveViscosities();
        }
    }  
    
    // Surfaces
    int EXPORT_API AddSurface(float k, float c, Vector3 p, Vector3 n) {
        if (falcon) {
            return falcon->AddSurface(k, c, p, n);
        }
        
        return -1;
    }

    void EXPORT_API SetSurface(int i, float k, float c, Vector3 p, Vector3 n) {
        if (falcon) {
            falcon->SetSurface(i, k, c, p, n);
        }
    }

    void EXPORT_API RemoveSurface(int i) {
        if (falcon) {
            falcon->RemoveSurface(i);
        }
    }

    void EXPORT_API RemoveSurfaces() {
        if (falcon) {
            falcon->RemoveSurfaces();
        }
    }

    // Springs
    int EXPORT_API AddSpring(float k, float c, float r, float m, Vector3 p) {
        if (falcon) {
            return falcon->AddSpring(k, c, r, m, p);
        }
        
        return -1;
    }

    void EXPORT_API SetSpring(int i, float k, float c, float r, float m, Vector3 p) {
        if (falcon) {
            falcon->SetSpring(i, k, c, r, m, p);
        }
    }
    
    void EXPORT_API RemoveSpring(int i) {
        if (falcon) {
            falcon->RemoveSpring(i);
        }
    }

    void EXPORT_API RemoveSprings() {
        if (falcon) {
            falcon->RemoveSprings();
        }
    }

    // Intermolecular forces
    int EXPORT_API AddIntermolecularForce(float k, float c, float r, float m, Vector3 p) {
        if (falcon) {
            return falcon->AddIntermolecularForce(k, c, r, m, p);
        }
        
        return -1;
    }

    void EXPORT_API SetIntermolecularForce(int i, float k, float c, float r, float m, Vector3 p) {
        if (falcon) {
            falcon->SetIntermolecularForce(i, k, c, r, m, p);
        }
    }
    
    void EXPORT_API RemoveIntermolecularForce(int i) {
        if (falcon) {
            falcon->RemoveIntermolecularForce(i);
        }
    }

    void EXPORT_API RemoveIntermolecularForces() {
        if (falcon) {
            falcon->RemoveIntermolecularForces();
        }
    }

    // Random forces
    int EXPORT_API AddRandomForce(float minMag, float maxMag, float minTime, float maxTime) {
        if (falcon) {
            return falcon->AddRandomForce(minMag, maxMag, minTime, maxTime);
        }
        
        return -1;
    }

    void EXPORT_API SetRandomForce(int i, float minMag, float maxMag, float minTime, float maxTime) {
        if (falcon) {
            falcon->SetRandomForce(i, minMag, maxMag, minTime, maxTime);
        }
    }
    
    void EXPORT_API RemoveRandomForce(int i) {
        if (falcon) {
            falcon->RemoveRandomForce(i);
        }
    }

    void EXPORT_API RemoveRandomForces() {
        if (falcon) {
            falcon->RemoveRandomForces();
        }
    }
}