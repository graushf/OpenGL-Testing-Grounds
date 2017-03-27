#ifndef INPUT_H
#define INPUT_H
	#include <GLFW/glfw3.h>
	
	//class GLFWwindow;
	class Input {
		public:
			static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	};
	

#endif /* INPUT_H */