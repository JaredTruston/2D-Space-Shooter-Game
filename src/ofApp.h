#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Particle.h"
#include "ParticleEmitter.h"

typedef enum { MoveStop, MoveLeft, MoveRight, MoveUp, MoveDown} MoveDir;

class ofApp : public ofBaseApp{

	public:
		// Default methods from ofApp
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		// Check to see if missile hit invader
		void checkCollisions(Emitter *sampleEnemy);
		// Check to see if invader hit player
		void playerHit(Emitter *sampleEnemy);

		// Creates the instance of emitter
		Emitter *turret;
		// Creates enemy emitter
		Emitter *enemy;
		// Creates enemy emitter
		Emitter *enemy2;
		// Sets initial velocity of sprites emitted by turret
		ofVec3f emitVelocity = ofVec3f(0, -200, 0);
		// Creates particle emitter for explosion
		ParticleEmitter partExplosion;
		// Sets Position of Explosion;
		ofVec2f explosionPos;
		// Determines if game can begin or not
		bool standby = true;
		// Determines if game ended or not
		bool gameOver = false;
		// Tracks total initial velocity of turret lazer sprites
		int total_velocity;
		// Creates a keymap to determine if specific keys are pushed/released
		map<int, bool> keymap;
		// Tracks score
		int score;
		int highScore;
		// Tracks range enemy2 sprites should be in to approach player
		int approachRange = 200;
		// Tracks velocity at which enemy2 sprites approach player
		ofVec3f approachVelocity = ofVec3f(100, 100, 0);
		// Holds sound played when sprites collide and are removed
		ofSoundPlayer explosion;	
		// Sets font for displaying text
		ofTrueTypeFont text;

		ofImage defaultImage;
		ofVec3f mouse_last;
		bool imageLoaded;
		bool bHide;

		// Turret sliders
		ofxFloatSlider rate;		// Sets up slider graphic for rate
		ofxFloatSlider life;		// Sets up slider graphic for life

		// Enemy sliders
		ofxFloatSlider enemy1Rate;	// Sets up slider graphic for rate of enemy1 emitter
		ofxFloatSlider enemy1Life;	// Sets up slider graphic for life of enemy1 emitter

		// Enemy2 sliders
		ofxFloatSlider enemy2Rate;	// Sets up slider graphic for rate of enemy2 emitter
		ofxFloatSlider enemy2Life;	// Sets up slider graphic for life of enemy2 emitter

		ofxLabel screenSize;
		ofxPanel gui;				// Sets up panel to display sliders
		
};
