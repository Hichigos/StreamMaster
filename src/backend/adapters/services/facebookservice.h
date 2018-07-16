#pragma once

#include "service.h"
#include "../encoders/encoder.h"


class FacebookService : public Service {
public:
	FacebookService();

protected:
	void initializeDefaults();
};