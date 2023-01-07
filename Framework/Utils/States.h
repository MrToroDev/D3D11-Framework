#pragma once

namespace DX
{
	class States {
	public:
		virtual void init() = 0;
		virtual void update(float dt) {};
		virtual void draw() = 0;
		virtual void destroy() = 0;

		virtual void Pause() {}
		virtual void Resume() {}
	};
}