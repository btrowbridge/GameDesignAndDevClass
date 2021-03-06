#include "pch.h"
#include "SimpleCollisionGame.h"

using namespace std;
using namespace DirectX;
using namespace Library;
using namespace Microsoft::WRL;

namespace SimpleCollision
{
	const XMVECTORF32 SimpleCollisionGame::BackgroundColor = Colors::Black;

	SimpleCollisionGame::SimpleCollisionGame(function<void*()> getWindowCallback, function<void(SIZE&)> getRenderTargetSizeCallback) :
		Game(getWindowCallback, getRenderTargetSizeCallback), mOnFloorCollide(), RecyclingBin(), mShapeCount(0)
	{
	}

	void SimpleCollisionGame::Initialize()
	{
		mKeyboard = make_shared<KeyboardComponent>(*this);
		mComponents.push_back(mKeyboard);
		mServices.AddService(KeyboardComponent::TypeIdClass(), mKeyboard.get());

		mPhysicsEngine = make_shared<Library::Box2DComponent>(*this);
		mComponents.push_back(mPhysicsEngine);
		mServices.AddService(Box2DComponent::TypeIdClass(), mPhysicsEngine.get());

		auto fpsCom = make_shared<UIComponent>(*this);
		mComponents.push_back(fpsCom);

		auto mCamera = make_shared<OrthographicCamera>(*this);
		mComponents.push_back(mCamera);

		mDebugDraw = make_shared<DebugDraw>(*this, mCamera);
		mComponents.push_back(mDebugDraw);

		mPhysicsEngine->World().SetDebugDraw(mDebugDraw.get());

		SpriteManager::Initialize(*this);

		Game::Initialize();

		mDebugDraw->AppendFlags(b2Draw::e_shapeBit);
		//mDebugDraw->AppendFlags(b2Draw::e_aabbBit);

		mCamera->SetPosition(0.0f, 0.0f, 5.0f);

		SetScene();
		SetListeners();
	}

	void SimpleCollisionGame::Update(const GameTime &gameTime)
	{
		if (mKeyboard->WasKeyPressedThisFrame(Keys::Q) || mKeyboard->IsKeyDown(Keys::R))
		{
			AddBox(0.0f, 10.0f);
			mShapeCount++;
		}
		
		if (mKeyboard->WasKeyPressedThisFrame(Keys::W) || mKeyboard->IsKeyDown(Keys::R)) 
		{
			AddCircle(0.0f, 10.0f);
			mShapeCount++;
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::E) || mKeyboard->IsKeyDown(Keys::R))
		{
			AddTriangle(0.0, 10.0f);
			mShapeCount++;
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::Back)) {
			ClearShapes();
		}

		if (mKeyboard->WasKeyPressedThisFrame(Keys::Escape))
		{
			Exit();
		}

		Game::Update(gameTime);

		EmptyBin();
	}

	void SimpleCollisionGame::Draw(const GameTime &gameTime)
	{
		mDirect3DDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), reinterpret_cast<const float*>(&BackgroundColor));
		mDirect3DDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		mPhysicsEngine->World().DrawDebugData();

		Game::Draw(gameTime);

		HRESULT hr = mSwapChain->Present(1, 0);

		// If the device was removed either by a disconnection or a driver upgrade, we must recreate all device resources.
		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
		{
			HandleDeviceLost();
		}
		else
		{
			ThrowIfFailed(hr, "IDXGISwapChain::Present() failed.");
		}
	}

	void SimpleCollisionGame::Shutdown()
	{
	}

	void SimpleCollisionGame::AddBox(float x, float y)
	{
		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		bodyDef.userData = new MyShapeData();


		b2Body* body = mPhysicsEngine->World().CreateBody(&bodyDef);

		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(1.0f, 1.0f);

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 1.0f;

		// Override the default friction.
		fixtureDef.friction = 0.3f;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);

		ApplyControlledForce(body);

	}

	void SimpleCollisionGame::AddCircle(float x, float y)
	{
		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		bodyDef.userData = new MyShapeData();


		b2Body* body = mPhysicsEngine->World().CreateBody(&bodyDef);

		// Define another box shape for our dynamic body.
		b2CircleShape dynamicCircle;
		dynamicCircle.m_radius = 1.0f;

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicCircle;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 1.0f;

		// Override the default friction.
		fixtureDef.friction = 0.3f;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);
		
		ApplyControlledForce(body);
	}

	void SimpleCollisionGame::AddTriangle(float x, float y)
	{
		// Define the dynamic body. We set its position and call the body factory.
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(x, y);
		bodyDef.userData = new MyShapeData();


		b2Body* body = mPhysicsEngine->World().CreateBody(&bodyDef);

		// Define another box shape for our dynamic body.
		b2PolygonShape dynamicTriangle;
		b2Vec2 vertices[3];
		vertices[0].Set(0.0f, 1.0f);
		vertices[1].Set(-1.0f, -1.0f);
		vertices[2].Set(1.0f, -1.0f);
		int count = 3;

		dynamicTriangle.Set(vertices, count);

		// Define the dynamic body fixture.
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicTriangle;

		// Set the box density to be non-zero, so it will be dynamic.
		fixtureDef.density = 1.0f;

		// Override the default friction.
		fixtureDef.friction = 0.3f;

		// Add the shape to the body.
		body->CreateFixture(&fixtureDef);

		ApplyControlledForce(body);

	}

	void SimpleCollisionGame::AddPlatform(float x, float y)
	{
		// Define the ground body.
		b2BodyDef groundBodyDef;
		groundBodyDef.position.Set(x, y);

		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body* groundBody = mPhysicsEngine->World().CreateBody(&groundBodyDef);

		// Define the ground box shape.
		b2PolygonShape groundBox;

		// The extents are the half-widths of the box.
		groundBox.SetAsBox(20.0f, 2.5f);

		// Add the ground fixture to the ground body.
		groundBody->CreateFixture(&groundBox, 0.0f);
	}

	void SimpleCollisionGame::AddWall(float x, float y, b2Vec2 direction)
	{
		// Define the ground body.
		b2BodyDef wallBodyDef;
		wallBodyDef.position.Set(x, y);

		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body* wallBody = mPhysicsEngine->World().CreateBody(&wallBodyDef);

		// Define the ground box shape.
		b2EdgeShape wallEdge;

		// The extents are the half-widths of the box.
		b2Vec2 v0(0.0f, 0.0f);
		b2Vec2 v1 = direction;

		wallEdge.Set(v0, v1);

		// Add the ground fixture to the ground body.
		wallBody->CreateFixture(&wallEdge, 0.0f);
	}

	void SimpleCollisionGame::AddFloor(float x, float y)
	{
		// Define the ground body.
		b2BodyDef floorBodyDef;
		floorBodyDef.position.Set(x, y);

		// Call the body factory which allocates memory for the ground body
		// from a pool and creates the ground box shape (also from a pool).
		// The body is also added to the world.
		b2Body* floorBody = mPhysicsEngine->World().CreateBody(&floorBodyDef);

		// Define the ground box shape.
		b2EdgeShape floorEdge;

		// The extents are the half-widths of the box.
		b2Vec2 v0(-50.0f, 0.0f);
		b2Vec2 v1(50.0f, 0.0f);
		floorEdge.Set(v0, v1);

		b2FixtureDef floorFixtureDef;
		floorFixtureDef.isSensor = true;
		floorFixtureDef.shape = &floorEdge;

		// Add the ground fixture to the ground body.
		floorBody->CreateFixture(&floorFixtureDef);
	}

	void SimpleCollisionGame::ApplyControlledForce(b2Body* body)
	{
		float magnitude = 500.0f;
		float xDir = 0;
		float yDir = 0;

		if(mKeyboard->IsKeyDown(Keys::Up))
		{
			yDir = 1.0f;
		}
		else if (mKeyboard->IsKeyDown(Keys::Down))
		{
			yDir = -1.0f;
		}

		if (mKeyboard->IsKeyDown(Keys::Left) )
		{
			xDir = -1.0f;
		}
		else if (mKeyboard->IsKeyDown(Keys::Right))
		{
			xDir = 1.0f;
		}
		b2Vec2 myForce(xDir* magnitude, yDir* magnitude);
		body->ApplyLinearImpulseToCenter(myForce , true);

	}

	void SimpleCollisionGame::Exit()
	{
		PostQuitMessage(0);
	}

	void SimpleCollisionGame::EmptyBin()
	{
		if (mShapeCount <= 0)
			return;

		auto destroyIt =RecyclingBin.begin();
		while(destroyIt != RecyclingBin.end() && !RecyclingBin.empty())
		{
			auto dyingBox = *destroyIt;
			//delete box... physics body is destroyed here
			if (dyingBox != nullptr)
				dyingBox->GetWorld()->DestroyBody(dyingBox);
				
			dyingBox = nullptr;
			mShapeCount--;
			//... and remove it from main list of boxes
			destroyIt = RecyclingBin.erase(destroyIt);
		}
		//clear this list for next time
		RecyclingBin.clear();	
	}
	void SimpleCollisionGame::SetScene()
	{
		AddPlatform(0.0f, -20.0f);
		AddWall(40.0f, -30.0f, b2Vec2(-25.0f, 40.0f));
		AddWall(-20.0f, -20.0f, b2Vec2(0.0f, 40.0f));
		AddFloor(0.0f, -40.0f);
	}
	void SimpleCollisionGame::SetListeners()
	{
		auto ThrowAway = [&](b2Body* b) {
			auto bData = reinterpret_cast<MyShapeData*>(b->GetUserData());
			if (!bData->isDying) {
				bData->isDying = true;
				RecyclingBin.push_back(b);
			}
		};

		mOnFloorCollide.SetCallback(ThrowAway);
		mPhysicsEngine->World().SetContactListener(&mOnFloorCollide);
	}

	int SimpleCollisionGame::ShapeCount() const
	{
		return mShapeCount;
	}

	void SimpleCollisionGame::ClearShapes()
	{
		b2Body * body = mPhysicsEngine->World().GetBodyList();
		int numBodies = mPhysicsEngine->World().GetBodyCount();
		for (int iBody = 0; iBody < numBodies; iBody++) {

			if (body == nullptr)
				return;

			auto bData = reinterpret_cast<MyShapeData*>(body->GetUserData());

			if (body->GetType() == b2_dynamicBody && !bData->isDying) {
				auto temp = body->GetNext();
				bData->isDying = true;
				RecyclingBin.push_back(body);
				body = temp;
			}
			
		}
	}
}