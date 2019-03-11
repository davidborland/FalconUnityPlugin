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

void printUsage(char** argv) {
	printf("Usage: %s -option\n", argv[0]);
	printf("Options:\n");
	printf("\tsimple\n");
	printf("\tviscosity\n");
	printf("\tsurface\n");
	printf("\tspring\n");
	printf("\tintermolecular\n");
	printf("\trandom\n");
}

int main(int argc, char** argv) {
	if (argc != 2) {
		printUsage(argv);
		printf("\nNo option provided, defaulting to simple\n");
	}

	// Initialize Falcon
	Falcon* falcon = new Falcon();
	falcon->Initialize();

	Vector3 center;
	center.x = center.y = center.z = 0.0;

	Vector3 size;
	size.x = size.y = size.z = 10.0;

	falcon->SetGraphicsWorkspace(center, size);

	// Create force effect
	if (argc != 2 || strcmp(argv[1], "-simple") == 0) {
		// Simple force
		Vector3 f;
		f.x = f.z = 0.0;
		f.y = -5.0;

		falcon->AddSimpleForce(f);
	}
	else if (strcmp(argv[1], "-viscosity") == 0) {
		falcon->AddViscosity(0.5);
	}
	else if (strcmp(argv[1], "-surface") == 0) {
		Vector3 p;
		p.x = p.y = p.z = 0.0;

		Vector3 n;
		n.x = n.z = 0.0;
		n.y = 1.0;

		falcon->AddSurface(p, n, 20.0f, 0.01f);
	}
	else if (strcmp(argv[1], "-spring") == 0) {
		Vector3 p;
		p.x = p.y = p.z = 0.0;

		falcon->AddSpring(p, 2.0f, 0.01f);
	}
	else if (strcmp(argv[1], "-intermolecular") == 0) {
		Vector3 p;
		p.x = p.y = p.z = 0.0f;

		falcon->AddIntermolecularForce(p, 10.0f, 0.01f, 2.0f, 4.0f);
	}
	else if (strcmp(argv[1], "-random") == 0) {
		falcon->AddRandomForce(1.0f, 5.0f, 0.01f, 0.1f);
	}
	else {
		printUsage(argv);
		printf("\nInvalid option, defaulting to simple\n");

		// Simple force
		Vector3 f;
		f.x = f.z = 0.0;
		f.y = -5.0;

		falcon->AddSimpleForce(f);
	}

    while (1) {
		Vector3 p = falcon->GetPosition();
        Vector3 f = falcon->GetForce();

		bool b0 = falcon->GetButton(0);
		bool b1 = falcon->GetButton(1);
		bool b2 = falcon->GetButton(2);
		bool b3 = falcon->GetButton(3);
		
		printf("---------------------------------\n");
		printf("Position: %f %f %f\n", p.x, p.y, p.z);
        printf("Force: %f %f %f\n", f.x, f.y, f.z);
		printf("Buttons: %d %d %d %d\n", b0, b1, b2, b3);		
    }

    return 0;
}