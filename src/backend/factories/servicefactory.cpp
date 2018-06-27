#include "servicefactory.h"

#include "../adapters/obs/services/youtubeservice.h"
#include "../adapters/obs/services/twitchservice.h"

ServicePtr ServiceFactory::createYouTubeService()
{
	return std::make_shared<YouTubeService>();
}

ServicePtr ServiceFactory::createTwitchService()
{
	return std::make_shared<TwitchService>();
}
