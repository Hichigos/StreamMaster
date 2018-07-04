#include "servicefactory.h"

#include "../adapters/services/youtubeservice.h"
#include "../adapters/services/twitchservice.h"


ServicePtr ServiceFactory::createService(const ServiceType type)
{
	if (type == ServiceType::YouTube)
		return std::make_shared<YouTubeService>();
	else 
		return std::make_shared<TwitchService>();
}
