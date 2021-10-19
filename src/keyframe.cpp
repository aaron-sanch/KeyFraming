#include "keyframe.h"

using namespace glm;
using namespace std;

Keyframe::Keyframe()
{
	glm::vec3 pos1 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 pos2 = glm::vec3(1.0f, 2.0f, -1.0f);
	glm::vec3 pos3 = glm::vec3(0.6f, 4.0f, -2.0f);
	glm::vec3 pos4 = glm::vec3(3.0f, -2.0f, 0.8f);
	glm::vec3 pos5 = glm::vec3(-4.0f, 0.0f, 2.0f);

	glm::quat quat1 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::quat quat2 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::quat quat3 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::quat quat4 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	glm::quat quat5 = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

	glm::vec3 axis0 = glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f));
	quat1 = glm::angleAxis((float)(M_PI), axis0);

	glm::vec3 axis1 = glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f));
	quat2 = glm::angleAxis((float)(-90.0f / 180.0f * M_PI), axis1);

	glm::vec3 axis2 = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
	quat3 = glm::angleAxis((float)(0.0f), axis2);

	glm::vec3 axis3 = glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f));
	quat4 = glm::angleAxis((float)(90.0f / 180.0f * M_PI), axis3);

	glm::vec3 axis4 = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
	quat5 = glm::angleAxis((float)(90.0f / 180.0f * M_PI), axis4);


	Keys k1(pos1, quat1);
	Keys k2(pos2, quat2);
	Keys k3(pos3, quat3);
	Keys k4(pos4, quat4);
	Keys k5(pos5, quat5);

	key_frames.push_back(k1);
	key_frames.push_back(k2);
	key_frames.push_back(k3);
	key_frames.push_back(k4);
	key_frames.push_back(k5);

	// Initialize us table

	usTable.push_back(make_pair(0.0f, 0.0f));
	double s = 0;
	glm::mat4 B;
	glm::mat4 G;
	B[0] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	B[1] = glm::vec4(-0.5f, 0.0f, 0.5f, 0.0f);
	B[2] = glm::vec4(1.0f, -2.5f, 2.0f, -0.5f);
	B[3] = glm::vec4(-0.5f, 1.5f, -1.5f, 0.5f);
	for (int i = 0; i < 5; i++) {
		for (double u = 0.01; u <= 1; u += .01) {
			if (u < 0.999) {
				G = createG(key_frames[i % 5].get_pos(), key_frames[(i + 1) % 5].get_pos(), key_frames[(i + 2) % 5].get_pos(), key_frames[(i + 3) % 5].get_pos());
			}
			else if (u > 0.999) {
				G = createG(key_frames[(i + 1) % 5].get_pos(), key_frames[(i + 2) % 5].get_pos(), key_frames[(i + 3) % 5].get_pos(), key_frames[(i + 4) % 5].get_pos());
			}
			glm::vec4 uvec2(1.0f, u, u * u, u * u * u);
			double u1 = u - 0.01;
			G = createG(key_frames[i % 5].get_pos(), key_frames[(i + 1) % 5].get_pos(), key_frames[(i + 2) % 5].get_pos(), key_frames[(i + 3) % 5].get_pos());
			glm::vec4 uvec1(1.0f, u1, u1 * u1, u1 * u1 * u1);
			s += glm::length((G * (B * uvec2)) - (G * (B * uvec1)));
			usTable.push_back(make_pair(i + u, s));
		}
	}

	//usTable.push_back();
}

float Keyframe::s2u(float s)
{
	// INSERT CODE HERE
	int larger_index = 0;
	for (int i = 0; i < usTable.size(); i++) {
		if (usTable[i].second > s) {
			larger_index = i;
			break;
		}
		else if (usTable[i].second == s) {
			return i;
		}
	}
	double alpha = (s - usTable[larger_index - 1].second) / (usTable[larger_index].second - usTable[larger_index - 1].second);
	double u = (1 - alpha) * usTable[larger_index - 1].first + (alpha * usTable[larger_index].first);
	return u;
}

void Keyframe::draw_spline_curve()
{
	// Create and initialize B
	glm::mat4 B;
	B[0] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	B[1] = glm::vec4(-0.5f, 0.0f, 0.5f, 0.0f);
	B[2] = glm::vec4(1.0f, -2.5f, 2.0f, -0.5f);
	B[3] = glm::vec4(-0.5f, 1.5f, -1.5f, 0.5f);

	// Create G
	glm::mat4 G;

	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(1.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 5; i++) {
		G = createG(key_frames[(i + 4) % 5].get_pos(), key_frames[(i) % 5].get_pos(), key_frames[(1 + i) % 5].get_pos(), key_frames[(2 + i) % 5].get_pos());
		for (double u = 0; u <= 1; u += .01) {
			// Choose points for G

			// Fill in uVec
			glm::vec4 uVec(1.0f, u, u * u, u * u * u);
			// Compute position at u
			glm::vec4 p = G * (B * uVec);

			glVertex3f(p.x, p.y, p.z);
		}
	}
	glEnd();

}

void Keyframe::translate_spline(std::shared_ptr<Helicopter> helicopter, std::shared_ptr<MatrixStack> MV, std::shared_ptr<MatrixStack> P, std::shared_ptr<Program> prog, double t, bool s_pressed)
{
	int tmax = 20;
	float smax = usTable[usTable.size() -1].second;
	float tNorm = std::fmod(t, tmax) / tmax;
	float sNorm = tNorm;
	float s = smax * sNorm;
	// Create and initialize B
	glm::mat4 B;
	B[0] = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
	B[1] = glm::vec4(-0.5f, 0.0f, 0.5f, 0.0f);
	B[2] = glm::vec4(1.0f, -2.5f, 2.0f, -0.5f);
	B[3] = glm::vec4(-0.5f, 1.5f, -1.5f, 0.5f);
	
	if (s_pressed) {
		s = 5;
	}
	float time = s2u(s);

	int i = floor(fmod(time, 5));
	double u = fmod(time, 5) - i;

	// Fill in uVec
	glm::vec4 uVec(1.0f, u, u * u, u * u * u);
	glm::mat4 G;
	G = createG(key_frames[(i + 4) % 5].get_q(), key_frames[(i) % 5].get_q(), key_frames[(1 + i) % 5].get_q(), key_frames[(2 + i) % 5].get_q());
	// Fill G using rotation quaternion

	glm::vec4 qVec = G * (B * uVec);
	glm::quat q(qVec[3], qVec[0], qVec[1], qVec[2]); // Constructor argument order: (w, x, y, z)
	glm::mat4 E = glm::mat4_cast(glm::normalize(q)); // Creates a rotation matrix

	// Create G
	G = createG(key_frames[(i + 4) % 5].get_pos(), key_frames[(i) % 5].get_pos(), key_frames[(1 + i) % 5].get_pos(), key_frames[(2 + i) % 5].get_pos());

	// Compute position at u
	glm::vec4 p = G * (B * uVec);

	prog->bind();
	MV->pushMatrix();
		// Translate by p
		MV->translate(p);
		MV->multMatrix(E);

		glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
		glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
		glUniform3f(prog->getUniform("kd"), 1.0f, 0.0f, 0.0f);

		helicopter->draw_body(prog);
		prog->unbind();

		helicopter->rotate_propeller(MV, P, prog, glm::vec3(0.0f, 0.4819f, 0.0f), t, 1);
		helicopter->rotate_propeller(MV, P, prog, glm::vec3(0.6228f, 0.1179f, 0.1365f), t, 2);

	MV->popMatrix();
}

glm::mat4 Keyframe::createG(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4)
{
	glm::mat4 G;
	G[0] = glm::vec4(v1, 0.0f);
	G[1] = glm::vec4(v2, 0.0f);
	G[2] = glm::vec4(v3, 0.0f);
	G[3] = glm::vec4(v4, 0.0f);
	return G;
}

glm::mat4 Keyframe::createG(glm::quat v1, glm::quat v2, glm::quat v3, glm::quat v4)
{
	glm::mat4 G;
	if (dot(v1, v2) < 0) {
		v2 *= -1;
	}
	if (dot(v2, v3) < 0) {
		v3 *= -1;
	}
	if (dot(v3, v4) < 0) {
		v4 *= -1;
	}
	G[0] = glm::vec4(v1.x, v1.y, v1.z, v1.w);
	G[1] = glm::vec4(v2.x, v2.y, v2.z, v2.w);
	G[2] = glm::vec4(v3.x, v3.y, v3.z, v3.w);
	G[3] = glm::vec4(v4.x, v4.y, v4.z, v4.w);
	return G;
}

void Keyframe::draw_each_frame(std::shared_ptr<Helicopter> helicopter, std::shared_ptr<MatrixStack> MV, std::shared_ptr<MatrixStack> P, std::shared_ptr<Program> prog)
{

	// Define R
	glm::mat4 R = glm::mat4_cast(key_frames[0].get_q());
	// Draw Pos 1
	prog->bind();

	MV->pushMatrix();
	MV->translate(key_frames[0].get_pos());
	MV->multMatrix(R);
	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 0.0f, 0.0f);

	helicopter->draw(prog);

	MV->popMatrix();

	prog->unbind();

	// Draw Pos 2
	// Define R
	R = glm::mat4_cast(key_frames[1].get_q());
	prog->bind();

	MV->pushMatrix();
	MV->translate(key_frames[1].get_pos());
	MV->multMatrix(R);

	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 0.0f, 0.0f);

	helicopter->draw(prog);

	MV->popMatrix();

	prog->unbind();

	// Draw Pos 3
	// Define R
	R = glm::mat4_cast(key_frames[2].get_q());
	prog->bind();

	MV->pushMatrix();
	MV->translate(key_frames[2].get_pos());
	MV->multMatrix(R);

	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 0.0f, 0.0f);

	helicopter->draw(prog);

	MV->popMatrix();

	prog->unbind();

	// Draw Pos 4
	// Define R
	R = glm::mat4_cast(key_frames[3].get_q());
	prog->bind();

	MV->pushMatrix();
	MV->translate(key_frames[3].get_pos());
	MV->multMatrix(R);

	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 0.0f, 0.0f);

	helicopter->draw(prog);

	MV->popMatrix();

	prog->unbind();

	// Draw Pos 5
	// Define R
	R = glm::mat4_cast(key_frames[4].get_q());
	prog->bind();

	MV->pushMatrix();
	MV->translate(key_frames[4].get_pos());
	MV->multMatrix(R);

	glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
	glUniform3f(prog->getUniform("kd"), 1.0f, 0.0f, 0.0f);

	helicopter->draw(prog);

	MV->popMatrix();

	prog->unbind();

}

Keyframe::~Keyframe()
{
}
