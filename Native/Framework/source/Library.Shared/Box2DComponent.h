#pragma once

#include "GameComponent.h"
#include <memory>
#include <vector>
#include <functional>
#include <Box2D\Box2D.h>

namespace Library
{
	class Box2DComponent final : public GameComponent
	{
		RTTI_DECLARATIONS(Box2DComponent, GameComponent)

	public:
		Box2DComponent(Game& game, const b2Vec2& gravity = DefaultGravity, float timeStep = DefaultTimeStep, std::int32_t velocityIterations = DefaultVelocityIterations, std::int32_t positionIterations = DefaultPositionIterations);

		const b2Vec2& Gravity() const;
		b2World& World();
		float TimeStep() const;
		void SetTimeStep(float timeStep);
		std::int32_t VelocityIterations() const;
		void SetVelocityIterations(std::int32_t velocityIterations);
		std::int32_t PositionIterations() const;
		void SetPositionIterations(std::int32_t positionIterations);
		void Clear();

		const std::vector<b2Joint*>& JointsScheduledForDestruction() const;
		void ScheduleJointForDestruction(b2Joint& joint);

		const std::vector<b2Body*>& Cemetery() const;
		void BuryBody(b2Body& body);
		void SetBodyDestroyedCallback(std::function<void(b2Body*)> callback);

		virtual void Update(const GameTime& gameTime) override;

	private:
		void OnBodyDestroyed(b2Body* body);

		static const b2Vec2 DefaultGravity;
		static const float DefaultTimeStep;
		static const std::int32_t DefaultVelocityIterations;
		static const std::int32_t DefaultPositionIterations;

		b2Vec2 mGravity;
		std::unique_ptr<b2World> mWorld;
		float mTimeStep;
		std::int32_t mVelocityIterations;
		std::int32_t mPositionIterations;
		std::vector<b2Joint*> mJointsScheduledForDestruction;
		std::vector<b2Body*> mCemetery;
		std::function<void(b2Body*)> mBodyDestroyedCallback;
	};
}