#include "ofxProjectorBlend2.h"

string fragmentShader2 = "#extension GL_ARB_texture_rectangle : enable\n \
						\
						uniform sampler2DRect Tex0;\
						\
						uniform float width;\
						uniform float height;\
						\
						uniform float OverlapLeft;\
						uniform float OverlapRight; \
						uniform float OverlapTop;\
						uniform float OverlapBottom;\
						\
						uniform float BlackOutLeft;\
						uniform float BlackOutRight; \
						uniform float BlackOutTop;\
						uniform float BlackOutBottom;\
						\
						uniform float BlendPower;\
						uniform float SomeLuminanceControl;\
						uniform vec3  GammaCorrection;\
						\
						uniform float BlendPower2;\
						uniform float SomeLuminanceControl2;\
						uniform vec3  GammaCorrection2;\
						\
						uniform float SolidEdgeEnable;\
						uniform vec4  SolidEdgeColor;\
						uniform vec2  texCoordOffset;\
						\
						uniform float projectors;\
						uniform float threshold;\
						\
						vec4 drawSmoothEdge(float loc, float extend, float blePow, float lumCon, vec3 gamma)\
						{\
						vec4 edge;\
						float curve;\
						\
						curve = loc / extend;\
						curve = (curve < 0.5) ? \
						lumCon * pow(2.0 * curve, blePow) \
						: \
						1.0 - (1.0 - lumCon) * pow(2.0 * (1.0 - curve), blePow);\
						\
						edge = vec4(pow(curve, 1.0 / gamma.r), \
						pow(curve, 1.0 / gamma.g),\
						pow(curve, 1.0 / gamma.b),\
						1.0);\
						\
						return edge;\
						}\
						\
						vec4 drawSmoothEdges(in vec4 overlap, in vec4 blankout, in vec4 color)\
						{\
						vec2 xy = gl_TexCoord[0].xy;\
						vec2 xy_t = xy + texCoordOffset.xy;\
						vec4 blank = vec4(0.0,0.0,0.0,0.0);	\
						\
						float proj2 = 1.0;\
						float thresh = proj2 * width + overlap.x;\
						float blnPwr = (xy_t.x > thresh) ? BlendPower: BlendPower2;\
						float slc    = (xy_t.x > thresh) ? SomeLuminanceControl: SomeLuminanceControl2;\
						vec3 gamma   = (xy_t.x > thresh) ? GammaCorrection: GammaCorrection2;\
						slc += 0.5;\
						\
						vec4 sebImage = \
						(blankout.x + overlap.x > xy.x) ? \
						(blankout.x > xy.x) ? 	\
						blank\
						:	(blankout.z + overlap.z > xy.y) ?\
						(blankout.z > xy.y) ? \
						blank\
						: 	texture2DRect(Tex0, xy_t) 	*		\
						drawSmoothEdge(xy.x - blankout.x, overlap.x, blnPwr, slc, gamma) * \
						drawSmoothEdge(xy.y - blankout.z, overlap.z, blnPwr, slc, gamma)\
						: 	(height - blankout.w - overlap.w < xy.y) ?  \
						(height - blankout.w < xy.y) ? \
						blank\
						: 	texture2DRect(Tex0, xy_t) 	*\
						drawSmoothEdge(xy.x - blankout.x, overlap.x, blnPwr, slc, gamma) * \
						drawSmoothEdge(height - blankout.w - xy.y, overlap.w, blnPwr, slc, gamma)	\
						:	texture2DRect(Tex0, xy_t) *\
						drawSmoothEdge(xy.x - blankout.x, overlap.x, blnPwr, slc, gamma)\
						:	(width - blankout.y - overlap.y < xy.x) ? \
						(width - blankout.y < xy.x) ? \
						blank\
						:	(blankout.z + overlap.z > xy.y) ?\
						(blankout.z > xy.y) ? \
						blank\
						: 	texture2DRect(Tex0, xy_t) 	*\
						drawSmoothEdge(width - blankout.y - xy.x, overlap.y, blnPwr, slc, gamma) *\
						drawSmoothEdge(xy.y - blankout.z, overlap.z, blnPwr, slc, gamma)\
						:	(height - blankout.w - overlap.w < xy.y) ?\
						(height - blankout.w < xy.y) ? \
						blank\
						:	texture2DRect(Tex0, xy_t) *\
						drawSmoothEdge(width - blankout.y - xy.x, overlap.y, blnPwr, slc, gamma) * \
						drawSmoothEdge(height - blankout.w - xy.y, overlap.w, blnPwr, slc, gamma)\
						:	texture2DRect(Tex0, xy_t) *\
						drawSmoothEdge(width - blankout.y - xy.x, overlap.y, blnPwr, slc, gamma)\
						:	(blankout.z + overlap.z > xy.y) ?\
						(blankout.z > xy.y) ? \
						blank\
						: 	texture2DRect(Tex0, xy_t) *\
						drawSmoothEdge(xy.y - blankout.z, overlap.z, blnPwr, slc, gamma)\
						:	(height - blankout.w - overlap.w < xy.y) ?\
						(height - blankout.w < xy.y) ? \
						blank\
						:	texture2DRect(Tex0, xy_t) *\
						drawSmoothEdge(height - blankout.w - xy.y, overlap.w, blnPwr, slc, gamma)		\
						:	texture2DRect(Tex0, xy_t);\
						\
						return sebImage;\
						}\
						\
						vec4 drawSolidEdges(vec4 overlap, vec4 blankout, vec4 color)\
						{	\
						\
						vec2 xy = gl_TexCoord[0].xy;\
						vec2 xy_t = xy + texCoordOffset.xy;\
						vec4 blank = vec4(0.0,0.0,0.0,0.0);		\
						vec4 edgeImage =\
						(blankout.x + overlap.x > xy.x) ?\
						(blankout.x > xy.x) ?\
						blank\
						:	(blankout.z + overlap.z > xy.y) ?\
						(blankout.z > xy.y) ?\
						blank\
						: 	color\
						: 	(height - blankout.w - overlap.w < xy.y) ?\
						(height - blankout.w < xy.y) ?\
						blank\
						: 	color\
						:	color\
						:	(width - blankout.y - overlap.y < xy.x) ?\
						(width - blankout.y < xy.x) ?\
						blank\
						:	(blankout.z + overlap.z > xy.y) ?\
						(blankout.z > xy.y) ?\
						blank\
						: 	color\
						:	(height - blankout.w - overlap.w < xy.y) ?\
						(height - blankout.w < xy.y) ? \
						blank\
						:	color\
						:	color\
						:	(blankout.z + overlap.z > xy.y) ?\
						(blankout.z > xy.y) ? \
						blank\
						: 	color\
						:	(height - blankout.w - overlap.w < xy.y) ?\
						(height - blankout.w < xy.y) ? \
						blank\
						:	color\
						:	texture2DRect(Tex0, xy_t);		\
						return edgeImage;\
						}\
						\
						void main (void)\
						{\
						vec4 overlap = vec4 (OverlapLeft, OverlapRight, OverlapBottom, OverlapTop);\
						vec4 blankout = vec4 (BlackOutLeft, BlackOutRight, BlackOutBottom, BlackOutTop);\
						gl_FragData[0] = (SolidEdgeEnable == 1.0) ?\
						drawSolidEdges(overlap, blankout, SolidEdgeColor)\
						:	drawSmoothEdges(overlap, blankout, SolidEdgeColor);\
						}";

ofxProjectorBlend2::ofxProjectorBlend2()
{
	showBlend = true;

	gammaR = gammaR2 = 0.5;
	gammaG = gammaG2 = 0.5;
	gammaB = gammaB2 = 0.5;
	blendPower = blendPower2 = 1;
	luminance = luminance2 = 0;
	threshold = 0;
	shaderLocation = "SmoothEdgeBlend";

	horNumProjectors = 0;
	verNumProjectors = 0;
}


ofxProjectorBlend2::~ofxProjectorBlend2()
{
}

void ofxProjectorBlend2::setShaderLocation(string _shaderLocation) {
	shaderLocation = _shaderLocation;
	//blendShader.load(shaderLocation);

	bool res = true;
	res &= blendShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader2);
	res &= blendShader.linkProgram();

	if (!res)
	{
		ofLogError() << "shader setup failed" << endl;
	}
}

void  ofxProjectorBlend2::setup(int _resolutionWidth, int _resolutionHeight,
	int _horNumProjectors, int _horPixelOverlap, int _verNumPorjectors, int _verPixelOverlap)
{
	horNumProjectors = _horNumProjectors;
	horPixelOverlap = _horPixelOverlap;

	verNumProjectors = _verNumPorjectors;
	verPixelOverlap = _verPixelOverlap;

	totalProjectors = horNumProjectors * verNumProjectors;

	singleChannelWidth = _resolutionWidth;
	singleChannelHeight = _resolutionHeight;

	ofLog(OF_LOG_NOTICE, "ofxProjectorBlend: res: %d x %d * %d x %d, horOverlap: %d, verOverlap: %d \n", 
		(int)singleChannelWidth, horNumProjectors, (int)singleChannelHeight,verNumProjectors, (int)horPixelOverlap, (int)verPixelOverlap);

	
	if (horNumProjectors <= 0 || verNumProjectors <= 0)
	{
		ofLog(OF_LOG_ERROR, "Cannot initialize with " + ofToString(this->horNumProjectors) + "*" + ofToString(verNumProjectors) + " projectors.");
		return;
	}

	fullTextureWidth = singleChannelWidth * horNumProjectors - (horNumProjectors - 1)* horPixelOverlap;
	fullTextureHeight = singleChannelHeight * verNumProjectors - (verNumProjectors - 1)* verPixelOverlap;

	displayWidth = singleChannelWidth * horNumProjectors;
	displayHeight = singleChannelHeight * verNumProjectors;


	ofLog(OF_LOG_NOTICE, "canvas res: %d  x  %d", (int)fullTextureWidth, (int)fullTextureHeight);

	overLapFlags.resize(horNumProjectors * verNumProjectors);
	for (int i = 0; i < overLapFlags.size(); i++)
	{
		overLapFlags[i].isLeftOverLap = false;
		overLapFlags[i].isRightOverLap = false;
		overLapFlags[i].isTopOverLap = false;
		overLapFlags[i].isBottomOverLap = false;
	}

	fullTexture.allocate(fullTextureWidth, fullTextureHeight, GL_RGB, 4);
	//blendShader.load(shaderLocation);
	blendShader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragmentShader2);
	blendShader.linkProgram();
}

void ofxProjectorBlend2::begin() {
	fullTexture.begin();

	ofPushStyle();
	ofClear(0, 0, 0, 0);
}

void ofxProjectorBlend2::end() {
	fullTexture.end();

	ofPopStyle();
}

float ofxProjectorBlend2::getDisplayWidth() {
	return displayWidth;
}

float ofxProjectorBlend2::getDisplayHeight() {
	return displayHeight;
}

void ofxProjectorBlend2::setWindowToDisplaySize() {
	ofSetWindowShape(getDisplayWidth(), getDisplayHeight());
}

void ofxProjectorBlend2::setHorPixelOverlap(float layoutlength)
{
	horPixelOverlap = layoutlength;

	fullTextureWidth = singleChannelWidth * horNumProjectors - (horNumProjectors - 1)* horPixelOverlap;

	fullTexture.allocate(fullTextureWidth, fullTextureHeight, GL_RGB, 4);
}

void ofxProjectorBlend2::setVerPixelOverlap(float layoutlength)
{
	verPixelOverlap = layoutlength;

	fullTextureHeight = singleChannelHeight * verNumProjectors - (verNumProjectors - 1)* verPixelOverlap;

	fullTexture.allocate(fullTextureWidth, fullTextureHeight, GL_RGB, 4);
}

void ofxProjectorBlend2::setLeftOverLapFlag(int _index, bool _flag)
{
	if (_index >= 0 && _index < overLapFlags.size())
	{
		overLapFlags[_index].isLeftOverLap = _flag;
	}
}

void ofxProjectorBlend2::setRightOverLapFlag(int _index, bool _flag)
{
	if (_index >= 0 && _index < overLapFlags.size())
	{
		overLapFlags[_index].isRightOverLap = _flag;
	}
}

void ofxProjectorBlend2::setTopOverLapFlag(int _index, bool _flag)
{
	if (_index >= 0 && _index < overLapFlags.size())
	{
		overLapFlags[_index].isTopOverLap = _flag;
	}
}

void ofxProjectorBlend2::setBottomOverLapFlag(int _index, bool _flag)
{
	if (_index >= 0 && _index < overLapFlags.size())
	{
		overLapFlags[_index].isBottomOverLap = _flag;
	}
}

int ofxProjectorBlend2::getProjectorsNums()const
{
	return totalProjectors;
}

float ofxProjectorBlend2::getCanvasWidth()
{
	return fullTextureWidth;
}

float ofxProjectorBlend2::getCanvasHeight()
{
	return fullTextureHeight;
}

void ofxProjectorBlend2::updateShaderUniforms() {

	blendShader.setUniform1f("OverlapTop", 0.0f);
	blendShader.setUniform1f("OverlapLeft", 0.0f);
	blendShader.setUniform1f("OverlapBottom", 0.0f);
	blendShader.setUniform1f("OverlapRight", 0.0f);

	blendShader.setUniform1f("BlendPower", blendPower);
	blendShader.setUniform1f("SomeLuminanceControl", luminance);
	blendShader.setUniform3f("GammaCorrection", gammaR, gammaG, gammaB);

	blendShader.setUniform1f("BlendPower2", blendPower2);
	blendShader.setUniform1f("SomeLuminanceControl2", luminance2);
	blendShader.setUniform3f("GammaCorrection2", gammaR2, gammaG2, gammaB2);

	blendShader.setUniform1f("projectors", totalProjectors);
	blendShader.setUniform1f("threshold", threshold);
}

void ofxProjectorBlend2::draw(float x, float y) {
	
	glPushMatrix();
	glTranslatef(x, y, 0);
	if (showBlend)
	{

		blendShader.begin();
		blendShader.setUniform1f("width", singleChannelWidth);
		blendShader.setUniform1f("height", singleChannelHeight);

		updateShaderUniforms();

		blendShader.setUniformTexture("Tex0", fullTexture.getTexture(), 0);

		ofVec2f offset(0, 0);
		glPushMatrix();

		// loop through each projector and glTranslatef() to its position and draw.
		for (int i = 0; i < totalProjectors; i++)
		{
			glPushMatrix();
			int horIndex = i % horNumProjectors;
			int verIndex = i / horNumProjectors;

			//ÎÆÀíÆ«ÒÆ
			offset.x = (singleChannelWidth - horPixelOverlap) * horIndex;
			offset.y = (singleChannelHeight - verPixelOverlap) * verIndex;

			//»æÖÆÆ«ÒÆ
			glTranslatef(singleChannelWidth * horIndex, singleChannelHeight * verIndex, 0);

			blendShader.setUniform2f("texCoordOffset", offset.x, offset.y);

			blendShader.setUniform1f("OverlapTop", overLapFlags[i].isBottomOverLap ? verPixelOverlap : 0.0f);
			blendShader.setUniform1f("OverlapLeft", overLapFlags[i].isLeftOverLap ? horPixelOverlap : 0.0f);
			blendShader.setUniform1f("OverlapBottom", overLapFlags[i].isTopOverLap ? verPixelOverlap : 0.0f);
			blendShader.setUniform1f("OverlapRight", overLapFlags[i].isRightOverLap ? horPixelOverlap : 0.0f);

			glBegin(GL_QUADS);
			glTexCoord2f(0, 0);                                    glVertex2f(0, 0);
			glTexCoord2f(singleChannelWidth, 0);                   glVertex2f(singleChannelWidth, 0);
			glTexCoord2f(singleChannelWidth, singleChannelHeight); glVertex2f(singleChannelWidth, singleChannelHeight);
			glTexCoord2f(0, singleChannelHeight);                  glVertex2f(0, singleChannelHeight);
			glEnd();
			glPopMatrix();
		}
		glPopMatrix();

		blendShader.end();
	}
	else
	{
		fullTexture.draw(0, 0, displayWidth, displayHeight);
	}

	//Ïû³ý¾â³Ý
	ofPushStyle();
	ofNoFill();
	ofSetColor(0, 0, 0);
	ofDrawRectangle(1, 1, displayWidth - 1, displayHeight - 1);
	ofSetColor(255, 255, 255);
	ofPopStyle();

	glPopMatrix();
}
