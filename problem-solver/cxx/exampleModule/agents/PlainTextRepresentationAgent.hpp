/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/cpp/kpm/sc_agent.hpp>

#include "keynodes/keynodes.hpp"
#include "PlainTextRepresentationAgent.generated.hpp"

namespace exampleModule
{

class PlainTextRepresentationAgent : public ScAgent
{
  SC_CLASS(Agent, Event(Keynodes::question_plain_text_representation, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
    void createLinkByContent(const string & str, ScAddr &newLink);
    string getLinkContent(const ScAddr & link);
    string mapMainIdtf(const ScAddr &mainEntity, const ScAddr &language);
    string mapIdtf(const ScAddr &mainEntity, const ScAddr &language);
    string mapStatement(const ScAddr &mainEntity, const ScAddr &language);
    string mapDefinition(const ScAddr &mainEntity, const ScAddr &language);
    string mapClassification(const ScAddr &mainEntity, const ScAddr &language);
    string mapExample(const ScAddr &mainEntity, const ScAddr &language);
    string getMainIdtf(const ScAddr &entity, const ScAddr &language);
    string getSystemIdtf(const ScAddr &entity);
    ScTemplateSearchResult templateSearchForDefOrStat(const ScAddr &mainEntity, const ScAddr &language, const ScAddr &type, const string templName);
    ScTemplateSearchResult templateSearchForIdtf(const ScAddr &mainEntity, const ScAddr &language, const ScAddr &type, const string templName);

};

} // namespace exampleModule
