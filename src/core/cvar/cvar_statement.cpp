#include "sani/core/cvar/cvar_statement.hpp"

namespace sani {

	CVarRequireStatement::CVarRequireStatement(const std::vector<CVarCondition>& conditions) : conditions(conditions) {
	}

	CVarRequireStatement::~CVarRequireStatement() {
	}

	bool CVarRequireStatement::operator()() const {
		/*
			TODO: idk, looks kinda funkeyyYY, maybe refactor?
				  could fix with a transition table.
		*/

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

				// Pre inc to get first at next iteration.
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
					if (i == 0)				  i++;
					if (i < conditions.size()) current = &conditions[i];
				}

				// Left side was true. Swap.
				if (!orResult && beforeOr) result = beforeOr;

				i++;
			} else {
				throw std::logic_error("Invalid or unsupported cvarlang::Operator");
			}
		}

		return result;
	}
}