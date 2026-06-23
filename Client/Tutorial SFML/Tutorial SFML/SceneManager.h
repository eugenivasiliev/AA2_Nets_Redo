#pragma once
#include <unordered_map>
#include "Scene.h"
#include "GameScene.h"
#include "LoginScene.h"
#include "LobbyScene.h"
#include "LobbyWaitingScene.h"
#include "RankingScene.h"
#include <string>
#include "User.h"
#include "config.h"
#include "SharedMemory.h"
#include "TimeManager.h"

class SceneManager
{
	std::unordered_map<SceneOption, Scene*> scenes;
	Scene* curScene;
	SharedMemory* sharedMemory;

public:
	SceneManager() {

		sharedMemory = new SharedMemory();

		scenes[SceneOption::GAME] = new GameScene();
		scenes[SceneOption::LOGIN] = new LoginScene();
		scenes[SceneOption::LOBBY] = new LobbyScene();
		scenes[SceneOption::WAITING_ROOM] = new LobbyWaitingScene();
		scenes[SceneOption::RANKING] = new RankingScene();
		curScene = scenes[SceneOption::LOGIN];
		curScene->enter(sharedMemory);
	}

	bool update(sf::RenderWindow& window) {
		NT->Update();
		TIME->Update();

		if (curScene->nextScene != SceneOption::NONE) {
			curScene->exit();
			curScene = scenes[curScene->nextScene];
			curScene->enter(sharedMemory);
		}

		return curScene->update(window);
	}
};

