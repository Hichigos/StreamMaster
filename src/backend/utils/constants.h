/*
	General set of constants, consisting of namespaces:
	Protocol - set of requests and responses for incoming connections
	Default - set of default application settings
*/

namespace Protocol {

	namespace Replays {

		namespace Network {

			const char* const OK = "ok";
			const char* const BadOperation = "bo";
			const char* const Error = "er";
			const char* const NotAvailable = "na";

		} // end of Protocol::Replays::Network

		namespace State {

			const char* const Started = "strd";
			const char* const Stopped = "stpd";
			const char* const Busy = "busy";

		} // end of Protocol::Replays::State

	} // end of Protocol::Replays

	namespace Request {

		const char* const State = "stat";
		const char* const Start = "stst";
		const char* const Stop = "spst";
		const char* const UpdateToken = "updt";
		const char* const UpdateService = "upds";
		const char* const Ping = "ping";

	} // end of Protocol::Request

} // end of Protocol


namespace Default {

	namespace Network {

		const char* const Address = "127.0.0.1";
		const char* const Port = "48684";
		
		const int MaxConnections = 1;

	} // end of Default::Network

} // end of Default