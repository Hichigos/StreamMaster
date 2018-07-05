#include "streamclientfactory.h"

#include "../clients/obsclient.h"

IStreamClientPtr StreamClientFactory::createStreamClient()
{
	return std::make_shared<OBSClient>();
}
