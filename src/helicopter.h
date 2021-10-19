#pragma  once
#ifndef HELICOPTER_H
#define HELICOPTER_H

#include <string>
#include "Shape.h"
#include "MatrixStack.h"
#include "Program.h"

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Helicopter
{
private:
	std::shared_ptr<Shape> propeller1;
	std::shared_ptr<Shape> propeller2;
	std::shared_ptr<Shape> body1;
	std::shared_ptr<Shape> body2;


public:
	Helicopter();
	Helicopter(std::string RESOURCE_DIR);

	void draw(std::shared_ptr<Program> prog);
	void draw_body(std::shared_ptr<Program> prog);

	void rotate_propeller(std::shared_ptr<MatrixStack> MV, std::shared_ptr<MatrixStack> P, std::shared_ptr<Program> prog, glm::vec3 &rot_vec, double t, int prop_number);

	std::shared_ptr<Shape> get_p1() { return propeller1; }
	std::shared_ptr<Shape> get_p2() { return propeller2; }
	std::shared_ptr<Shape> get_b1() { return body1; }
	std::shared_ptr<Shape> get_b2() { return body2; }

	virtual ~Helicopter();

};

#endif
