#include "SpriteSystem.h"

// --------------------------------------------
// Define BaseObject Constructor
BaseObject::BaseObject() {
	position = ofVec3f(0, 0, 0);
	rotation = 0;
}

//--------------------------------------------
// Define BaseObject method setPosition
void BaseObject::setPosition(ofVec3f pos) {
	position = pos;
}

//--------------------------------------------
// Integrator for Trajectory Phyisc of BaseObject
void BaseObject::integrate() {
	// update position from velocity and time
	this->position = this->position + this->velocity * (1.0 / 60.0);
	// update velocity (from acceleration)
	this->velocity = this->velocity + this->acceleration * (1.0 / 60.0);
	// multiply velocity by damping factor
	this->velocity = this->velocity * this->damping;
}

//--------------------------------------------
// Integrator for simple trajectory physics
// handles rotation of BaseObject
void BaseObject::integrateTurn() {
	// update position from turnVelocity and time
	this->rotation = this->rotation + this->turnVelocity * (1.0 / 60.0);
	// update turnVelocity (from turnAcceleration)
	this->turnVelocity = this->turnVelocity + this->turnAcceleration * (1.0 / 60.0);
	// multiply final result by damping factor
	this->turnVelocity = this->turnVelocity * this->damping;
}

//--------------------------------------------
// Define Sprite Constructor
Sprite::Sprite() {
	speed = 0;
	velocity = ofVec3f(0, 0, 0);
	lifespan = -1;					// lifespan of -1 => immortal 
	birthtime = 0;
	haveImage = false;
	name = "UnamedSprite";
	// sets attributes of default image for sprite
	width = 60;
	height = 80;
	spawnTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------
// Return a sprite's age in milliseconds
float Sprite::age() {
	return (ofGetElapsedTimeMillis() - birthtime);
}

//-----------------------------------
// Changes a given sprites velocity
void Sprite::setVelocity(ofVec3f newVelocity)
{
	this->velocity = newVelocity;
}

//--------------------------------------------
// Set an image for the sprite. If you don't set one, a rectangle
// gets drawn.
void Sprite::setImage(ofImage img) {
	image = img;
	haveImage = true;
	width = image.getWidth();
	height = image.getHeight();
}

//--------------------------------------------
// Render the sprite
void Sprite::draw() {

	ofSetColor(255, 255, 255, 255);

	ofPushMatrix();
	ofMultMatrix(getMatrix());
	// draw image centered and add in translation amount
	if (haveImage) {
		image.draw(-width / 2.0, -height / 2.0);
	}
	else {
		// in case no image is supplied, draw something.
		ofSetColor(255, 0, 0);
		ofDrawRectangle(-width / 2.0, -height / 2.0, width, height);
	}
	ofPopMatrix();
}

//--------------------------------------------
// Add a Sprite to the Sprite System
void SpriteSystem::add(Sprite s) {
	sprites.push_back(s);
}

//--------------------------------------------
// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
void SpriteSystem::remove(int i) {
	sprites.erase(sprites.begin() + i);
}

//------------------------------------------------
// Moves all sprites in system
void SpriteSystem::moveSprites()
{
	// iterates through all sprites and moves them
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].position += sprites[i].velocity / ofGetFrameRate();
	}
}

//--------------------------------------------
// Update the SpriteSystem by checking which sprites have exceeded their
// lifespan (and deleting).  Also the sprite is moved to it's next
// location based on velocity and direction.
void SpriteSystem::update() {
	if (sprites.size() == 0) return;
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;

	// check which sprites have exceed their lifespan and delete
	// from list.  When deleting multiple objects from a vector while
	// traversing at the same time, use an iterator.
	while (s != sprites.end()) {
		if (s->lifespan != -1 && s->age() > s->lifespan) {
			tmp = sprites.erase(s);
			s = tmp;
		}
		else s++;
	}

	// Move sprite
	this->moveSprites();
}

//--------------------------------------------
// Remove all sprites within a given distance of point, return true if removed
bool SpriteSystem::collisionKill(ofVec3f point, float dist)
{
	vector<Sprite>::iterator s = sprites.begin();
	vector<Sprite>::iterator tmp;
	int count = 0;

	while (s != sprites.end()) {
		ofVec3f v = s->position - point;
		if (v.length() < dist) {
			tmp = sprites.erase(s);
			s = tmp;
			
			return true;
		}
		else {
			s++;
		}
	}
	return false;
	
}

//--------------------------------------------
// Render all the sprites
void SpriteSystem::draw() {
	for (int i = 0; i < sprites.size(); i++) {
		sprites[i].draw();
	}
}