#pragma once

#include "Common.h"

#include "Vector4f.h" // con vertex.h o Mat4x4f.h ya estaria incluido pero lo dejo por si acaso
#include "Mat4x4f.h"
#include "vertex.h"
#include "InputManager.h"
#include "Quaternion4f.h"
#include "RenderProgram.h"
#include "Texture.h"
#include "collider.h"

/**
 * @class Object3D
 * @brief Represents a 3D object in a graphics program.
 */
class Object3D{
	private:
		static inline int idCounter = 0;
	
	public:
		int id = 0;
		int type = 0;
		bool active = true;


		libMath::vector4f position;
		libMath::vector4f rotation;
		libMath::vector4f scale;

		Collider* coll = nullptr;
		SphereCollider* sphrColl = nullptr;

		std::vector<vertex_t> vertexList;
		std::vector<int> idList;

		libMath::mat4x4f modelMatrix;

		Program* prg = new Program();
		material_t mat = { 1.0f, 1.0f, 1.0f, 70 };
		Texture* tex = nullptr;

		/**
		 * @brief Default constructor for Object3D.
		 */
		Object3D();

		Object3D(std::string fileName, libMath::vector4f pos);

		void createCollider();

		void createSphereCollider();
		
		/**
		 * @brief Loads object data from a file.
		 * @param fileName The name of the file to load from.
		 */
		void loadFromFile(const char* fileName);
		
		/**
		 * @brief Creates a cube object.
		 */
		void createCube();
		
		/**
		 * @brief Creates a triangle object.
		 */
		void createTriangle();
		
		/**
		 * @brief Moves the object based on the specified time step.
		 * @param timeStep The time step to move the object by.
		 */
		virtual void move(double timeStep);
		
		/**
		 * @brief Updates the model matrix of the object.
		 */
		void updateModelMatrix();
};

