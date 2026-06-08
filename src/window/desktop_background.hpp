#pragma once

#include "../image.hpp"
#include "../renderable.hpp"

class DesktopBackground : public Renderable {
private:
	Image image;
public:
	void render() override;

	DesktopBackground();
};
