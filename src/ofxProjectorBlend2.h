#pragma once
#include "ofMain.h"

struct overLapData
{
	bool isLeftOverLap;
	bool isRightOverLap;
	bool isTopOverLap;
	bool isBottomOverLap;
};

class ofxProjectorBlend2
{
public:
	ofxProjectorBlend2();
	~ofxProjectorBlend2();


	void setup(int _resolutionWidth, int _resolutionHeight, 
		int _horNumProjectors, int _horPixelOverlap,int _verNumPorjectors,int _verPixelOverlap);

	void begin();
	void end();
	void draw(float x = 0, float y = 0);


	float getCanvasWidth();
	float getCanvasHeight();
	ofFbo & getFullTexture() { return fullTexture; }
	void setShaderLocation(string shaderLocation);

	float getDisplayWidth();
	float getDisplayHeight();

	void setWindowToDisplaySize();

	void setHorPixelOverlap(float layoutlength);
	void setVerPixelOverlap(float layoutlength);

	void setLeftOverLapFlag(int _index, bool _flag);
	void setRightOverLapFlag(int _index, bool _flag);
	void setTopOverLapFlag(int _index, bool _flag);
	void setBottomOverLapFlag(int _index, bool _flag);
	int getProjectorsNums()const;



	bool showBlend;

	float blendPower;
	float gammaR;
	float gammaG;
	float gammaB;
	float luminance;
	float blendPower2;
	float gammaR2;
	float gammaG2;
	float gammaB2;
	float luminance2;
	float threshold;

private:

	string shaderLocation;

	float fullTextureWidth;
	float fullTextureHeight;
	float singleChannelWidth;
	float singleChannelHeight;

	float displayWidth;
	float displayHeight;

	//float pixelOverlap;
	float horPixelOverlap;
	float verPixelOverlap;

	//int numProjectors;
	int horNumProjectors;
	int verNumProjectors;
	int totalProjectors;
	vector<overLapData> overLapFlags;

	ofShader blendShader;
	ofFbo fullTexture;
	void updateShaderUniforms();
};

