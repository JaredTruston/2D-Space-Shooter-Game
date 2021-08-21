#include "ofApp.h"

//--------------------------------------------------------------
// Sets up attributes of turret, enemy1, and enemy2 emitter
// Sets up gui displaying attributes of emitters
void ofApp::setup() {
	// Loads explosion sound effect
	explosion.load("sounds/Explosion.mp3");
	// Initializes score to 0
	score = 0;
	// Enables vertical sync
	ofSetVerticalSync(true);
	// Loads the font
	text.loadFont("AmaticSC-Regular.ttf", 30);

	// Loads lazer image spawned by turret emitter
	if (defaultImage.load("images/lazer.png")) {
		imageLoaded = true;
	}
	else {
		ofLogFatalError("can't load image: images/lazer.png");
		ofExit();
	}
	// Creates emitter instance and stores it in turret;
	turret = new Emitter(new SpriteSystem());
	// Sets position of turret emitter
	turret->setPosition(ofVec3f(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0));
	// Sets Thrust of turret
	turret->thrust = 500;
	// Sets Damping of turret
	turret->damping = 0.99;
	// Sets initial acceleration of turret
	turret->acceleration = glm::vec3(0,0,0);
	// Sets initial velocity of turret
	turret->velocity = glm::vec3(0, 0, 0);
	// Makes turret emitter visible
	turret->drawable = true;
	// Make Turret one shot
	turret->setOneShot(true);
	// Loads an image for turret emitter
	if (turret->image.load("images/turret.png")) {
		turret->haveImage = true;
	}
	else {
		ofLogFatalError("can't load image: images/turret.png");
		ofExit();
	}
	// Sets image of the turret emitter itself
	turret->setImage(turret->image);		
	// Sets image emitted from turret
	turret->setChildImage(defaultImage);
	// Sets sound played when turret fires
	turret->setSound("sounds/lazer.mp3");

	// Sets up sliders for turret emitter settings
	gui.setup();
	gui.add(rate.setup("turret rate", 1, 1, 10));
	gui.add(life.setup("turret life", 5, .1, 10));
	// Sets up sliders for enemy1 emitter settings
	gui.add(enemy1Rate.setup("enemy1 rate", 1, 1, 10));
	gui.add(enemy1Life.setup("enemy1 life", 10, .1, 10));
	// Sets up sliders for enemy2 emitter settings
	gui.add(enemy2Rate.setup("enemy2 rate", 1, 1, 10));
	gui.add(enemy2Life.setup("enemy2 life", 10, .1, 10));
	bHide = false;

	// Records initial emit velocity of turret
	total_velocity = emitVelocity.x + emitVelocity.y;

	// Loads the image for sprites being spawned by enemy emitter
	if (defaultImage.load("images/badguy.png")) {
		imageLoaded = true;
	}
	else {
		ofLogFatalError("can't load image: images/space-invaders-ship-scaled.png");
		ofExit();
	}
	// Instantiates enemy emitter
	enemy = new Emitter(new SpriteSystem());
	// Sets position of enemy emitter
	enemy->setPosition(ofVec3f(0, ofGetWindowHeight() / 2.0, 0));
	// Makes enemy emitter invisible
	enemy->drawable = false;
	// Sets image of enemy emitter
	enemy->setChildImage(defaultImage);

	// Loads the image for sprites being spawned by enemy2 emitter
	if (defaultImage.load("images/badguy2.png")) {
		imageLoaded = true;
	}
	else {
		ofLogFatalError("can't load image: images/space-invaders-ship-scaled.png");
		ofExit();
	}
	// Instantiates enemy2 emitter
	enemy2 = new Emitter(new SpriteSystem());
	// Sets position of enemy2 emitter
	enemy2->setPosition(ofVec3f(ofGetWindowWidth(), ofGetWindowHeight() / 2.0, 0));
	// Makes enemy2 emitter invisible
	enemy2->drawable = false;
	// Sets image of enemy2 emitter
	enemy2->setChildImage(defaultImage);
	// Sets initial emit velocity of enemy2 emitter
	enemy2->setEmitVelocity(ofVec3f(-100, 0, 0));

	// Sets attributes of particle explosion
	partExplosion.setOneShot(true);
	partExplosion.setEmitterType(RadialEmitter);
	partExplosion.setGroupSize(50);
	partExplosion.setLifespan(0.0625);
	partExplosion.setVelocity(ofVec3f(1000, 0, 0));
	partExplosion.setParticleRadius(2);
}

//--------------------------------------------------------------
// Updates the emitter with values of sliders
void ofApp::update() {
	// Updates fields of turret emitter
	turret->setRate(rate);
	turret->setLifespan(life * 1000);    // convert to milliseconds
	turret->setEmitVelocity(ofVec3f(emitVelocity.x, emitVelocity.y, emitVelocity.z));
	// Handles movement of turret emitter
	turret->integrate();
	turret->integrateTurn();
	turret->update();
	// When moves off edge of screen, appears on other side
	if (turret->position.x > ofGetWindowWidth())
		turret->setPosition(ofVec3f(0, turret->position.y, 0));
	if (turret->position.x < 0)
		turret->setPosition(ofVec3f(ofGetWindowWidth(), turret->position.y, 0));
	if (turret->position.y > ofGetWindowHeight())
		turret->setPosition(ofVec3f(turret->position.x, 0, 0));
	if (turret->position.y < 0)
		turret->setPosition(ofVec3f(turret->position.x, ofGetWindowHeight(), 0));
	// Sets emitVelocity direction to direction of turret rotation
	emitVelocity = ofVec3f(-total_velocity * sin(ofDegToRad(turret->rotation)), total_velocity*cos(ofDegToRad(turret->rotation)), emitVelocity.z);

	// Updates fields of enemy emitter
	enemy->setRate(enemy1Rate);
	enemy->setLifespan(enemy1Life * 10000);
	enemy->setEmitVelocity(ofVec3f(10, 100, 0));
	// Changes velocity of each sprite spawned by enemy emitter after specified amount of time
	for (int i = 0; i < enemy->sys->sprites.size(); i++) {
		if ((ofGetElapsedTimeMillis() - enemy->sys->sprites[i].spawnTime > 5 * 1000)) {
			// Resets spawn time to current time
			enemy->sys->sprites[i].spawnTime = ofGetElapsedTimeMillis();
			// Records previous velocity, modifies it, and sets new velocity
			ofVec3f prevVelocity = enemy->sys->sprites[i].velocity;
			ofVec3f newVelocity = ofVec3f(prevVelocity.x*1.55, -1*prevVelocity.y*1.5, prevVelocity.z);
			enemy->sys->sprites[i].setVelocity(newVelocity);
		}
	}
	// Updates enemy emitter
	enemy->update();
	// Handles collisions between enemy sprite and lazer sprite/turret emitter
	checkCollisions(enemy);
	playerHit(enemy);

	// Start enemy2 emitter if score exceeds 10
	if (score == 10 && enemy2->started == false) {
		enemy2->start();
		approachRange = 200;
		approachVelocity = ofVec3f(100, 100, 0);
	}
	// Increase approachRange and approachVelocity as score increases
	if (score == 20) {
		approachRange = 300;
		approachVelocity = ofVec3f(200, 200, 0);
	}
	if (score == 30) {
		approachRange = 400;
		approachVelocity = ofVec3f(300, 300, 0);
	}
	if (score == 40) {
		approachRange = 500;
		approachVelocity = ofVec3f(400, 400, 0);
	}
	// Updates fields of enemy2 emitter
	enemy2->setRate(enemy2Rate);
	enemy2->setLifespan(enemy2Life * 2000);
	// Sets random direction of movement for enemy2 emitter sprites
	ofVec3f tempE2Vel = enemy2->emitVelocity;
	enemy2->setEmitVelocity(ofVec3f(-100, ofRandom(-tempE2Vel.x/2, tempE2Vel.x/2), 0));
	// Sets random new position of enemy2 emitter
	ofVec3f tempE2Pos = enemy2->position;
	enemy2->setPosition(ofVec3f(tempE2Pos.x, ofRandom(100, ofGetWindowHeight()-100), tempE2Pos.z));
	// Rotates enemy2 ships towards turret and moves them along headings
	for (int i = 0; i < enemy2->sys->sprites.size(); i++) {
		float distance = sqrtf(pow(turret->position.x - enemy2->sys->sprites[i].position.x, 2) + pow(turret->position.y - enemy2->sys->sprites[i].position.y, 2));
		// Trigger rotation once distance between turret and enemy2 sprite is within threshold
		if (distance <= approachRange) {
			if (turret->position.x - enemy2->sys->sprites[i].position.x < 0)
				enemy2->sys->sprites[i].rotation = ofRadToDeg(acos((turret->position.y - enemy2->sys->sprites[i].position.y) / distance));
			else
				enemy2->sys->sprites[i].rotation = -ofRadToDeg(acos((turret->position.y - enemy2->sys->sprites[i].position.y) / distance));
			enemy2->sys->sprites[i].rotation += 180;
			enemy2->sys->sprites[i].setVelocity(approachVelocity * enemy2->sys->sprites[i].heading());
		}
	}
	// Updates enemy2 emitter
	enemy2->update();
	// Handles collisions between enemy2 sprite and lazer sprite/turret emitter
	checkCollisions(enemy2);
	playerHit(enemy2);

	// Updates the particle explosion
	partExplosion.update();
}

//--------------------------------------------------------------
// Draws turret, enemy, and enemy2 emitters
// Displays score
void ofApp::draw() {
	// Standby variable dictates if user has started game yet
	if (standby == false) {
		// Draws emitters
		ofSetColor(255, 255, 255);
		turret->draw();
		enemy->draw();
		enemy2->draw();
		// Hides gui sliders
		if (bHide) {
			gui.draw();
		}
		// Displays score on screen
		string scoreText;
		ofSetColor(ofColor::yellow);
		scoreText += "Score: " + std::to_string(score);
		//ofDrawBitmapString(scoreText, ofPoint(10, 15));
		text.drawString(scoreText, 7, 35);
	}
	else {
		// Draws directions for starting game on screen
		ofSetColor(ofColor::yellow);
		string instructionText = "Press the Space-bar to begin";
		text.drawString(instructionText, ofGetWindowWidth() / 2 - text.stringWidth(instructionText)/2, ofGetWindowHeight() / 2);
		instructionText = "\nControls: Attack with Space-bar and Move with Arrow-keys or WASD";
		text.drawString(instructionText, ofGetWindowWidth() / 2 - text.stringWidth(instructionText)/2, ofGetWindowHeight() / 2);
	}

	// Draws Particle Explosion
	ofPushMatrix();
	ofTranslate(explosionPos);
	partExplosion.draw();
	ofPopMatrix();

	// Ends Game
	if (gameOver) {
		// Removes all turret sprites
		for (int i = 0; i < turret->sys->sprites.size(); i++) {
			turret->sys->remove(i);
		}
		// Removes all enemy sprites
		for (int i = 0; i < enemy->sys->sprites.size(); i++) {
			enemy->sys->remove(i);
		}
		// Removes all enemy2 sprites
		for (int i = 0; i < enemy2->sys->sprites.size(); i++) {
			enemy2->sys->remove(i);
		}
		// Replaces highScore with score if score exceeds highScore
		if (score > highScore)
			highScore = score;
		// Stops all emitters
		turret->stop();
		enemy->stop();
		enemy2->stop();
		// Draws "Game Over" screen, displays high score, and shows directions for restrating
		ofSetColor(ofColor::red);
		string goText = "Game Over! High Score: " + std::to_string(highScore);
		text.drawString(goText, ofGetWindowWidth() / 2 - text.stringWidth(goText) / 2, ofGetWindowHeight() / 2);
		ofSetColor(ofColor::yellow);
		goText = "\nPress R to restart game";
		text.drawString(goText, ofGetWindowWidth() / 2 - text.stringWidth(goText) / 2, ofGetWindowHeight() / 2);
	}
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {
	
}

//--------------------------------------------------------------
// Moves emitter to mousepoint if canMove is true
void ofApp::mouseDragged(int x, int y, int button) {
	glm::vec3 mousePoint = glm::vec3(x, y, 1);
	// Only moves emitter if canMove is true
	if (turret->canMove)
		turret->position = mousePoint;
}

//--------------------------------------------------------------
// Does the check to see if mouse is inside emitter once clicked
void ofApp::mousePressed(int x, int y, int button) {
	// Stores current mousePoint
	glm::vec3 mousePoint = glm::vec3(x, y, 1);

	// Sets canMove to true if mouse is clicked while mousePoint is inside emitter
	if (turret->inside(mousePoint)) {
		turret->canMove = true;
	}
}

//--------------------------------------------------------------
// Sets canMove to false once mouse is released
void ofApp::mouseReleased(int x, int y, int button) {
	turret->canMove = false;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
// Defines functionality for when certain keys are pressed
void ofApp::keyPressed(int key) {
	keymap[key] = true;
	if (keymap['F'] || keymap['f']) {	// Toggles full screen
		ofToggleFullscreen();
	}
	if (keymap['H'] || keymap['h']) {	// Hides slider UI
		bHide = !bHide;
	}
	if (keymap['R'] || keymap['r']) {	// Restart Game if gameOver is true
		if (gameOver) {
			gameOver = false;
			score = 0;
			enemy->start();
			turret->drawable = true;
			turret->setPosition(ofVec3f(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0));
			turret->rotation = 0;
			turret->velocity = ofVec3f(0, 0, 0);
			turret->turnVelocity = 0;
			turret->acceleration = ofVec3f(0, 0, 0);
			turret->turnAcceleration = 0;
		}
	}
	if (keymap[' ']) {					// Starts the game
		if (standby == true) {
			standby = false;
			turret->drawable = true;
			turret->setPosition(ofVec3f(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0));
			turret->rotation = 0;
			turret->velocity = ofVec3f(0, 0, 0);
			turret->turnVelocity = 0;
			turret->acceleration = ofVec3f(0, 0, 0);
			turret->turnAcceleration = 0;
			enemy->start();
		}
		else {							// Fires lazer sprite from emitter
			if (!turret->started && gameOver == false && turret->sys->sprites.size() < 5) {
				turret->start();
			}
		}
	}
	if (keymap[OF_KEY_LEFT] || keymap['a'] || keymap['A']) {
		// Rotate turret counter-clockwise
		turret->turnAcceleration = -turret->thrust;
	}
	if (keymap[OF_KEY_RIGHT] || keymap['d'] || keymap['D']) {
		// Rotate turret clockwise
		turret->turnAcceleration = turret->thrust;
	}
	if (keymap[OF_KEY_UP] || keymap['w'] || keymap['W']) {
		// Moves up relative to heading
		turret->acceleration = turret->thrust * turret->heading();
	}
	if (keymap[OF_KEY_DOWN] || keymap['s'] || keymap['S']) {
		// Moves down relative to heading
		turret->acceleration = -turret->thrust * turret->heading();
	}
}

//--------------------------------------------------------------
// Defines functionality for when certain keys are released
void ofApp::keyReleased(int key) {
	keymap[key] = false;
	if (!keymap[OF_KEY_LEFT]) {
		// cease turret counter-clockwise rotation
		turret->turnAcceleration = 0;
	}
	if (!keymap[OF_KEY_RIGHT]) {
		// cease turret clockwise rotation
		turret->turnAcceleration = 0;
	}
	if (!keymap[OF_KEY_UP]) {	
		// cease turret movement up relative to heading
		turret->acceleration = glm::vec3(0, 0, 0);
	}
	if (!keymap[OF_KEY_DOWN]) {	
		// cease turret movement down relative to heading
		turret->acceleration = glm::vec3(0, 0, 0);
	}
}

//--------------------------------------------------------------
// Check to see if turret lazer hit enemy sprite
void ofApp::checkCollisions(Emitter *sampleEnemy) {
	// Find distance when two sprites will collide
	float collisionDist = turret->childHeight / 2 + sampleEnemy->childHeight / 2;

	// Iterate through each turret sprite and remove any enemy and turret lazer sprite
	// that came within collision distance
	for (int i = 0; i < turret->sys->sprites.size(); i++) {
		if (sampleEnemy->sys->collisionKill(turret->sys->sprites[i].position, collisionDist)) {
			// Sets position of particle explosion
			explosionPos = turret->sys->sprites[i].position;
			// Begins explosion visual effect
			partExplosion.start();
			// Plays explosion sound effect
			explosion.play();
			// Increases the score
			score++;
			// Removes specific lazer sprite from game
			turret->sys->remove(i);
		}
	}
}

//--------------------------------------------------------------
// Check to see if enemy sprite hit turret emitter
void ofApp::playerHit(Emitter * sampleEnemy)
{
	// Find distance when enemy sprite and turret emitter collide
	float collisionDist = turret->height / 2 + sampleEnemy->childHeight / 2;

	if (sampleEnemy->sys->collisionKill(turret->position, collisionDist)) {
		// Sets position of particle explosion
		explosionPos = turret->position;
		// Begins explosion visual effect
		partExplosion.start();
		// Plays explosion sound effect
		explosion.play();
		// Removes Turret
		turret->drawable = false;
		// Sets Game Over to true
		gameOver = true;
	}
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
