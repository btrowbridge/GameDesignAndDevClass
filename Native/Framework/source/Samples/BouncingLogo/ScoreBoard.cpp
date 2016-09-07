#include "pch.h"
#include "ScoreBoard.h"
#include "BouncingLogoGame.h"
using namespace std;
using namespace Library;
using namespace DirectX;

namespace BouncingLogo {

	const XMFLOAT2 ScoreBoard::mMargin(0, 50);
	const int mScoreToWin = 10;

	ScoreBoard::ScoreBoard(Library::Game & game) : DrawableGameComponent(game), mPlayerScore(0), mComputerScore(0)
	{
	}

	void ScoreBoard::Initialize()
	{

		mSpriteBatch = make_unique<SpriteBatch>(mGame->Direct3DDeviceContext());
		mSpriteFont = make_unique<SpriteFont>(mGame->Direct3DDevice(), L"Content\\Fonts\\Arial_14_Regular.spritefont");


		auto& mViewport = mGame->Viewport();


		mPlayerScorePosition = XMFLOAT2(mViewport.Width * .25, mMargin.y);
		mComputerScorePosition = XMFLOAT2(mViewport.Width * .75 ,mMargin.y);
	}

	void ScoreBoard::Update(const Library::GameTime & gameTime)
	{
	}

	void ScoreBoard::Draw(const Library::GameTime & gameTime)
	{
		auto& mViewport = mGame->Viewport();

		mSpriteBatch->Begin();

		wostringstream playerScoreLabel;
		playerScoreLabel << setprecision(10) << left << mPlayerScore;
		
		mSpriteFont->DrawString(mSpriteBatch.get(), playerScoreLabel.str().c_str(), mPlayerScorePosition);

		wostringstream computerScoreLabel;
		 
		computerScoreLabel << setprecision(10) << left << mComputerScore;
		mSpriteFont->DrawString(mSpriteBatch.get(), computerScoreLabel.str().c_str(), mComputerScorePosition);
		
		mSpriteBatch->End();

	}
	void ScoreBoard::PlayerScores()
	{
		mPlayerScore++;
	}
	void ScoreBoard::ComputerScores()
	{
		mComputerScore++;
	}
	void ScoreBoard::ResetScores()
	{
		mPlayerScore = 0;
		mComputerScore = 0;
	}
}