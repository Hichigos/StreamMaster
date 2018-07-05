#include "../clients/interfaces/istreamclient.h"

class StreamClientFactory {

public:
	static IStreamClientPtr createStreamClient();
};