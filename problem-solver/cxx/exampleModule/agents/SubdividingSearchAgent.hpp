/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include <sc-memory/cpp/kpm/sc_agent.hpp>

#include "keynodes/keynodes.hpp"
#include "SubdividingSearchAgent.generated.hpp"

using namespace std;

namespace exampleModule
{

class SubdividingSearchAgent : public ScAgent
{
  SC_CLASS(Agent, Event(Keynodes::question_find_subdividing, ScEvent::Type::AddOutputEdge))
  SC_GENERATED_BODY()

private:
    void decompositionIntoWords(ScAddr & linkWithText, std::vector <std::string> & words);
    void  getLinksByString(std::string const & str, ScAddrList & list);

   void getCombinationsOfLexeme(vector<vector<ScAddr>>& combinationsOfLexeme, size_t  i, const std::vector<vector<ScAddr>>& variantsOfLexemeForEachWord, size_t& k);

    void createLinkByContent(const string &str, ScAddr &newLink);
};


} // namespace exampleModule
