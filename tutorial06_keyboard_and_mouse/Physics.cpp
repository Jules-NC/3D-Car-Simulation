#include "Physics.h"
#include <btBulletDynamicsCommon.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


void Physics::initPhysics()
{
	//==============================================================================================================================================
	// Build the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
	//btVector3 worldMin(-1000, -1000, -1000);
	//btVector3 worldMax(1000, 1000, 1000);
	//btAxisSweep3* overlappingPairCache = new btAxisSweep3(worldMin, worldMax);

	// The actual physics solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// The world.
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -0.1, 0));
	this->mDynamicsWorld = dynamicsWorld;
}

btDiscreteDynamicsWorld * Physics::getDynamicsWorld()
{
	return this->mDynamicsWorld;
}


glm::mat4 Physics::getModelMatrix(int i)
{
	btScalar linearTamp[16];
	this->mRigidBodies[i]->getWorldTransform().getOpenGLMatrix(linearTamp);
	glm::mat4 modelMatrix = glm::mat4(
		linearTamp[0], linearTamp[1], linearTamp[2], linearTamp[3],
		linearTamp[4], linearTamp[5], linearTamp[6], linearTamp[7],
		linearTamp[8], linearTamp[9], linearTamp[10], linearTamp[11],
		linearTamp[12], linearTamp[13], linearTamp[14], linearTamp[15]);

	return modelMatrix;
}

btRaycastVehicle * Physics::getVehicle(int i)
{
	return this->mVehicles[i];
}

int Physics::createGround(btVector3 shape)
{
	btCollisionShape* groundShape = new btBoxShape(btVector3(50, 3, 50));
	btDefaultMotionState* groundMS = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(.0f, -3.f, 0.f)));
	btRigidBody::btRigidBodyConstructionInfo groundCI(0, groundMS, groundShape, btVector3(0, 0, 0));
	btRigidBody *ground = new btRigidBody(groundCI);
	this->mDynamicsWorld->addRigidBody(ground);
	ground->setFriction(0.5);
	ground->activate(true);

	return this->addRigidBody(ground);
}

int Physics::createBox(float mass, btVector3 shape, btVector3 origin)
{
	btCollisionShape* boxCollisionShape = new btBoxShape(shape);
	btTransform boxTrans;
	boxTrans.setIdentity();
	boxTrans.setOrigin(origin);
	btVector3 boxInertia = btVector3(0, 0, 0);
	boxCollisionShape->calculateLocalInertia(mass, boxInertia);
	btDefaultMotionState* boxMS = new btDefaultMotionState(boxTrans);
	btRigidBody::btRigidBodyConstructionInfo boxCI(1, boxMS, boxCollisionShape, boxInertia);
	btRigidBody *box = new btRigidBody(boxCI);
	this->mDynamicsWorld->addRigidBody(box);
	box->activate(true);
	box->setActivationState(DISABLE_DEACTIVATION);

	return this->addRigidBody(box);

}

int Physics::createSphere(float mass, float radius, btVector3 origin)
{
	btCollisionShape* sphereCollisionShape = new btSphereShape(radius);
	btTransform sphereTrans;
	sphereTrans.setIdentity();
	sphereTrans.setOrigin(origin);
	btVector3 sphereInertia = btVector3(0, 0, 0);
	sphereCollisionShape->calculateLocalInertia(mass, sphereInertia);
	btDefaultMotionState* sphereMS = new btDefaultMotionState(sphereTrans);
	btRigidBody::btRigidBodyConstructionInfo sphereCI(1, sphereMS, sphereCollisionShape, sphereInertia);
	btRigidBody *sphere = new btRigidBody(sphereCI);
	this->mDynamicsWorld->addRigidBody(sphere);
	sphere->setFriction(1);
	sphere->setDamping(0.1, 0.01);
	sphere->activate(true);
	sphere->setActivationState(DISABLE_DEACTIVATION);

	return this->addRigidBody(sphere);
}

int Physics::createCar(float & gEngineForce, float & gBreakinForce, float & gVehicleSteering)
{
	#define CUBE_HALF_EXTENTS 1


	float	maxEngineForce = 30.f;//this should be engine/velocity dependent
	float	maxBreakingForce = 100.f;

	float	steeringIncrement = 0.04f;
	float	steeringClamp = 0.5f;
	float	wheelRadius = 0.5f;
	float	wheelWidth = 0.1f;
	float	wheelFriction = 1000;//BT_LARGE_FLOAT;
	float	suspensionStiffness = 20.f;
	float	suspensionDamping = 2.3f;
	float	suspensionCompression = 4.4f;
	float	rollInfluence = 0.1f;//1.0f;
	float carMass = 800.0f;
	int rightIndex = 0;
	int upIndex = 1;
	int forwardIndex = 2;

	btVector3 wheelDirectionCS0(0, -1, 0);
	btVector3 wheelAxleCS(-1, 0, 0);
	btScalar suspensionRestLength(0.6);

	btCollisionShape* chassisShape = new btBoxShape(btVector3(1.f, 0.5f, 2.f));
	//m_collisionShapes.push_back(chassisShape);

	btCompoundShape* compound = new btCompoundShape();
	//m_collisionShapes.push_back(compound);
	btTransform localTrans;
	localTrans.setIdentity();
	//localTrans effectively shifts the center of mass with respect to the chassis
	localTrans.setOrigin(btVector3(0, 1, 0));
	compound->addChildShape(localTrans, chassisShape);
	//btTransform tr;
	//tr.setIdentity();
	localTrans.setOrigin(btVector3(3, 0, 0));//-64.5f,0));
	btVector3 vehicleInertia = btVector3(0, 0.0, 0.0);

	compound->calculateLocalInertia(carMass, vehicleInertia);
	btRigidBody::btRigidBodyConstructionInfo vehicleCI(
		carMass,                  // mass, in kg. 0 -> Static object, will never move.
		new btDefaultMotionState(localTrans),
		compound,  // collision shape of body
		vehicleInertia // local inertia
	);

	btRigidBody *carChassis = new btRigidBody(vehicleCI);
	this->mDynamicsWorld->addRigidBody(carChassis);

	btCollisionShape* wheelShape = new btCylinderShapeX(btVector3(wheelWidth, wheelRadius, wheelRadius));
	gVehicleSteering = 0.f;
	carChassis->setCenterOfMassTransform(btTransform::getIdentity());
	carChassis->setLinearVelocity(btVector3(0, 0, 0));
	carChassis->setAngularVelocity(btVector3(0, 0, 0));
	Physics::getInstance().getDynamicsWorld()->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(carChassis->getBroadphaseHandle(), this->mDynamicsWorld->getDispatcher());

	btDefaultVehicleRaycaster * vehicleRayCaster = new btDefaultVehicleRaycaster(this->mDynamicsWorld);

	btRaycastVehicle * vehicle = new btRaycastVehicle(btRaycastVehicle::btVehicleTuning(), carChassis, vehicleRayCaster);
	carChassis->setActivationState(DISABLE_DEACTIVATION);
	this->mDynamicsWorld->addVehicle(vehicle);
	float connectionHeight = 1.2f;

	vehicle->setCoordinateSystem(rightIndex, upIndex, forwardIndex);

	btVector3 connection(CUBE_HALF_EXTENTS - (0.3*wheelWidth), connectionHeight, 2 * CUBE_HALF_EXTENTS - wheelRadius);
	vehicle->addWheel(connection, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, btRaycastVehicle::btVehicleTuning(), true);
	connection = btVector3(-CUBE_HALF_EXTENTS + (0.3*wheelWidth), connectionHeight, 2 * CUBE_HALF_EXTENTS - wheelRadius);
	vehicle->addWheel(connection, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, btRaycastVehicle::btVehicleTuning(), true);
	connection = btVector3(-CUBE_HALF_EXTENTS + (0.3*wheelWidth), connectionHeight, -2 * CUBE_HALF_EXTENTS + wheelRadius);
	vehicle->addWheel(connection, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, btRaycastVehicle::btVehicleTuning(), false);
	connection = btVector3(CUBE_HALF_EXTENTS - (0.3*wheelWidth), connectionHeight, -2 * CUBE_HALF_EXTENTS + wheelRadius);
	vehicle->addWheel(connection, wheelDirectionCS0, wheelAxleCS, suspensionRestLength, wheelRadius, btRaycastVehicle::btVehicleTuning(), false);
	for (int i = 0; i < vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
	}

	return this->addVehicle(vehicle);
}


glm::mat4 Physics::getChassisMatrix(int i)
{
	btScalar chassisMatrix[16];
	this->mVehicles[i]->getChassisWorldTransform().getOpenGLMatrix(chassisMatrix);
	glm::mat4 chassisModelMatrix = glm::mat4(
		chassisMatrix[0], chassisMatrix[1], chassisMatrix[2], chassisMatrix[3],
		chassisMatrix[4], chassisMatrix[5], chassisMatrix[6], chassisMatrix[7],
		chassisMatrix[8], chassisMatrix[9], chassisMatrix[10], chassisMatrix[11],
		chassisMatrix[12], chassisMatrix[13], chassisMatrix[14], chassisMatrix[15]);
	return chassisModelMatrix;
}

std::vector<glm::mat4> Physics::getWheelsMatrix(int i)
{
	auto wheelsMatrix = std::vector<glm::mat4>();
	btScalar wheelMatrix[16];
	btRaycastVehicle * vehicle = this->getVehicle(i);

	for (int i = 0; i < vehicle->getNumWheels(); i++) {
		vehicle->getWheelInfo(i).m_worldTransform.getOpenGLMatrix(wheelMatrix);
		glm::mat4 wheelModelMatrix = glm::mat4(
			wheelMatrix[0], wheelMatrix[1], wheelMatrix[2], wheelMatrix[3],
			wheelMatrix[4], wheelMatrix[5], wheelMatrix[6], wheelMatrix[7],
			wheelMatrix[8], wheelMatrix[9], wheelMatrix[10], wheelMatrix[11],
			wheelMatrix[12], wheelMatrix[13], wheelMatrix[14], wheelMatrix[15]);
		wheelsMatrix.push_back(wheelModelMatrix);
	}
	return wheelsMatrix;
}

void Physics::applyForcesOnCar(int i, float gEngineForce, float gBreakingForce, float gVehicleSteering)
{
	btRaycastVehicle* vehicle = this->getVehicle(i);
	int wheelIndex = 2;
	vehicle->applyEngineForce(gEngineForce, wheelIndex);
	vehicle->setBrake(gBreakingForce, wheelIndex);
	wheelIndex = 3;
	vehicle->applyEngineForce(gEngineForce, wheelIndex);
	vehicle->setBrake(gBreakingForce, wheelIndex);


	wheelIndex = 0;
	vehicle->setSteeringValue(gVehicleSteering, wheelIndex);
	vehicle->applyEngineForce(gEngineForce, wheelIndex);
	vehicle->setBrake(gBreakingForce, wheelIndex);
	wheelIndex = 1;
	vehicle->setSteeringValue(gVehicleSteering, wheelIndex);
	vehicle->applyEngineForce(gEngineForce, wheelIndex);
	vehicle->setBrake(gBreakingForce, wheelIndex);
}

int Physics::addRigidBody(btRigidBody * body)
{
	this->mRigidBodies.push_back(body);
	return this->mRigidBodies.size() - 1;
}

int Physics::addVehicle(btRaycastVehicle * vehicle)
{
	this->mVehicles.push_back(vehicle);
	return this->mVehicles.size() - 1;
}
