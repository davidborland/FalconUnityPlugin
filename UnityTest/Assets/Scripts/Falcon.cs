/*=========================================================================

  Name:        Falcon.cs

  Author:      David Borland, RENCI

  Description: Script for loading and interfacing with the Novint Falcon
  			   Unity plugin.

=========================================================================*/


using UnityEngine;
using System.Collections;
using System;
using System.Runtime.InteropServices;


public class Falcon : MonoBehaviour {	
	// Game object whose bounding box is represents the haptic workspace
	public GameObject hapticWorkspace;
	
	// Force to apply
	public Vector3 force;
	
	// Buttons
	public bool[] buttons;
	
	
	// Load functions from DLL
	[DllImport ("FalconUnityPlugin")]
	private static extern bool Initialize();	
	
	[DllImport ("FalconUnityPlugin")]
	private static extern void CleanUp();
	
	[DllImport ("FalconUnityPlugin")]
	private static extern void SetGraphicsWorkspace(Vector3 center, Vector3 size);
	
	[DllImport ("FalconUnityPlugin")]
	private static extern Vector3 GetPosition();
	
	[DllImport ("FalconUnityPlugin")]
	private static extern bool GetButton(int button);
	
	[DllImport ("FalconUnityPlugin")]
	private static extern void SetForce(Vector3 force);
	
	
	void Start() {
		force.Set(0.0f, 0.0f, 0.0f);
		
		buttons = new bool[4] { false, false, false, false };

		if (Initialize()) {
			SetGraphicsWorkspace(hapticWorkspace.collider.bounds.center, 
							 	 hapticWorkspace.collider.bounds.size);
			
			Debug.Log("Falcon success");
		}
		else {
			Debug.Log("Falcon failure");
		}
	}
	
	void OnDestroy() {
		Debug.Log("Falcon cleaned up");	
		CleanUp();
	}
	
	void Update() {
		// Update position here, as we want to read that from the device before performing calculations	
		transform.position = GetPosition();	
		
		// Update buttons
		for (int i = 0; i < buttons.Length; i++) {
			buttons[i] = GetButton(i);	
		}
	}
	
	void LateUpdate() {	
		// Update the force here, as we want to send that to the device after performing calculations
	 	SetForce(force);
	}
}
