#include "Render.h"

Render::Render() {
    this->objectList = std::vector<Object3D*>();
	this->lightList = std::vector<Light*>();
    Render::r = this;
}

void Render::initGL(const char* windowName, int sizeX, int sizeY) {
    if (glfwInit() != GLFW_TRUE) {
        std::cout << "ERROR GLFW no iniciada\n";
        exit(0);
    }

    #ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif

    this->window = glfwCreateWindow(sizeX, sizeY, windowName, NULL, NULL);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPos(window, 0.0, 0.0);
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
            
    InputManager::initInputManager(window);

    glEnable(GL_DEPTH_TEST);
}

void Render::putObject(Object3D* obj) {
    // objectList.push_back(obj);
    setupObject(obj);
    nextObjectList.push_back(obj);
}

void Render::putCamera(Camera* cam) {
    this->cam = cam;
}

void Render::drawGL(int objectPosition, const std::vector<int>& lightPositions) {
    libMath::mat4x4f modelMat = (this->objectList[objectPosition]->modelMatrix);
    libMath::mat4x4f viewMat = (this->cam->computeViewMatrix());
    libMath::mat4x4f projectionMat = (this->cam->computeProjectionMatrix());

    libMath::mat4x4f MVP = libMath::transpose(projectionMat * viewMat * modelMat);

    bufferObject_t bo = bufferObjectList[this->objectList[objectPosition]->id];
    this->objectList[objectPosition]->prg->use();
    glBindVertexArray(bo.bufferID);
    glBindBuffer(GL_ARRAY_BUFFER, bo.vertexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.edgeBufferID);

    this->objectList[objectPosition]->prg->setMVP(MVP);
    this->objectList[objectPosition]->prg->setMatrix("uniformM", libMath::transpose(modelMat));

    this->objectList[objectPosition]->prg->setFloat("material_ambient_reflectivity", this->objectList[objectPosition]->mat.Ka);
    this->objectList[objectPosition]->prg->setFloat("material_diffuse_reflectivity", this->objectList[objectPosition]->mat.Kd);
    this->objectList[objectPosition]->prg->setFloat("material_specular_reflectivity", this->objectList[objectPosition]->mat.Ks);
    this->objectList[objectPosition]->prg->setInteger("material_shininess", this->objectList[objectPosition]->mat.shinny);

    for (int i = 0; i < lightPositions.size(); i++) {
        int lightPosition = lightPositions[i];
        this->objectList[objectPosition]->prg->setVec4f("light_position" + std::to_string(i), this->lightList[lightPosition]->pos);
        this->objectList[objectPosition]->prg->setVec4f("light_color" + std::to_string(i), this->lightList[lightPosition]->color);
        this->objectList[objectPosition]->prg->setVec4f("light_direction" + std::to_string(i), this->lightList[lightPosition]->direction);
        this->objectList[objectPosition]->prg->setFloat("light_ambient_intensity" + std::to_string(i), this->lightList[lightPosition]->Ia);
        this->objectList[objectPosition]->prg->setFloat("light_diffuse_intensity" + std::to_string(i), this->lightList[lightPosition]->Id);
        this->objectList[objectPosition]->prg->setFloat("light_specular_intensity" + std::to_string(i), this->lightList[lightPosition]->Is);
        this->objectList[objectPosition]->prg->setInteger("light_type" + std::to_string(i), (int)this->lightList[lightPosition]->type);
        this->objectList[objectPosition]->prg->setInteger("light_enable" + std::to_string(i), this->lightList[lightPosition]->enable);
    }

    this->objectList[objectPosition]->prg->setVertexAttribute("attributeVertPos", 4, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, vertexPos));
    this->objectList[objectPosition]->prg->setVertexAttribute("attributeVertColor", 4, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, vertexColor));
    this->objectList[objectPosition]->prg->setVertexAttribute("attributeVertNormal", 4, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, vertexNormal));
    this->objectList[objectPosition]->prg->setVertexAttribute("attributeVertUv", 4, GL_FLOAT, sizeof(vertex_t), (void*)offsetof(vertex_t, vertexUv));

    this->objectList[objectPosition]->prg->setInteger("textureColor", 0);
    if (this->objectList[objectPosition]->tex != nullptr) {
        this->objectList[objectPosition]->prg->setInteger("material_texture_enabled", 1);
        this->objectList[objectPosition]->tex->bind(0);
    }
    else {
        this->objectList[objectPosition]->prg->setInteger("material_texture_enabled", 0);
    }

    glDrawElements(GL_TRIANGLES, this->objectList[objectPosition]->idList.size(), GL_UNSIGNED_INT, nullptr);
}

void Render::setupObject(Object3D* obj) {
    bufferObject_t bo;

    glGenVertexArrays(1, &bo.bufferID);
    glBindVertexArray(bo.bufferID);
    
    glGenBuffers(1, &bo.vertexBufferID);
    glGenBuffers(1, &bo.edgeBufferID);

    glBindBuffer(GL_ARRAY_BUFFER, bo.vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_t) * obj->vertexList.size(), obj->vertexList.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bo.edgeBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * obj->idList.size(), obj->idList.data(), GL_STATIC_DRAW);

    bufferObjectList[obj->id] = bo;
}

void Render::move(float deltaTime) {
    /*
    for (auto& obj : this->objectList) {
        if (obj->coll != nullptr) obj->coll->update(obj->modelMatrix);
        else if (obj->sphrColl != nullptr) obj->sphrColl->update(obj->modelMatrix, obj->position);
        obj->move(deltaTime);
        if (obj->active) nextObjectList.push_back(obj);
    }
    */

    for (auto& obj : objectList)
    {
        if (obj->coll != nullptr) obj->coll->update(obj->modelMatrix);
        else if (obj->sphrColl != nullptr) obj->sphrColl->update(obj->modelMatrix, obj->position);
    }

    for (auto& obj : objectList) if (obj != nullptr) obj->move(deltaTime);
    for (auto& obj : objectList) if (obj->active) nextObjectList.push_back(obj);
        
    objectList = nextObjectList;
    nextObjectList.clear();
}

void Render::drawObjects() {
    for (int objId = 0; objId < this->objectList.size(); objId++) {
        std::vector<int> activeLights;
        for (int lightId = 0; lightId < this->lightList.size(); lightId++) {
            if (this->lightList[lightId]->enable) activeLights.push_back(lightId);
        }
        drawGL(objId, activeLights);
    }
}

void Render::putLight(Light* light) {
	this->lightList.push_back(light);
}

void Render::moveLights(float deltaTime) {
    for (auto& light : this->lightList) {
		light->move(deltaTime);
	}
}

bool Render::collisionType(Object3D* obj, int objType)
{
    for (auto& o : this->objectList) {
        if (o->type == objType && o != obj && obj->coll->collision(o->coll)) {
            this->r->deleteObject(o);
            return true;
        }
	}
    return false;
}

bool Render::cameraCollision(int objType)
{
    for (auto& o : this->objectList) if (o->type == objType && o->sphrColl->collision(cam->coll)) return true;
    return false;
}

void Render::deleteObject(Object3D* obj){
    obj->active = false;
}

void Render::mainLoop() {   
    float newTime = 0;
    float lastTime = 0;
    float deltaTime = 0;
            
    while (!glfwWindowShouldClose(this->window) && !InputManager::keysState[GLFW_KEY_ESCAPE] && !r->gameOver) {    
        newTime = static_cast<float>(glfwGetTime());
        deltaTime = newTime - lastTime;
        lastTime = newTime;
        //glClearColor(0, 0, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwPollEvents();

        this->cam->move(deltaTime);
        moveLights(deltaTime);
        move(deltaTime);

        drawObjects();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
}