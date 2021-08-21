#include "SpriteSystem.h"

/* Emitter class form emitting sprites
   inherits from BaseObject parent class*/
class Emitter : public BaseObject {
public:
	// Constructor
	Emitter(SpriteSystem *);

	// Methods
	void draw();							// Draws the emitter and sliders for various attributes
	void start();							// Sets started field to true so emitter can spawn sprites
	void stop();							// Sets started field to false
	void setLifespan(float);				// Sets lifespan of sprites spawned by emitter
	void setEmitVelocity(ofVec3f);			// Sets emitVelocity of sprites spawned by emitter
	void setChildImage(ofImage);			// Sets image of sprite emitted from emitter
	void setChildSize(float w, float h) { childWidth = w; childHeight = h; }
	void setImage(ofImage);					// Sets image of the emitter itself
	void setRate(float);					// Sets rate of fire of sprites from emitter
	void update();
	bool inside(glm::vec3 currentPosition);	// Checks if the mouse cursor is currently inside the emitter
	void setOneShot(bool s) { oneShot = s; }

	// Fields
	SpriteSystem *sys;		// Holds sprite system of emitter
	float rate;				// Rate of sprites emitted from emitter
	ofVec3f emitVelocity;	// Velocity of sprites emitted from emitter
	float lifespan;			// Lifespan of sprites emitted from emitter
	bool started = false;	// Dictates whether emitter can spawn sprites or not
	float lastSpawned;
	ofImage childImage;		// Holds image of sprites
	ofImage image;			// Holds image of emitter
	bool drawable;			// Dictates visibility of emitter itself
	bool haveChildImage;
	bool haveImage;
	bool canMove = false;	// Dictates if mouse can move emitter
	bool oneShot;
	float width, height;
	float childWidth, childHeight;
	bool hasSound = false;
	void setSound(string sound);
	ofSoundPlayer lazer;	// Holds sound played when emitter spawns new sprite
};
