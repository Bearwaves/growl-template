#include "game.h"
#include "growl/core/api/api.h"
#include "growl/core/error.h"

using Growl::Error;
using MyTitle::Game;

Error Game::init() {
	getAPI().system().log("Game", "Game starting up!");
	return nullptr;
}

void Game::render() {
	auto batch = getAPI().graphics().createBatch();
	batch->clear(0, 0, 0);
	batch->begin();
	batch->end();
}

void Game::resize(const int width, const int height) {}

Error Game::dispose() {
	getAPI().system().log("Game", "Game shutting down");
	return nullptr;
}
