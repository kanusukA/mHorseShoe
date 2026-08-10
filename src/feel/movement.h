#include <glm/glm.hpp>
#include <feel/feel.h>
#include <memory>
// Intermediatory class between Feel and camera movements

class HorseMovement {
public:

	std::unique_ptr<glm::vec3> direction = std::make_unique<glm::vec3>();
	

	void updateInputPosition();
	


};