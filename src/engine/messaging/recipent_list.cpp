#include "sani/engine/messaging/recipent_list.hpp"
#include "sani/debug.hpp"

#include <algorithm>

namespace sani {

	namespace engine {

		RecipentList::RecipentList() {
		}

		void RecipentList::clear() {
			recipents.clear();
		}
		void RecipentList::addRecipent(const String& recipent) {
			recipents.push_back(recipent);
		}
		void RecipentList::removeRecipent(const String& recipent) {
			auto it = std::find_if(recipents.begin(), recipents.end(), [&recipent](const String& lhs) {
				return lhs == recipent;
			});

			if (it == recipents.end()) return;
			
			recipents.erase(it);
		}

		std::vector<const String>::const_iterator RecipentList::begin() const {
			return recipents.begin();
		}
		std::vector<const String>::const_iterator RecipentList::end() const {
			return recipents.end();
		}

		RecipentList::~RecipentList() {
		}
	}
}