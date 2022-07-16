#include <Engine.h>

class Sandbox : public Engine::Application {
public:
	Sandbox() {

	}
	~Sandbox() {

	}
};

int main() {
	Sandbox* sandbox = new Sandbox();
	sandbox->Print();
	delete sandbox;
}