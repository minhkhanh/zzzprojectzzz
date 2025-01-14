#include "StdAfx.h"
#include "GameSound.h"

int GameSound::m_iSoundOn = 1;

GameSound::GameSound(void)
{
}

GameSound::~GameSound(void)
{
}

void GameSound::Coin()
{
	//mciSendString(L"stop coin", NULL, 0, 0);
	//mciSendString(L"close coin", NULL, 0, 0);

	mciSendString(L"open Resource\\Music\\coin.wav type mpegvideo alias coin", NULL, 0, 0);
	mciSendString(L"play coin from 0 to 100", NULL, 0, 0);	// to 100 để cắt ngắn tiếng ăn tiền
}

void GameSound::Jump()
{
	//mciSendString(L"stop jump", NULL, 0, 0);
	//mciSendString(L"close jump", NULL, 0, 0);

	mciSendString(L"open Resource\\Music\\step.wav type waveaudio alias jump", NULL, 0, 0);
	mciSendString(L"play jump from 0 to 100", NULL, 0, 0);	// to 100 để cắt ngắn tiếng nhảy
}

void GameSound::MainGameTheme(int iFlag)
{
	mciSendString(L"stop typeAtheme", NULL, 0, 0);
	mciSendString(L"close typeAtheme", NULL, 0, 0);

	if (iFlag == 1)
	{
		mciSendString(L"open Resource\\Music\\ROCK.wav type mpegvideo alias typeAtheme", NULL, 0, 0);
		mciSendString(L"play typeAtheme repeat", NULL, 0, 0);
	}
}

void GameSound::MenuTheme( int iFlag )
{
	mciSendString(L"stop menutheme", NULL, 0, 0);
	mciSendString(L"close menutheme", NULL, 0, 0);

	if (iFlag == 1)
	{
		mciSendString(L"open Resource\\Music\\menu.mid type mpegvideo alias menutheme", NULL, 0, 0);
		mciSendString(L"play menutheme from 0 repeat", NULL, 0, 0);
	}
}

void GameSound::IntroTheme( int iFlag )
{
	mciSendString(L"stop pushing", NULL, 0, 0);
	mciSendString(L"close pushing", NULL, 0, 0);

	if (iFlag == 1)
	{
		mciSendString(L"open Resource\\Music\\pushing.mid type mpegvideo alias pushing", NULL, 0, 0);
		mciSendString(L"play pushing from 0 repeat", NULL, 0, 0);
	}
}

void GameSound::EnterMainGame( int iFlag )
{
	mciSendString(L"stop beginlevel", NULL, 0, 0);
	mciSendString(L"close beginlevel", NULL, 0, 0);

	if (iFlag == 1)
	{
		mciSendString(L"open Resource\\Music\\beginlevel.mid type mpegvideo alias beginlevel", NULL, 0, 0);
		mciSendString(L"play beginlevel from 0 wait", NULL, 0, 0);
	}
}

void GameSound::Explosion()
{
	//mciSendString(L"stop explosion", NULL, 0, 0);
	//mciSendString(L"close explosion", NULL, 0, 0);

	mciSendString(L"open Resource\\Music\\explosion.wav type mpegvideo alias explosion", NULL, 0, 0);
	mciSendString(L"play explosion from 0", NULL, 0, 0);
}

void GameSound::Play( int iSoundID , int iFlag )
{
	if (m_iSoundOn == 0)
		return;

	switch (iSoundID)
	{
	case COIN_SOUND:
		GameSound::Coin();
		break;
	case JUMP_SOUND:
		GameSound::Jump();
		break;
	case EXPLOSION_SOUND:
		GameSound::Explosion();
		break;
	case MAIN_GAME_THEME_SOUND:
		GameSound::MainGameTheme(iFlag);
		break;
	case INTRO_THEME_SOUND:
		GameSound::IntroTheme(iFlag);
		break;
	case ENTER_MAIN_GAME_SOUND:
		GameSound::EnterMainGame(iFlag);
		break;
	case MENU_THEME_SOUND:
		GameSound::MenuTheme(iFlag);
		break;
	case GAME_OVER_THEME_SOUND:
		GameSound::GameOverTheme(iFlag);
		break;
	case BONUS_LIVE_SOUND:
		GameSound::BonusLive(iFlag);
		break;
	case GAME_FINISH_THEME_SOUND:
		GameSound::GameFinishTheme(iFlag);
		break;
	case OCEGRINE_DIES_SOUND:
		GameSound::OcegrineDies(iFlag);
		break;
	}
}

void GameSound::TurnOnVolume()
{
	m_iSoundOn = 1;
}

void GameSound::TurnOffVolume()
{
	m_iSoundOn = 0;
}

void GameSound::GameOverTheme( int iFlag )
{
	mciSendString(L"stop gameover", NULL, 0, 0);
	mciSendString(L"close gameover", NULL, 0, 0);

	if (iFlag == 1)
	{
		mciSendString(L"open Resource\\Music\\AULDLANG.MID type mpegvideo alias gameover", NULL, 0, 0);
		mciSendString(L"play gameover from 0 repeat", NULL, 0, 0);
	}
}

void GameSound::BonusLive( int iFlag )
{
	mciSendString(L"open Resource\\Music\\1up.wav type mpegvideo alias 1up", NULL, 0, 0);
	mciSendString(L"play 1up from 0", NULL, 0, 0);
}

void GameSound::GameFinishTheme( int iFlag /*= 1*/ )
{
	mciSendString(L"stop gamefinish", NULL, 0, 0);
	mciSendString(L"close gamefinish", NULL, 0, 0);

	if (iFlag == 1)
	{
		mciSendString(L"open Resource\\Music\\bmintro.mid type mpegvideo alias gamefinish", NULL, 0, 0);
		mciSendString(L"play gamefinish from 0 repeat", NULL, 0, 0);
	}
}

void GameSound::OcegrineDies( int iFlag )
{
	mciSendString(L"open Resource\\Music\\die.wav type mpegvideo alias die", NULL, 0, 0);
	mciSendString(L"play die from 0 wait", NULL, 0, 0);
}