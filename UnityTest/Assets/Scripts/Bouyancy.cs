/*=========================================================================

  Name:        Bouyancy.cs

  Author:      David Borland, RENCI

  Description: Simple demonstration of computing the bouyant force for a 
  			   probe object interacting with a liquid object.  Density is 
  			   assumed to be in g/cm^3 and distances in meters.  The 
  			   probe and liquid objects are assumed to be rectangular 
  			   prisms, such that their axis-aligned bounding boxes 
  			   accurately represent their shape.

=========================================================================*/


using UnityEngine;
using System.Collections;


public class Bouyancy : MonoBehaviour {	
	// The liquid object
	public GameObject liquid;
	
	// Probe density in g/cm^3
	public float probeDensity;
	
	// Liquid density in g/cm^3
	public float liquidDensity;
	
	// Gravity
	private const float g = 9.80665f;
	
	// Liquid size
	private Vector3 liquidScale;
	private Vector3 liquidPosition;
	
	
	void Start() {
		// Store the initial scale and position
		liquidScale = liquid.transform.localScale;
		liquidPosition = liquid.transform.position;
	}

	
	void Update() {
		// Reference to the Falcon object
		Falcon falcon = gameObject.GetComponent<Falcon>();
		
		// Compute the weight of the probe object
		float force = -Weight(probeDensity, gameObject.collider.bounds.size);
		
		if (collider.bounds.Intersects(liquid.collider.bounds)) {
			// Compute the displaced volume of liquid
			Vector3 displacedVolume = DisplacedVolume();
			
			// Add the bouyant force, which is the weight of the displaced volume
			force += Weight(liquidDensity, displacedVolume);
			
			// Represent the displaced volume graphically
			DisplaceLiquid(displacedVolume);			
		}
		else {				
			// No intersection, so make sure scale and position are reset
			liquid.transform.localScale = liquidScale;
			liquid.transform.position = liquidPosition;	
		}
		
		// Only apply forces if middle button is down
		if (falcon.buttons[0]) {
			falcon.force.Set(0.0f, force, 0.0f);
		}
		else {
			falcon.force.Set(0.0f, 0.0f, 0.0f);	
		}
	}
	
	Vector3 DisplacedVolume() {
		// Get bounding boxes
		Bounds bbProbe = gameObject.collider.bounds;
		Bounds bbLiquid = liquid.collider.bounds;
		
		// Compute how submerged the probe is
		float under = Mathf.Min(bbLiquid.max.y - bbProbe.min.y, bbProbe.size.y);	
		
		// Compute the volume of the liquid displaced by the probe
		return new Vector3(bbProbe.size.x, under, bbProbe.size.z);
	}
	
	void DisplaceLiquid(Vector3 volume) {			
		// Get bounding box
		Bounds bbLiquid = liquid.collider.bounds;
		
		// Compute the offset		
		float area = bbLiquid.size.x * bbLiquid.size.z;
		float offset = volume.x * volume.y * volume.z / area;
		
		// First scale vertically to match the offset
		liquid.transform.localScale = liquidScale;
		float s = (bbLiquid.size.y + offset) / bbLiquid.size.y;		
		
		Vector3 scale = liquidScale;
		scale.y *= s;
		liquid.transform.localScale = scale;
		
		// Then translate vertically so the base of the liquid stays fixed,
		// assuming object origin is in the center
		Vector3 position = liquidPosition;
		position.y += offset / 2;
		liquid.transform.position = position;
	}
	
	
	static float Weight(float density, Vector3 volume) {		
		// Compute the mass in kg (density is given in g/cm^3, volume in meters)
		float mass = density * 1000.0f * volume.x * volume.y * volume.z;
		
		// Return the weight in N = kg m/s^2		
		return mass * g;
	}
}