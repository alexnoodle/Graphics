/* On macOS, compile with...
    clang 590mainShadowing.c /usr/local/gl3w/src/gl3w.o -lglfw -framework OpenGL -framework CoreFoundation
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdarg.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <sys/time.h>

double getTime(void) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (double)tv.tv_sec + (double)tv.tv_usec * 0.000001;
}

#include "500shader.c"
#include "530vector.c"
#include "580mesh.c"
#include "590matrix.c"
#include "520camera.c"
#include "540texture.c"
#include "580scene.c"
#include "560light.c"
#include "590shadow.c"

camCamera cam;
texTexture texH, texV, texW, texT, texL;
meshGLMesh meshH, meshV, meshW, meshT, meshL;
sceneNode nodeH, nodeV, nodeW, nodeT, nodeL;
/* We need just one shadow program, because all of our meshes have the same 
attribute structure. */
shadowProgram sdwProg;
/* We need one shadow map per shadow-casting light. */
lightLight lightOne;
lightLight lightTwo;
shadowMap sdwMapOne;
shadowMap sdwMapTwo;
/* The main shader program has extra hooks for shadowing. */
GLuint program;
GLint viewingLoc, modelingLoc;
GLint unifLocs[1], textureLocs[1];
GLint attrLocs[3];
GLint lightOnePosLoc, lightOneColLoc, lightOneAttLoc, lightOneDirLoc, lightOneCosLoc;
GLint lightTwoPosLoc, lightTwoColLoc, lightTwoAttLoc, lightTwoDirLoc, lightTwoCosLoc;
GLint camPosLoc;
GLint viewingSdwOneLoc, textureSdwOneLoc;
GLint viewingSdwTwoLoc, textureSdwTwoLoc;

void handleError(int error, const char *description) {
	fprintf(stderr, "handleError: %d\n%s\n", error, description);
}

void handleResize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
	camSetWidthHeight(&cam, width, height);
}

void handleKey(GLFWwindow *window, int key, int scancode, int action,
		int mods) {
	int shiftIsDown = mods & GLFW_MOD_SHIFT;
	int controlIsDown = mods & GLFW_MOD_CONTROL;
	int altOptionIsDown = mods & GLFW_MOD_ALT;
	int superCommandIsDown = mods & GLFW_MOD_SUPER;
	if (action == GLFW_PRESS && key == GLFW_KEY_L) {
		camSwitchProjectionType(&cam);
	} else if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		if (key == GLFW_KEY_O)
			camAddTheta(&cam, -0.1);
		else if (key == GLFW_KEY_P)
			camAddTheta(&cam, 0.1);
		else if (key == GLFW_KEY_I)
			camAddPhi(&cam, -0.1);
		else if (key == GLFW_KEY_K)
			camAddPhi(&cam, 0.1);
		else if (key == GLFW_KEY_U)
			camAddDistance(&cam, -0.5);
		else if (key == GLFW_KEY_J)
			camAddDistance(&cam, 0.5);
		else if (key == GLFW_KEY_Y) {
			GLdouble vec[3];
			vecCopy(3, lightOne.translation, vec);
			vec[1] += 1.0;
			lightSetTranslation(&lightOne, vec);
		} else if (key == GLFW_KEY_H) {
			GLdouble vec[3];
			vecCopy(3, lightOne.translation, vec);
			vec[1] -= 1.0;
			lightSetTranslation(&lightOne, vec);
		}
		else if (key == GLFW_KEY_T) {
			GLdouble vec[3];
			vecCopy(3, lightOne.translation, vec);
			vec[0] += 1.0;
			lightSetTranslation(&lightOne, vec);
		} else if (key == GLFW_KEY_G) {
			GLdouble vec[3];
			vecCopy(3, lightOne.translation, vec);
			vec[0] -= 1.0;
			lightSetTranslation(&lightOne, vec);
		}
		else if (key == GLFW_KEY_W) {
			GLdouble vec[3];
			vecCopy(3, lightTwo.translation, vec);
			vec[1] += 1.0;
			lightSetTranslation(&lightTwo, vec);
		} else if (key == GLFW_KEY_S) {
			GLdouble vec[3];
			vecCopy(3, lightTwo.translation, vec);
			vec[1] -= 1.0;
			lightSetTranslation(&lightTwo, vec);
		}
		else if (key == GLFW_KEY_A) {
			GLdouble vec[3];
			vecCopy(3, lightTwo.translation, vec);
			vec[0] += 1.0;
			lightSetTranslation(&lightTwo, vec);
		} else if (key == GLFW_KEY_D) {
			GLdouble vec[3];
			vecCopy(3, lightTwo.translation, vec);
			vec[0] -= 1.0;
			lightSetTranslation(&lightTwo, vec);
		}
	}
}

/* Returns 0 on success, non-zero on failure. Warning: If initialization fails 
midway through, then does not properly deallocate all resources. But that's 
okay, because the program terminates almost immediately after this function 
returns. */
int initializeScene(void) {
	if (texInitializeFile(&texH, "grass.jpg", GL_LINEAR, GL_LINEAR, 
    		GL_REPEAT, GL_REPEAT) != 0)
    	return 1;
    if (texInitializeFile(&texV, "rock.jpg", GL_LINEAR, GL_LINEAR, 
    		GL_REPEAT, GL_REPEAT) != 0)
    	return 2;
    if (texInitializeFile(&texW, "water.jpg", GL_LINEAR, GL_LINEAR, 
    		GL_REPEAT, GL_REPEAT) != 0)
    	return 3;
    if (texInitializeFile(&texT, "trunk.jpg", GL_LINEAR, GL_LINEAR, 
    		GL_REPEAT, GL_REPEAT) != 0)
    	return 4;
    if (texInitializeFile(&texL, "tree.jpg", GL_LINEAR, GL_LINEAR, 
    		GL_REPEAT, GL_REPEAT) != 0)
    	return 5;
	GLuint attrDims[3] = {3, 2, 3};
    double zs[12][12] = {
		{5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0}, 
		{5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0, 25.0}, 
		{5.0, 5.0, 10.0, 12.0, 5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0, 25.0}, 
		{5.0, 5.0, 10.0, 10.0, 5.0, 5.0, 5.0, 5.0, 5.0, 20.0, 25.0, 27.0}, 
		{0.0, 0.0, 5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 20.0, 20.0, 25.0}, 
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 25.0}, 
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 
		{0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 
		{0.0, 0.0, 0.0, 0.0, 0.0, 5.0, 7.0, 0.0, 0.0, 0.0, 0.0, 0.0}, 
		{0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 20.0}, 
		{5.0, 5.0, 5.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 20.0, 20.0, 20.0}, 
		{10.0, 10.0, 5.0, 5.0, 0.0, 0.0, 0.0, 5.0, 10.0, 15.0, 20.0, 25.0}};
	double ws[12][12] = {
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}, 
		{1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0}};
	meshMesh mesh, meshLand;
	if (meshInitializeLandscape(&meshLand, 12, 12, 5.0, (double *)zs) != 0)
		return 6;
	if (meshInitializeDissectedLandscape(&mesh, &meshLand, M_PI / 3.0, 1) != 0)
		return 7;
	/* There are now two VAOs per mesh. */
	meshGLInitialize(&meshH, &mesh, 3, attrDims, 2);
	meshGLVAOInitialize(&meshH, 0, attrLocs);
	meshGLVAOInitialize(&meshH, 1, sdwProg.attrLocs);
	meshDestroy(&mesh);
	if (meshInitializeDissectedLandscape(&mesh, &meshLand, M_PI / 3.0, 0) != 0)
		return 8;
	meshDestroy(&meshLand);
	double *vert, normal[2];
	for (int i = 0; i < mesh.vertNum; i += 1) {
		vert = meshGetVertexPointer(&mesh, i);
		normal[0] = -vert[6];
		normal[1] = vert[5];
		vert[3] = (vert[0] * normal[0] + vert[1] * normal[1]) / 20.0;
		vert[4] = vert[2] / 20.0;
	}
	meshGLInitialize(&meshV, &mesh, 3, attrDims, 2);
	meshGLVAOInitialize(&meshV, 0, attrLocs);
	meshGLVAOInitialize(&meshV, 1, sdwProg.attrLocs);
	meshDestroy(&mesh);
	if (meshInitializeLandscape(&mesh, 12, 12, 5.0, (double *)ws) != 0)
		return 9;
	meshGLInitialize(&meshW, &mesh, 3, attrDims, 2);
	meshGLVAOInitialize(&meshW, 0, attrLocs);
	meshGLVAOInitialize(&meshW, 1, sdwProg.attrLocs);
	meshDestroy(&mesh);
	if (meshInitializeCapsule(&mesh, 1.0, 10.0, 1, 8) != 0)
		return 10;
	meshGLInitialize(&meshT, &mesh, 3, attrDims, 2);
	meshGLVAOInitialize(&meshT, 0, attrLocs);
	meshGLVAOInitialize(&meshT, 1, sdwProg.attrLocs);
	meshDestroy(&mesh);
	if (meshInitializeSphere(&mesh, 5.0, 8, 16) != 0)
		return 11;
	meshGLInitialize(&meshL, &mesh, 3, attrDims, 2);
	meshGLVAOInitialize(&meshL, 0, attrLocs);
	meshGLVAOInitialize(&meshL, 1, sdwProg.attrLocs);
	meshDestroy(&mesh);
	if (sceneInitialize(&nodeW, 3, 1, &meshW, NULL, NULL) != 0)
		return 14;
	if (sceneInitialize(&nodeL, 3, 1, &meshL, NULL, NULL) != 0)
		return 16;
	if (sceneInitialize(&nodeT, 3, 1, &meshT, &nodeL, &nodeW) != 0)
		return 15;
	if (sceneInitialize(&nodeV, 3, 1, &meshV, NULL, &nodeT) != 0)
		return 13;
	if (sceneInitialize(&nodeH, 3, 1, &meshH, &nodeV, NULL) != 0)
		return 12;
	GLdouble trans[3] = {40.0, 28.0, 5.0};
	sceneSetTranslation(&nodeT, trans);
	vecSet(3, trans, 0.0, 0.0, 7.0);
	sceneSetTranslation(&nodeL, trans);
	GLdouble unif[3] = {0.0, 0.0, 0.0};
	sceneSetUniform(&nodeH, unif);
	sceneSetUniform(&nodeV, unif);
	sceneSetUniform(&nodeT, unif);
	sceneSetUniform(&nodeL, unif);
	vecSet(3, unif, 1.0, 1.0, 1.0);
	sceneSetUniform(&nodeW, unif);
	texTexture *tex;
	tex = &texH;
	sceneSetTexture(&nodeH, &tex);
	tex = &texV;
	sceneSetTexture(&nodeV, &tex);
	tex = &texW;
	sceneSetTexture(&nodeW, &tex);
	tex = &texT;
	sceneSetTexture(&nodeT, &tex);
	tex = &texL;
	sceneSetTexture(&nodeL, &tex);
	double waterTransl[3] = {-40.0, -28.0, -2.0};
	sceneSetTranslation(&nodeW, waterTransl);
	return 0;
}

void destroyScene(void) {
	texDestroy(&texH);
	texDestroy(&texV);
	texDestroy(&texW);
	texDestroy(&texT);
	texDestroy(&texL);
	meshGLDestroy(&meshH);
	meshGLDestroy(&meshV);
	meshGLDestroy(&meshW);
	meshGLDestroy(&meshT);
	meshGLDestroy(&meshL);
	sceneDestroyRecursively(&nodeH);
}

/* Returns 0 on success, non-zero on failure. Warning: If initialization fails 
midway through, then does not properly deallocate all resources. But that's 
okay, because the program terminates almost immediately after this function 
returns. */
int initializeCameraLight(void) {
    GLdouble vec[3] = {30.0, 30.0, 5.0};
	camSetControls(&cam, camPERSPECTIVE, M_PI / 6.0, 10.0, 768.0, 768.0, 100.0, 
		M_PI / 4.0, M_PI / 4.0, vec);
	lightSetType(&lightOne, lightSPOT);
	vecSet(3, vec, 45.0, 30.0, 20.0);
	lightShineFrom(&lightOne, vec, M_PI * 3.0 / 4.0, M_PI * 3.0 / 4.0);
	vecSet(3, vec, 0.3, 0.7, 0.8);
	lightSetColor(&lightOne, vec);
	vecSet(3, vec, 1.0, 0.0, 0.0);
	lightSetAttenuation(&lightOne, vec);
	lightSetSpotAngle(&lightOne, M_PI / 3.0);
	
	/* Sets the second light and initializes both shadow maps */
	lightSetType(&lightTwo, lightSPOT);
	vecSet(3, vec, 45.0, 40.0, 40.0);
	lightShineFrom(&lightTwo, vec, M_PI * 3.0 / 4.0, M_PI * 3.0 / 4.0);
	vecSet(3, vec, 0.9, 0.5, 0.3);
	lightSetColor(&lightTwo, vec);
	vecSet(3, vec, 1.0, 0.0, 0.0);
	lightSetAttenuation(&lightTwo, vec);
	lightSetSpotAngle(&lightTwo, M_PI / 2.5);
	/* Configure shadow mapping. */
	if (shadowProgramInitialize(&sdwProg, 3) != 0)
		return 1;
	if (shadowMapInitialize(&sdwMapOne, 1024, 1024) != 0)
		return 2;
	if (shadowMapInitialize(&sdwMapTwo, 1024, 1024) != 0)
		return 3;
	return 0;
}

/* Returns 0 on success, non-zero on failure. */
int initializeShaderProgram(void) {
	GLchar vertexCode[] = "\
		#version 140\n\
		uniform mat4 viewing;\
		uniform mat4 modeling;\
		uniform mat4 viewingSdwOne;\
		uniform mat4 viewingSdwTwo;\
		in vec3 position;\
		in vec2 texCoords;\
		in vec3 normal;\
		out vec3 fragPos;\
		out vec3 normalDir;\
		out vec2 st;\
		out vec4 fragSdwOne;\
		out vec4 fragSdwTwo;\
		void main(void) {\
			mat4 scaleBias = mat4(\
				0.5, 0.0, 0.0, 0.0, \
				0.0, 0.5, 0.0, 0.0, \
				0.0, 0.0, 0.5, 0.0, \
				0.5, 0.5, 0.5, 1.0);\
			vec4 worldPos = modeling * vec4(position, 1.0);\
			gl_Position = viewing * worldPos;\
			fragSdwOne = scaleBias * viewingSdwOne * worldPos;\
			fragSdwTwo = scaleBias * viewingSdwTwo * worldPos;\
			fragPos = vec3(worldPos);\
			normalDir = vec3(modeling * vec4(normal, 0.0));\
			st = texCoords;\
		}";
	GLchar fragmentCode[] = "\
		#version 140\n\
		uniform sampler2D texture0;\
		uniform vec3 specular;\
		uniform vec3 camPos;\
		uniform vec3 lightOnePos;\
		uniform vec3 lightOneCol;\
		uniform vec3 lightOneAtt;\
		uniform vec3 lightOneAim;\
		uniform float lightOneCos;\
		uniform vec3 lightTwoPos;\
		uniform vec3 lightTwoCol;\
		uniform vec3 lightTwoAtt;\
		uniform vec3 lightTwoAim;\
		uniform float lightTwoCos;\
		uniform sampler2DShadow textureSdwOne;\
		uniform sampler2DShadow textureSdwTwo;\
		in vec3 fragPos;\
		in vec3 normalDir;\
		in vec2 st;\
		in vec4 fragSdwOne;\
		in vec4 fragSdwTwo;\
		out vec4 fragColor;\
		void main(void) {\
			vec3 diffuseOne = vec3(texture(texture0, st));\
			vec3 litDirOne = normalize(lightOnePos - fragPos);\
			float diffIntOne, specIntOne = 0.0;\
			if (dot(lightOneAim, -litDirOne) < lightOneCos)\
				diffIntOne = 0.0;\
			else\
				diffIntOne = 1.0;\
			float sdwOne = textureProj(textureSdwOne, fragSdwOne);\
			diffIntOne *= sdwOne;\
			specIntOne *= sdwOne;\
			\
			vec3 diffuseTwo = vec3(texture(texture0, st));\
			vec3 litDirTwo = normalize(lightTwoPos - fragPos);\
			float diffIntTwo, specIntTwo = 0.0;\
			if (dot(lightTwoAim, -litDirTwo) < lightTwoCos)\
				diffIntTwo = 0.0;\
			else\
				diffIntTwo = 1.0;\
			float sdwTwo = textureProj(textureSdwTwo, fragSdwTwo);\
			diffIntTwo *= sdwTwo;\
			specIntTwo *= sdwTwo;\
			\
			vec3 diffReflOne = max(0.2, diffIntOne) * lightOneCol * diffuseOne;\
			vec3 specReflOne = specIntOne * lightOneCol * specular;\
			vec3 diffReflTwo = max(0.2, diffIntTwo) * lightTwoCol * diffuseTwo;\
			vec3 specReflTwo = specIntTwo * lightTwoCol * specular;\
			fragColor = vec4(diffReflOne + diffReflTwo + specReflOne + specReflTwo, 1.0);\
		}";
	program = makeProgram(vertexCode, fragmentCode);
	if (program != 0) {
		glUseProgram(program);
		attrLocs[0] = glGetAttribLocation(program, "position");
		attrLocs[1] = glGetAttribLocation(program, "texCoords");
		attrLocs[2] = glGetAttribLocation(program, "normal");
		viewingLoc = glGetUniformLocation(program, "viewing");
		modelingLoc = glGetUniformLocation(program, "modeling");
		unifLocs[0] = glGetUniformLocation(program, "specular");
		textureLocs[0] = glGetUniformLocation(program, "texture0");
		camPosLoc = glGetUniformLocation(program, "camPos");
		lightOnePosLoc = glGetUniformLocation(program, "lightOnePos");
		lightOneColLoc = glGetUniformLocation(program, "lightOneCol");
		lightOneAttLoc = glGetUniformLocation(program, "lightOneAtt");
		lightOneDirLoc = glGetUniformLocation(program, "lightOneAim");
		lightOneCosLoc = glGetUniformLocation(program, "lightOneCos");
		lightTwoPosLoc = glGetUniformLocation(program, "lightTwoPos");
		lightTwoColLoc = glGetUniformLocation(program, "lightTwoCol");
		lightTwoAttLoc = glGetUniformLocation(program, "lightTwoAtt");
		lightTwoDirLoc = glGetUniformLocation(program, "lightTwoAim");
		lightTwoCosLoc = glGetUniformLocation(program, "lightTwoCos");
		viewingSdwOneLoc = glGetUniformLocation(program, "viewingSdwOne");
		textureSdwOneLoc = glGetUniformLocation(program, "textureSdwOne");
		viewingSdwTwoLoc = glGetUniformLocation(program, "viewingSdwTwo");
		textureSdwTwoLoc = glGetUniformLocation(program, "textureSdwTwo");
	}
	return (program == 0);
}

void render(void) {
	GLdouble identity[4][4];
	mat44Identity(identity);
	/* Save the viewport transformation. */
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	/* For each shadow-casting light, render its shadow map using minimal 
	uniforms and textures. */
	GLint sdwTextureLocs[1] = {-1};
	shadowMapRender(&sdwMapOne, &sdwProg, &lightOne, -100.0, -1.0);
	sceneRender(&nodeH, identity, sdwProg.modelingLoc, 0, NULL, NULL, 1, 
		sdwTextureLocs);
	shadowMapUnrender();
		
	shadowMapRender(&sdwMapTwo, &sdwProg, &lightTwo, -100.0, -1.0);
	sceneRender(&nodeH, identity, sdwProg.modelingLoc, 0, NULL, NULL, 1, 
		sdwTextureLocs);
	/* Finish preparing the shadow maps, restore the viewport, and begin to 
	render the scene. */
	shadowMapUnrender();
	glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	camRender(&cam, viewingLoc);
	GLfloat vec[3];
	vecOpenGL(3, cam.translation, vec);
	glUniform3fv(camPosLoc, 1, vec);
	/* For each light, we have to connect it to the shader program, as always. 
	For each shadow-casting light, we must also connect its shadow map. */
	lightRender(&lightOne, lightOnePosLoc, lightOneColLoc, lightOneAttLoc, lightOneDirLoc, 
		lightOneCosLoc);
	shadowRender(&sdwMapOne, viewingSdwOneLoc, GL_TEXTURE7, 7, textureSdwOneLoc);
	
	lightRender(&lightTwo, lightTwoPosLoc, lightTwoColLoc, lightTwoAttLoc, lightTwoDirLoc, 
		lightTwoCosLoc);
	shadowRender(&sdwMapTwo, viewingSdwTwoLoc, GL_TEXTURE8, 8, textureSdwTwoLoc);
	GLuint unifDims[1] = {3};
	sceneRender(&nodeH, identity, modelingLoc, 1, unifDims, unifLocs, 0, 
		textureLocs);
	/* For each shadow-casting light, turn it off when finished rendering. */
	shadowUnrender(GL_TEXTURE7);
	shadowUnrender(GL_TEXTURE8);
}

int main(void) {
	double oldTime;
	double newTime = getTime();
    glfwSetErrorCallback(handleError);
    if (glfwInit() == 0) {
    	fprintf(stderr, "main: glfwInit failed.\n");
        return 1;
    }
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *window;
    window = glfwCreateWindow(768, 768, "Shadows", NULL, NULL);
    if (window == NULL) {
    	fprintf(stderr, "main: glfwCreateWindow failed.\n");
        glfwTerminate();
        return 2;
    }
    glfwSetWindowSizeCallback(window, handleResize);
    glfwSetKeyCallback(window, handleKey);
    glfwMakeContextCurrent(window);
    if (gl3wInit() != 0) {
    	fprintf(stderr, "main: gl3wInit failed.\n");
    	glfwDestroyWindow(window);
    	glfwTerminate();
    	return 3;
    }
    fprintf(stderr, "main: OpenGL %s, GLSL %s.\n", 
		glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	/* We no longer do glDepthRange(1.0, 0.0). Instead we have changed our 
	projection matrices. */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    if (initializeShaderProgram() != 0)
    	return 3;
    /* Initialize the shadow mapping before the meshes. Why? */
	if (initializeCameraLight() != 0)
		return 4;
    if (initializeScene() != 0)
    	return 5;
    while (glfwWindowShouldClose(window) == 0) {
    	oldTime = newTime;
    	newTime = getTime();
    	if (floor(newTime) - floor(oldTime) >= 1.0)
			fprintf(stderr, "main: %f frames/sec\n", 1.0 / (newTime - oldTime));
		render();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    /* Deallocate more resources than ever. */
    shadowProgramDestroy(&sdwProg);
    shadowMapDestroy(&sdwMapOne);
    shadowMapDestroy(&sdwMapTwo);
    glDeleteProgram(program);
    destroyScene();
	glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
