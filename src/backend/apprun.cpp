#include "app\smapp.h"

int main() {
	utils::log_string("________new instance_________\n\n");

	SMApp app;
	app.Run();

	utils::log_string("________end of instance______\n\n");

	return 0;
}