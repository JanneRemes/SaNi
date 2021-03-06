#include "sani/core/cvar/cvar_statement.hpp"
#include "sani/core/logging/log.hpp"

namespace sani {

	CVarRequireStatement::CVarRequireStatement(const std::vector<CVarCondition>& conditions, const String& message) : conditions(conditions),
																													  message(message) {
	}

	const String& CVarRequireStatement::getMessage() const {
		return message;
	}

	CVarRequireStatement::~CVarRequireStatement() {
	}

	bool CVarRequireStatement::operator()() const {
		// No conditions, can always write.
		if (conditions.size() == 0) return true;

		// Interpret.
		bool lastResult = false;
		bool result = false;
		size_t i = 0;

		while (i < conditions.size()) {
			const CVarCondition* current = &conditions[i];
			const CVarCondition* second = nullptr;

			if (conditions[i].getOperator() == sani::cvarlang::LogicalOperators::None) {
				result = (*current)();

				i++;
			} else if (conditions[i].getOperator() == sani::cvarlang::LogicalOperators::And) {
				// Post inc to get next.
				i++;

				lastResult = result;

				second = &conditions[i];
				result = (*current)() && (*second)();

				// Pre-inc to get first at next iteration.
				i++;
			} else if (conditions[i].getOperator() == sani::cvarlang::LogicalOperators::Or) {
				bool orResult = false;
				bool beforeOr = result;

				// || a || b || c 
				while (i < conditions.size() || current->getOperator() == cvarlang::LogicalOperators::Or) {
					// Get cur + 1 element.
					if (i > 0) lastResult = result;
					else	   { i++; lastResult = conditions[i](); }

					// True found, return.
					if (orResult) break;

					// Compute results.
					orResult = (*current)() || lastResult;
					result = orResult;

					// Get next. cur + 1.
					if (i == 0)				   i++;
					if (i < conditions.size()) current = &conditions[i];
				}

				// Left side was true. Swap.
				if (!orResult && beforeOr) result = beforeOr;

				i++;
			} else {
				FNCLOG_ERR(log::OutFlags::All, "invalid or unsupported cvarlang::Operator");

				std::abort();
			}
		}

		return result;
	}
	
	bool CVarRequireStatement::operator == (const CVarRequireStatement& other) const {
		if (conditions.size() != other.conditions.size()) return false;

		for (size_t i = 0; i < conditions.size(); i++) {
			const CVarCondition& lhs = conditions[i];
			const CVarCondition& rhs = other.conditions[i];

			if (lhs != rhs) return false;
		}

		return true;
	}
	bool CVarRequireStatement::operator != (const CVarRequireStatement& other) const {
		return !(*this == other);
	}
}