#include "ofMain.h"

/* Base object that all drawable objects will inherit from */
class BaseObject {
public:
	// Constructor
	BaseObject();

	// Fields
	glm::vec3 position;		// Holds position of specific Base object
	float rotation = 0.0;	// Holds rotation of specific Base object

	// Methods
	// Returns transformation matrix of current Base object
	glm::mat4 getMatrix() {
		// Translates position of current Base object
		glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(position));
		// Rotates current Base object
		glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));

		return (trans * rot);
	}
	// Returns heading of current Base object
	glm::vec3 heading() {
		glm::vec3 initialHeading = glm::vec3(0, -1, 0);	// Heading at start
		glm::mat4 Mrot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
		glm::vec3 h = Mrot * glm::vec4(initialHeading, 1);
		return glm::normalize(h);
	}
	// Sets position of current Base object
	void setPosition(ofVec3f);

	// Integrator Functions;
	//
	void integrate();			// Movement forward and backward relative to heading
	void integrateTurn();		// Turns BaseObject instance clockwise and counter-clockwise

	// Physics data for movement of BaseObjects
	//
	glm::vec3 velocity;			// velocity of BaseObject
	glm::vec3 acceleration;		// acceleration of BaseObject
	float thrust;				// thrust force value
	float turnVelocity;			// velocity of BaseObject's rotation
	float turnAcceleration;		// acceleration of BaseObject
	float damping;				// damping factor to slow down BaseObject
};

/* General Sprite Class */
class Sprite : public BaseObject {
public:
	// Constructor
	Sprite();

	// Methods
	void draw();								// Draws the sprite	
	float age();								// Holds age of sprite
	void setVelocity(ofVec3f newVelocity);		// Changes a given sprites velocity
	void setImage(ofImage);						// Sets the image of the sprite

	// Fields
	float speed;			// In pixels per second
	ofImage image;			// Holds image of sprite if there is one
	float birthtime;		// Elapsed time in ms
	float lifespan;			// Time in ms
	string name;			// Holds name of sprite
	bool haveImage;			// Indicates if sprite has image
	float width, height;	// Holds width and height of sprite
	float spawnTime;		// Tracks time when sprite was spawned
};

/* Manages all Sprites in a system */
class SpriteSystem {
public:
	// Methods
	void add(Sprite);		// Adds a sprite to the sprite system
	void remove(int);		// Removes a sprite from the sprite system
	void moveSprites();		// Iterates through sprites and moves them all based on velocity
	void update();
	// delete sprites that are within given distance
	bool collisionKill(ofVec3f point, float dist);
	void draw();			// Draws all sprites of sprite system

	// Field
	vector<Sprite> sprites;	// Holds all sprites of sprite system
};
