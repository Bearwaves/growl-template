#pragma once

#include "growl/core/game/game.h"

namespace MyTitle {

class Game : public Growl::Game {
public:
	Growl::Error init() override;
	Growl::Error dispose() override;
	void render() override;
	void resize(const int width, const int height) override;
};

} // namespace MyTitle
