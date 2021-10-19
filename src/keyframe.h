#pragma  once
#ifndef KEYFRAME_H
#define KEYFRAME_H

#include <string>
#include "Shape.h"
#include "MatrixStack.h"
#include "Program.h"
#include "helicopter.h"
#include "keys.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>


class Keyframe
{
private:

	std::vector<Keys> key_frames;
	std::vector<std::pair<float, float>> usTable;

public:
	Keyframe();
	void draw_each_frame(std::shared_ptr<Helicopter> helicopter, std::shared_ptr<MatrixStack> MV, 
		std::shared_ptr<MatrixStack> P, std::shared_ptr<Program> prog);
	void draw_spline_curve();

	void translate_spline(std::shared_ptr<Helicopter> helicopter, std::shared_ptr<MatrixStack> MV,
		std::shared_ptr<MatrixStack> P, std::shared_ptr<Program> prog, double t, bool s_pressed);

	glm::mat4 createG(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4);
	glm::mat4 createG(glm::quat v1, glm::quat v2, glm::quat v3, glm::quat v4);
	
	float s2u(float s);

	virtual ~Keyframe();
};

#endif
