#pragma once

#include <sc-memory/cpp/kpm/sc_agent.hpp>

#include "keynodes/keynodes.hpp"
#include "TextNormalizationAgent.generated.hpp"

namespace dialogModule
{

class TextNormalizationAgent : public ScAgent
{
  SC_CLASS(Agent, Event(Keynodes::question_text_normalization, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
  std::string lowerOf(std::string data);
  size_t findIC(const std::string& data, const std::string& sub);
};

} // namespace dialogModule
