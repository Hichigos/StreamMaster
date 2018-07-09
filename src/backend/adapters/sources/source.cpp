#include "source.h"

Source::Source() 
{}

Source::~Source() {
	obs_source_release(m_source);
}

Source::operator obs_source*() const {
	return m_source;
}

void Source::updateSettings(const Settings &settings) {
	m_settings = settings;
	obs_source_update(m_source, m_settings);
}

void Source::updateScaleResolution(int width, int height)
{
	m_settings.SetString("scale_res", "1600õ900");//std::to_string(width) + "x" + std::to_string(height));
	obs_source_update(m_source, m_settings);
}
