#include "InputManager.h"
// #include "../DisplayGLFW/display.h"
#include "game.h"
#include "../res/includes/glm/glm.hpp"
#include "parser.h"
#include "renderer.h"

int main(int argc,char *argv[])
{
	const int DISPLAY_WIDTH = 800;
	const int DISPLAY_HEIGHT = 800;
	const float CAMERA_ANGLE = 0.0f;
	const float NEAR = 1.0f;
	const float FAR = 100.0f;

	Game *scn = new Game(CAMERA_ANGLE,(float)DISPLAY_WIDTH/DISPLAY_HEIGHT,NEAR,FAR);
	
	Display display(DISPLAY_WIDTH, DISPLAY_HEIGHT, "OpenGL");
	
	Init(display);

	Camera* camera = scn->getCameras()[0];
	std::vector<Light> lights;
	std::vector<std::shared_ptr<Object>> objects;
	std::vector<Material> materials;
	glm::vec4 ambientLight;

	Parser parser("../res/scene.txt");
	parser.parseScene(camera, &lights, &objects, &materials, &ambientLight);
	parser.fixColors(&objects, &materials);

	Renderer renderer(camera, lights, objects, materials, ambientLight, DISPLAY_WIDTH, DISPLAY_HEIGHT);
	renderer.render();

	scn->Init(DISPLAY_WIDTH, DISPLAY_HEIGHT, renderer.getImageBuffer());

	display.SetScene(scn);

	while(!display.CloseWindow())
	{
		scn->Draw(1,0,scn->BACK,true,false);
		scn->Motion();
		display.SwapBuffers();
		display.PollEvents();	
			
	}
	delete scn;
	return 0;
}
