#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	int singleW = 640;
	int singleH = 480;
	int horNums = 2;
	int verNums = 2;
	int horOverLap = singleW * 0.2f;
	int verOverLap = singleH * 0.2f;
	blender.setup(singleW, singleH, horNums, horOverLap, verNums, verOverLap);
	blender.setWindowToDisplaySize();

	for (int i = 0;i < blender.getProjectorsNums();i++)
	{
		int xIndex = i % 2;
		int yIndex = i / 2;
		blender.setLeftOverLapFlag(i,true);
		blender.setRightOverLapFlag(i, true);
		blender.setTopOverLapFlag(i, true);
		blender.setBottomOverLapFlag(i, true);


		 if (xIndex == 0)
		 {
			 blender.setLeftOverLapFlag(i, false);
		 }
		 else if (xIndex == horNums-1)
		 {
			 blender.setRightOverLapFlag(i, false);
		 }

		 if (yIndex == 0)
		 {
			 blender.setTopOverLapFlag(i, false);
		 }
		 else if (yIndex == verNums-1)
		 {
			 blender.setBottomOverLapFlag(i, false);
		 }
	}
}

//--------------------------------------------------------------
void ofApp::update(){
	blender.begin();
	ofPushStyle();
	//light gray backaground
	ofSetColor(100, 100, 100);
	ofDrawRectangle(0, 0, blender.getCanvasWidth(), blender.getCanvasHeight());

	//thick grid lines for blending
	ofSetColor(255, 255, 255);
	ofSetLineWidth(3);

	//vertical line
	for (int i = 0; i <= blender.getCanvasWidth(); i += 40) {
		ofDrawLine(i, 0, i, blender.getCanvasHeight());
	}

	//horizontal lines
	for (int j = 0; j <= blender.getCanvasHeight(); j += 40) {
		ofDrawLine(0, j, blender.getCanvasWidth(), j);
	}
	ofPopStyle();
	blender.end();
}

//--------------------------------------------------------------
void ofApp::draw(){
	blender.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	//hit spacebar to toggle the blending strip
	if (key == ' ') {
		//toggling this variable effects whether the blend strip is shown
		blender.showBlend = !blender.showBlend;
	}

	// more info here on what these variables do
	// http://local.wasp.uwa.edu.au/~pbourke/texture_colour/edgeblend/

	else if (key == 'q') {
		blender.luminance -= .05;
		blender.luminance2 -= .05;
	}
	else if (key == 'w') {
		blender.luminance += .05;
		blender.luminance2 += .05;
	}
	else if (key == 'e') {
		blender.blendPower -= .05;
		blender.blendPower2 -= .05;
	}
	else if (key == 'r') {
		blender.blendPower += .05;
		blender.blendPower2 += .05;
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
