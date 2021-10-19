#include "helicopter.h"

using namespace std;

Helicopter::Helicopter()
{
}

Helicopter::Helicopter(std::string RESOURCE_DIR)
{
	// load in all of our helicopter parts into their respective shape meshes
	propeller1 = std::make_shared<Shape>();
	propeller1->loadMesh(RESOURCE_DIR + "helicopter_prop1.obj");
	propeller1->init();

	propeller2 = std::make_shared<Shape>();
	propeller2->loadMesh(RESOURCE_DIR + "helicopter_prop2.obj");
	propeller2->init();

	body1 = std::make_shared<Shape>();
	body1->loadMesh(RESOURCE_DIR + "helicopter_body1.obj");
	body1->init();

	body2 = std::make_shared<Shape>();
	body2->loadMesh(RESOURCE_DIR + "helicopter_body2.obj");
	body2->init();

}

void Helicopter::draw(std::shared_ptr<Program> prog)
{
	propeller1->draw(prog);
	propeller2->draw(prog);
	body1->draw(prog);
	body2->draw(prog);
}

void Helicopter::draw_body(std::shared_ptr<Program> prog)
{
	/*propeller1->draw(prog);
	propeller2->draw(prog);*/
	body1->draw(prog);
	body2->draw(prog);
}

void Helicopter::rotate_propeller(std::shared_ptr<MatrixStack> MV, std::shared_ptr<MatrixStack> P, std::shared_ptr<Program> prog, glm::vec3 &rot_vec, double t, int prop_number)
{
	prog->bind();
		
		MV->pushMatrix();
			MV->translate(rot_vec);

			if (prop_number <= 1) {
				MV->rotate(t, glm::vec3(0, 1, 0));
			}
			else {
				MV->rotate(t, glm::vec3(0, 0, 1));
			}

			MV->translate(-rot_vec);

			glUniformMatrix4fv(prog->getUniform("P"), 1, GL_FALSE, glm::value_ptr(P->topMatrix()));
			glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV->topMatrix()));
			glUniform3f(prog->getUniform("kd"), 1.0f, 0.0f, 0.0f);

			if (prop_number <= 1) {
				propeller1->draw(prog);
			}
			else {
				propeller2->draw(prog);
			}

		MV->popMatrix();

	prog->unbind();
}



Helicopter::~Helicopter()
{
}




