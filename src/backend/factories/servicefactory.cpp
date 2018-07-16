#include "servicefactory.h"

#include "../adapters/services/youtubeservice.h"
#include "../adapters/services/twitchservice.h"
#include "../adapters/services/facebookservice.h"


ServicePtr ServiceFactory::createService(const ServiceType type)
{
	switch (type) {
	case YouTube:
		return std::make_shared<YouTubeService>();
	case Twitch:
		return std::make_shared<TwitchService>();
	case Facebook:
		return std::make_shared<FacebookService>();

	default:
		return std::make_shared<YouTubeService>();
	}
}
