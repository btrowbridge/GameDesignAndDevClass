#include "pch.h"
#include "Box2DComponent.h"

using namespace std;
using namespace DirectX;

namespace Library
{
	RTTI_DEFINITIONS(Box2DComponent)

	const b2Vec2 Box2DComponent::DefaultGravity = { 0.0f, -9.8f };
	const float Box2DComponent::DefaultTimeStep = 1.0f / 60.0f;
	const int32_t Box2DComponent::DefaultVelocityIterations = 8;
	const int32_t Box2DComponent::DefaultPositionIterations = 3;

	Box2DComponent::Box2DComponent(Game& game, const b2Vec2& gravity, float timeStep, int32_t velocityIterations, int32_t positionIterations) :
		GameComponent(game), mGravity(gravity), mWorld(make_unique<b2World>(gravity)), mTimeStep(timeStep),
		mVelocityIterations(velocityIterations), mPositionIterations(positionIterations)
	{
	}

	const b2Vec2& Box2DComponent::Gravity() const
	{
		return mGravity;
	}

	b2World& Box2DComponent::World()
	{
		return *mWorld;
	}

	float Box2DComponent::TimeStep() const
	{
		return mTimeStep;
	}

	void Box2DComponent::SetTimeStep(float timeStep)
	{
		mTimeStep = timeStep;
	}

	int32_t Box2DComponent::VelocityIterations() const
	{
		return mVelocityIterations;
	}

	void Box2DComponent::SetVelocityIterations(int32_t velocityIterations)
	{
		mVelocityIterations = velocityIterations;
	}

	int32_t Box2DComponent::PositionIterations() const
	{
		return mPositionIterations;
	}

	void Box2DComponent::SetPositionIterations(int32_t positionIterations)
	{
		mPositionIterations = positionIterations;
	}

	void Box2DComponent::Clear()
	{
		mWorld = make_unique<b2World>(mGravity);
		mJointsScheduledForDestruction.clear();
		mCemetery.clear();
	}

	const vector<b2Joint*>& Box2DComponent::JointsScheduledForDestruction() const
	{
		return mJointsScheduledForDestruction;
	}

	void Box2DComponent::ScheduleJointForDestruction(b2Joint& joint)
	{
		mJointsScheduledForDestruction.push_back(&joint);
	}

	const vector<b2Body*>& Box2DComponent::Cemetery() const
	{
		return mCemetery;
	}

	void Box2DComponent::BuryBody(b2Body& body)
	{
		mCemetery.push_back(&body);
	}

	void Box2DComponent::SetBodyDestroyedCallback(function<void(b2Body*)> callback)
	{
		mBodyDestroyedCallback = callback;
	}
	
	void Box2DComponent::Update(const GameTime& gameTime)
	{
		UNREFERENCED_PARAMETER(gameTime);

		mWorld->Step(mTimeStep, mVelocityIterations, mPositionIterations);

		// Destroy any joints schedule for destruction
		for (b2Joint* joint : mJointsScheduledForDestruction)
		{
			mWorld->DestroyJoint(joint);
		}
		mJointsScheduledForDestruction.clear();

		// Destroy any bodies schedule for destruction
		for (b2Body* body : mCemetery)
		{
			OnBodyDestroyed(body);
			mWorld->DestroyBody(body);
		}
		mCemetery.clear();
	}

	void Box2DComponent::OnBodyDestroyed(b2Body* body)
	{
		if (mBodyDestroyedCallback)
		{
			mBodyDestroyedCallback(body);
		}
	}	
}