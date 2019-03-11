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

	void EXPORT_API ResetForces() {
		if (falcon) {
			falcon->ResetForces();
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


    // Simple forces
    int EXPORT_API AddSimpleForce(Vector3 f) {
        if (falcon) {
            return falcon->AddSimpleForce(f);
        }

        return -1;
    }

    void EXPORT_API UpdateSimpleForce(int i, Vector3 f) {
        if (falcon) {
            falcon->UpdateSimpleForce(i, f);
        }
    }

    void EXPORT_API RemoveSimpleForce(int i) {
        if (falcon) {
            falcon->RemoveSimpleForce(i);
        }
    }

    void EXPORT_API RemoveSimpleForces() {
        if (falcon) {
            falcon->RemoveSimpleForces();
        }
    }

    // Viscosities
    int EXPORT_API AddViscosity(float c, float w) {
        if (falcon) {
            return falcon->AddViscosity(c, w);
        }

        return -1;
    }

    void EXPORT_API UpdateViscosity(int i, float c, float w) {
        if (falcon) {
            falcon->UpdateViscosity(i, c, w);
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
    int EXPORT_API AddSurface(Vector3 p, Vector3 n, float k, float c) {
        if (falcon) {
            return falcon->AddSurface(p, n, k, c);
        }
        
        return -1;
    }

    void EXPORT_API UpdateSurface(int i, Vector3 p, Vector3 n, float k, float c) {
        if (falcon) {
            falcon->UpdateSurface(i, p, n, k, c);
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
    int EXPORT_API AddSpring(Vector3 p, float k, float c, float r, float m) {
        if (falcon) {
            return falcon->AddSpring(p, k, c, r, m);
        }
        
        return -1;
    }

    void EXPORT_API UpdateSpring(int i, Vector3 p, float k, float c, float r, float m) {
        if (falcon) {
            falcon->UpdateSpring(i, p, k, c, r, m);
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
    int EXPORT_API AddIntermolecularForce(Vector3 p, float k, float c, float r, float m) {
        if (falcon) {
            return falcon->AddIntermolecularForce(p, k, c, r, m);
        }
        
        return -1;
    }

    void EXPORT_API UpdateIntermolecularForce(int i, Vector3 p, float k, float c, float r, float m) {
        if (falcon) {
            falcon->UpdateIntermolecularForce(i, p, k, c, r, m);
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

    void EXPORT_API UpdateRandomForce(int i, float minMag, float maxMag, float minTime, float maxTime) {
        if (falcon) {
            falcon->UpdateRandomForce(i, minMag, maxMag, minTime, maxTime);
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