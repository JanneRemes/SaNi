#pragma once

#include "sani/engine/messaging/messages/message.hpp"

namespace sani {
	
	namespace engine {
		
		namespace messages {

			/// @class PeerToPeerMessage peer_to_peer_message.hpp "sani/engine/messaging/messages/peer_to_peer_message.hpp"
			/// @author voidbab
			///
			/// Represents a simple one-way p2p message.
			class PeerToPeerMessage : public Message {
			public:
				PeerToPeerMessage();
			};
		}
	}
}