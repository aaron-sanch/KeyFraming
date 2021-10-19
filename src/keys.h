#pragma  once
#ifndef KEYS_H
#define KEYS_H

#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>


class Keys
{
private:
	glm::vec3 pos;
	glm::quat quatonian;
public:
	Keys();
	Keys(glm::vec3 p, glm::quat q);

	glm::vec3 get_pos() { return pos; }
	glm::quat get_q() { return quatonian; }
	
	virtual ~Keys();
};

#endif
