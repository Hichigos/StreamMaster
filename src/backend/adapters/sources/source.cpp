#include "source.h"

Source::Source() {}
Source::~Source() { obs_source_release(source); }

Source::operator obs_source*() const { return source; }

void Source::UpdateSettings(const Settings &settings) {
	this->settings = settings;
	obs_source_update(source, this->settings);
}
