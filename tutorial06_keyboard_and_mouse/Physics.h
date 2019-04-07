#pragma once
#include <vector>

#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <GL/glew.h>					// |glew


class Physics
{
public:
	static Physics& getInstance() {
		static Physics instance;
		return instance;
	}

	void initPhysics();
	btDiscreteDynamicsWorld* getDynamicsWorld();

	glm::mat4 getModelMatrix(int i);
	btRaycastVehicle * getVehicle(int i);

	int createGround(btVector3 shape);
	int createBox(float mass, btVector3 shape, btVector3 origin);
	int createSphere(float mass, float radius, btVector3 origin);

	// CAR PUTAIN DE MERDE D'OBJET BEACOUP TROP LOURD
	int createCar(float & gEngineForce, float & gBreakinForce, float & gVehicleSteering);
	glm::mat4 getChassisMatrix(int i);
	std::vector<glm::mat4> getWheelsMatrix(int i);
	void applyForcesOnCar(int i, float gEngineForce, float gBreakingForce, float gVehicleSteering);

private:
	Physics() = default;
	~Physics() = default;
	Physics(const Physics&) = delete;
	Physics& operator=(const Physics&) = delete;

	int addRigidBody(btRigidBody * body);
	int addVehicle(btRaycastVehicle * vehicle);
	btDiscreteDynamicsWorld* mDynamicsWorld;
	std::vector<btRigidBody *> mRigidBodies;
	std::vector<btRaycastVehicle *> mVehicles;
};


// DEBUG
class BulletDebugDrawer_DeprecatedOpenGL : public btIDebugDraw {
public:
	void SetMatrices(glm::mat4 pViewMatrix, glm::mat4 pProjectionMatrix) {
		glUseProgram(0); // Use Fixed-function pipeline (no shaders)
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(&pViewMatrix[0][0]);
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(&pProjectionMatrix[0][0]);
	}
	virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) {
		glColor3f(color.x(), color.y(), color.z());
		glBegin(GL_LINES);
		glVertex3f(from.x(), from.y(), from.z());
		glVertex3f(to.x(), to.y(), to.z());
		glEnd();
	}
	virtual void drawContactPoint(const btVector3 &, const btVector3 &, btScalar, int, const btVector3 &) {}
	virtual void reportErrorWarning(const char *) {}
	virtual void draw3dText(const btVector3 &, const char *) {}
	virtual void setDebugMode(int p) {
		m = p;
	}
	int getDebugMode(void) const { return 3; }
	int m;
};
