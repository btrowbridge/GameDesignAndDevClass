#pragma once

// Windows
#include <windows.h>
#include <wrl.h>
#include <SDKDDKVer.h>

// Standard
#include <exception>
#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>
#include <map>
#include <stack>
#include <cstdint>
#include <iomanip>
#include <codecvt>
#include <algorithm>
#include <functional>
#include <iterator>

#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

// DirectX
#include <d3d11_2.h>
#include <dxgi1_3.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DDSTextureLoader.h>
#include <WICTextureLoader.h>
#include <SpriteBatch.h>
#include <SpriteFont.h>
#include <Audio.h>
#include <GamePad.h>
#include <Keyboard.h>
#include <Mouse.h>

// Library.Shared
#include "RTTI.h"
#include "GameException.h"
#include "GameClock.h"
#include "GameTime.h"
#include "IServiceProvider.h"
#include "ServiceContainer.h"
#include "RenderTarget.h"
#include "Game.h"
#include "GameComponent.h"
#include "DrawableGameComponent.h"
#include "VectorHelper.h"
#include "MatrixHelper.h"
#include "DirectXHelper.h"
#include "Camera.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "FirstPersonCamera.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "ColorHelper.h"
#include "Utility.h"
#include "VertexDeclarations.h"
#include "BlendStates.h"
#include "RasterizerStates.h"
#include "SamplerStates.h"
#include "RenderStateHelper.h"
#include "FpsComponent.h"
#include "StreamHelper.h"
#include "Model.h"
#include "Mesh.h"
#include "ModelMaterial.h"
#include "ProxyModel.h"
#include "Skybox.h"
#include "MouseComponent.h"
#include "KeyboardComponent.h"
#include "GamePadComponent.h"
#include "Grid.h"
#include "AudioEngineComponent.h"
#include "TextureHelper.h"
#include "GameScreen.h"
#include "ScreenManager.h"
#include "ScreenInputManager.h"
#include "SpriteManager.h"
#include "ContentManager.h"
#include "MathHelper.h"
#include "EventArgs.h"

// Library.Desktop
#include "UtilityWin32.h"

//GameComponents
#include "Ball.h"
#include "PlayerPaddle.h"
#include "Paddle.h"
#include "ComputerPaddle.h"