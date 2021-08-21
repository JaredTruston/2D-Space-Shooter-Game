#include "ofMain.h"
#include "Emitter.h"

/*  Create a new Emitter - needs a SpriteSystem */
Emitter::Emitter(SpriteSystem *spriteSys) {
	sys = spriteSys;
	lifespan = 3000;					// milliseconds
	started = false;
	oneShot = false;
	lastSpawned = 0;
	rate = 1;							// sprites/sec
	haveChildImage = false;
	haveImage = false;
	emitVelocity = ofVec3f(100, 100, 0);
	drawable = true;
	width = 50;
	height = 50;
}

/*  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter */
void Emitter::draw() {
	if (drawable) {
		// Add matrix transformations for emitter
		ofPushMatrix();
		ofMultMatrix(getMatrix());
		if (haveImage) {
			image.draw(-image.getWidth() / 2.0, -image.getHeight() / 2.0);
		}
		else {
			ofSetColor(0, 0, 200);
			ofDrawRectangle(-width / 2, -height / 2, width, height);
		}
		ofPopMatrix();
	}

	// draw sprite system
	sys->draw();
}

/*  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
*/
void Emitter::update() {

	// Update the emitter even if started isn't false so sprites still move
	if (!started)
		sys->update();
	else {
		float time = ofGetElapsedTimeMillis();
		if (oneShot) {
			// spawn a new sprite
			Sprite sprite;
			if (haveChildImage) sprite.setImage(childImage);
			sprite.velocity = emitVelocity;
			sprite.lifespan = lifespan;
			sprite.setPosition(ofVec3f(position[0] + (image.getWidth() / 2.0 * this->heading()[0]), position[1] + (image.getWidth() / 2.0 * this->heading()[1]), position[2]));
			sprite.rotation = this->rotation;
			sprite.birthtime = time;
			sprite.width = childWidth;
			sprite.height = childHeight;
			if (this->hasSound)
				lazer.play();
			sys->add(sprite);
			lastSpawned = time;
			stop();
		}
		else if ((time - lastSpawned) > (1000.0 / rate)) {
			// spawn a new sprite
			Sprite sprite;
			if (haveChildImage) sprite.setImage(childImage);
			sprite.velocity = emitVelocity;
			sprite.lifespan = lifespan;
			// Set position of sprite a bit in front of emitter
			sprite.setPosition(ofVec3f(position[0] + (image.getWidth() / 2.0 * this->heading()[0]), position[1] + (image.getWidth()/2.0 * this->heading()[1]), position[2]));
			sprite.rotation = this->rotation;
			sprite.birthtime = time;
			sprite.width = childWidth;
			sprite.height = childHeight;
			if (this->hasSound)
				lazer.play();
			sys->add(sprite);
			lastSpawned = time;
		}
		sys->update();
	}
}

/* Checks if a given point is within the emitter */
bool Emitter::inside(glm::vec3 currentPosition)
{
	float emMaxWidth = position[0] + (1.0 / 2.0) * (image.getWidth());
	float emMinWidth = position[0] - (1.0 / 2.0) * (image.getWidth());
	float emMaxHeight = position[1] + (1.0 / 2.0) * (image.getHeight());
	float emMinHeight = position[1] - (1.0 / 2.0) * (image.getHeight());

	if (currentPosition[0] < emMaxWidth && currentPosition[0] > emMinWidth && currentPosition[1] < emMaxHeight && currentPosition[1] > emMinHeight)
		return true;
	else
		return false;
}

void Emitter::setSound(string sound)
{
	lazer.load(sound);
	this->hasSound = true;
}

/* Starts the emitter. */
void Emitter::start() {
	started = true;
	lastSpawned = ofGetElapsedTimeMillis();
}

/* Stops the emitter. */
void Emitter::stop() {
	started = false;
}

/* Sets lifespan of emitter */
void Emitter::setLifespan(float life) {
	lifespan = life;
}

/* Sets emitVelocity of the emitter */
void Emitter::setEmitVelocity(ofVec3f v) {
	emitVelocity = v;
}

/* Set child image of emitter*/
void Emitter::setChildImage(ofImage img) {
	childImage = img;
	haveChildImage = true;
	childWidth = img.getWidth();
	childHeight = img.getHeight();
}

/* Sets image of emitter itself */
void Emitter::setImage(ofImage img) {
	image = img;
}

/* Sets rate of fire for emitter */
void Emitter::setRate(float r) {
	rate = r;
}