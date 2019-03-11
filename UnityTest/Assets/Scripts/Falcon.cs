/*=========================================================================

  Name:        Falcon.cs

  Author:      David Borland, RENCI

  Description: Script for loading and interfacing with the Novint Falcon
  			   Unity plugin.

=========================================================================*/


using UnityEngine;
using System.Runtime.InteropServices;


public class Falcon : MonoBehaviour {
	// Position
	public Vector3 position = Vector3.zero;
	
	// Displayed force
	public Vector3 force = Vector3.zero;
	
	// Buttons
	public bool[] buttons;
	
	// Use force feedback or not
	public bool useForceFeedback = true;

	// Load functions from DLL
	[DllImport ("FalconUnityPlugin")]
	private static extern bool Initialize();	
	
	[DllImport ("FalconUnityPlugin")]
	private static extern void CleanUp();
	
	[DllImport ("FalconUnityPlugin")]
	private static extern void SetGraphicsWorkspace(Vector3 center, Vector3 size);

	[DllImport ("FalconUnityPlugin")]
	private static extern void ResetForces();
	
	[DllImport ("FalconUnityPlugin")]
	private static extern Vector3 GetPosition();

	[DllImport ("FalconUnityPlugin")]
	private static extern Vector3 GetForce();
	
	[DllImport ("FalconUnityPlugin")]
	private static extern bool GetButton(int button);

	[DllImport ("FalconUnityPlugin")]
	private static extern bool UseForceFeedback(bool use);

	// Proxy position

	[DllImport ("FalconUnityPlugin")]
	public static extern bool SetProxyPosition(Vector3 p);

	// Simple forces

	[DllImport ("FalconUnityPlugin")]
	public static extern int AddSimpleForce(Vector3 force);
	
	[DllImport ("FalconUnityPlugin")]
	public static extern void UpdateSimpleForce(int i, Vector3 force);

	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveSimpleForce(int i);

	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveSimpleForces();

	// Viscosities

	[DllImport ("FalconUnityPlugin")]
	public static extern int AddViscosity(float c, float w);

	[DllImport ("FalconUnityPlugin")]
	public static extern void UpdateViscosity(int i, float c, float w);

	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveViscosity(int i);
	
	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveViscosities();

	// Surfaces
	
	[DllImport ("FalconUnityPlugin")]
	public static extern int AddSurface(Vector3 p, Vector3 n, float k, float c);
		
	[DllImport ("FalconUnityPlugin")]
	public static extern void UpdateSurface(int i, Vector3 p, Vector3 n, float k, float c);

	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveSurface(int i);
	
	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveSurfaces();

	// Springs
	
	[DllImport ("FalconUnityPlugin")]
	public static extern int AddSpring(Vector3 p, float k, float c, float r, float m);

	[DllImport ("FalconUnityPlugin")]
	public static extern void UpdateSpring(int i, Vector3 p, float k, float c, float r, float m);

	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveSpring(int i);

	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveSprings();

	// Intermolecular forces
	
	[DllImport ("FalconUnityPlugin")]
	public static extern int AddIntermolecularForce(Vector3 p, float k, float c, float r, float m);
	
	[DllImport ("FalconUnityPlugin")]
	public static extern void UpdateIntermolecularForce(int i, Vector3 p, float k, float c, float r, float m);
	
	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveIntermolecularForce(int i);
	
	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveIntermolecularForces();

	// Random forces
		
	[DllImport ("FalconUnityPlugin")]
	public static extern int AddRandomForce(float minMag, float maxMag, float minTime, float maxTime);
	
	[DllImport ("FalconUnityPlugin")]
	public static extern void UpdateRandomForce(int i, float minMag, float maxMag, float minTime, float maxTime);

	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveRandomForce(int i);

	[DllImport ("FalconUnityPlugin")]
	public static extern void RemoveRandomForces();	
	
	void Awake() {		
		// Initialize buttons
		buttons = new bool[] { false, false, false, false };

		if (Initialize()) {
			Renderer renderer = GetComponent<Renderer> ();
			SetGraphicsWorkspace(renderer.bounds.center, 
			                     renderer.bounds.size);

			UpdateState();

			UseForceFeedback(useForceFeedback);

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
	
	void FixedUpdate() {
		UpdateState();
	}

	void UpdateState() {
		// Update position
		position = GetPosition();
		
		// Update force
		force = GetForce();
		
		// Update buttons
		for (int i = 0; i < buttons.Length; i++) {
			buttons[i] = GetButton(i);	
		}
	}
}
