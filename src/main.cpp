#ifdef WIN32
#include <GL/glew.h>
#endif

#include <GL/glfw.h>
#include <cstdio>
#include <pthread.h>
#include <vector>

#include "SimpleRenderer.h"
#include "Scene.h"
#include "ObjLoader.h"

#define WIDTH 512
#define HEIGHT 512

typedef struct
{
    RenderDevice *device;
    Scene *scene;
    float *pixels;
}RenderTarget;

void *renderParallell( void *arg )
{
    RenderTarget *rt = (RenderTarget*)arg;
    rt->device->renderToArray(rt->scene, rt->pixels, WIDTH, HEIGHT);

    return NULL;
}

void initScene(Scene &scene)
{
    loadObj(scene.geometry, "media/bunny.obj", 1.0f);

    Material matRed;
    matRed.setDiffuseColor(1.0,0.0,0.0);

    scene.material.push_back(matRed);

    Camera cam;
    cam.direction = vec3(0,0,-1);
    cam.up = vec3(0,1,0);

    scene.camera.push_back(cam);

    scene.activeCamera = 0;

    scene.translate(0,-4,-10);
}

int main(int argc, char ** argv) {

	glfwInit();

	// GLFW_WINDOW
	if(!glfwOpenWindow(WIDTH, HEIGHT, 8,8,8,8, 32,0, GLFW_WINDOW)) {
		glfwTerminate(); // glfwOpenWindow failed, quit the program.
		return 0;
	}

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// making default background color black
    glfwSwapInterval(1);

	float *pixels = new float[WIDTH*HEIGHT*3];

    SimpleRenderer renderer;
    Scene scene;

    initScene(scene);

    RenderTarget rt;
    rt.device = &renderer;
    rt.scene = &scene;
    rt.pixels = pixels;

	pthread_t thread;
	int rc = pthread_create(&thread, NULL, renderParallell, (void*)&rt);
	if (rc)
	{
        printf("ERROR; return code from pthread_create() is %d\n", rc);
        return 0;
    }

    bool quitProgram = false;
	while(!quitProgram) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawPixels(WIDTH,HEIGHT,GL_RGB,GL_FLOAT,pixels);
		glfwSwapBuffers();

		// check if time to quit
		if(glfwGetKey('Q') || !glfwGetWindowParam(GLFW_OPENED)) {
			quitProgram = true;
		}
	}

	delete pixels;

	glfwTerminate();
    pthread_exit(NULL);

	return 0;
}