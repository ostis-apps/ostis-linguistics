/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include <sc-memory/cpp/sc_stream.hpp>
#include <sc-kpm/sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-kpm/sc-agents-common/utils/AgentUtils.hpp>


#include <sc-memory/cpp/sc_link.hpp>
#include <algorithm>

#include "SubdividingSearchAgent.hpp"
#include "keynodes/keynodes.hpp"

using namespace std;
using namespace utils;

namespace exampleModule
{

SC_AGENT_IMPLEMENTATION(SubdividingSearchAgent)
{
  if (!edgeAddr.IsValid())
    return SC_RESULT_ERROR;

  ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
  ScAddr param = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
  if (!param.IsValid())
    return SC_RESULT_ERROR_INVALID_PARAMS;

  ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);

  ScAddr nodeText = IteratorUtils::getFirstByInRelation(ms_context.get(), param, Keynodes::nrel_sc_text_translation);
  ScAddr textOfMessageNode;

  ScIterator3Ptr iterator3 = ms_context->Iterator3(nodeText, ScType::EdgeAccessConstPosPerm, ScType::Link);
    while (iterator3->Next())
    {
        textOfMessageNode = iterator3->Get(2);
        break;
    }
  ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, textOfMessageNode);
  std::vector <std::string> words;
  decompositionIntoWords(textOfMessageNode, words);

  vector < vector <ScAddr> > potentialLexemeForEachWord;

  for (size_t i=0;i<words.size();i++) {
      potentialLexemeForEachWord.emplace_back();

      ScAddrList listOfLinkAddrs;
      getLinksByString(words[i], listOfLinkAddrs);

      for (auto &addr: listOfLinkAddrs) {
         ScTemplate templ;
         templ.Triple(
                 ScType::NodeVar >> "_empty_node_standart",
                 ScType::EdgeAccessVarPosPerm >> "_edge_rrel_standart",
                 addr);
         templ.Triple(
                 Keynodes::rrel_standard,
                 ScType::EdgeAccessVarPosPerm,
                 "_edge_rrel_standart");
         templ.Triple(
                 ScType::NodeVar >> "_sheaf_node_standart",
                 ScType::EdgeAccessVarPosPerm,
                 "_empty_node_standart");
         templ.Triple(
                 "_sheaf_node_standart",
                 ScType::EdgeDCommonVar >> "_common_edge_lexeme",
                 ScType::NodeVar >> "_detected_lexeme");
         templ.Triple(
                 Keynodes::nrel_paradigm,
                 ScType::EdgeAccessVarPosPerm,
                 "_common_edge_lexeme");
         ScTemplateSearchResult resultList;
         ms_context->HelperSearchTemplate(templ, resultList);

         if (resultList.Size() == 0) continue;

         ScTemplateSearchResultItem searchResultItem = resultList[0];
         ScAddr defLexemeAddr = searchResultItem["_detected_lexeme"];
         potentialLexemeForEachWord[i].push_back(defLexemeAddr);
      }
  }

  size_t quantity=1;
  size_t k =0;
  for(size_t i = 0; i < potentialLexemeForEachWord.size(); i++){
     if(potentialLexemeForEachWord[i].empty())
         potentialLexemeForEachWord[i].push_back(Keynodes::undefined_word);
     else  quantity*=potentialLexemeForEachWord[i].size();
  }
  vector < vector <ScAddr> > combinationsOfLexeme(quantity, vector <ScAddr>(words.size()));
  getCombinationsOfLexeme(combinationsOfLexeme, -1, potentialLexemeForEachWord, k);


 for(auto & numberOfCombination : combinationsOfLexeme) {
     ScAddr edgeText;
     ScAddr sheaf;
     ScTemplate templ;
     ScTemplateGenResult resultGen;
         templ.Triple(
                 ScType::NodeVar>>"_sheaf_decomposition",
                 ScType::EdgeDCommonVar >> "_common_edge_decomposition",
                 textOfMessageNode);
         templ.Triple(
                 Keynodes::nrel_text_decomposition,
                 ScType::EdgeAccessVarPosPerm >> "_edge_nrel_decomposition",
                 "_common_edge_decomposition");
         ms_context->HelperGenTemplate(templ, resultGen);
         sheaf = resultGen["_sheaf_decomposition"];

         ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, resultGen["_sheaf_decomposition"]);
         ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, resultGen["_common_edge_decomposition"]);
         ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, resultGen["_edge_nrel_decomposition"]);
         ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_text_decomposition);

         for (size_t wordNumber = 0; wordNumber < numberOfCombination.size(); wordNumber++) {
             templ.Clear();
             ScAddr newLink;
             createLinkByContent(words[wordNumber],newLink);
             templ.Triple(
                 sheaf,
                 ScType::EdgeAccessVarPosPerm >> "_edge_text",
                 newLink);
             templ.Triple(
                 numberOfCombination[wordNumber],
                 ScType::EdgeAccessVarPosPerm >> "_edge_lexeme",
                 newLink);

             if(wordNumber == 0)
                 templ.Triple(
                     Keynodes::rrel_1,
                     ScType::EdgeAccessVarPosPerm,
                     "_edge_text");
             else{
                 templ.Triple(
                     edgeText,
                     ScType::ScType::EdgeDCommonVar >> "_sequence_edge",
                     "_edge_text");
                 templ.Triple(
                     Keynodes::nrel_sequence,
                     ScType::EdgeAccessVarPosPerm,
                     "_sequence_edge");
             }
             ms_context->HelperGenTemplate(templ, resultGen);
             edgeText = resultGen["_edge_text"];

             ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, resultGen["_edge_text"]);
             ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, newLink);
             ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, resultGen["_edge_lexeme"]);
             ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, numberOfCombination[wordNumber]);
            /* iterator3 = ms_context->Iterator3(ScType::Unknown, ScType::EdgeAccessConstPosPerm, numberOfCombination[wordNumber]);
             while (iterator3->Next())
             {
                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, iterator3->Get(0));
                 ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, iterator3->Get(1));
             }
             */
     }
 }
  AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
  return SC_RESULT_OK;
}

    void SubdividingSearchAgent:: getCombinationsOfLexeme(vector<vector<ScAddr>>& combinationsOfLexeme, size_t i,
            const std::vector<vector<ScAddr>>& variantsOfLexemeForEachWord, size_t& k)
    {
        i++;
        for (size_t a = 0; a < variantsOfLexemeForEachWord[i].size(); a++)
        {
            if (a>0) k++;
            if (a != 0)
                for (size_t  b = 0; b<i; b++)
                {
                    combinationsOfLexeme[k][b] = combinationsOfLexeme[k - 1][b];
                }
            combinationsOfLexeme[k][i] = variantsOfLexemeForEachWord[i][a];
            if(i < variantsOfLexemeForEachWord.size() - 1)
                getCombinationsOfLexeme(combinationsOfLexeme, i, variantsOfLexemeForEachWord, k);

        }
    }
    void SubdividingSearchAgent:: decompositionIntoWords(ScAddr & linkWithText, std::vector <std::string> & words)
    {
        ScStream stream ;
        ms_context->GetLinkContent(linkWithText,stream);

        auto *text = new sc_char[stream.Size()];
        size_t size = stream.Size();
        stream.Read(text,size,size);
        for (size_t i=0;i<size;i++)
            text[i]=toupper(text[i]);

        string strText = text;
        std::string const delims{ " .,:;!?" };

        size_t beg, pos = 0;
        while ((beg = strText.find_first_not_of(delims, pos)) != std::string::npos)
        {
            pos = strText.find_first_of(delims, beg + 1);
            words.push_back(strText.substr(beg, pos - beg));

        }
        delete[] text;
    }

    void SubdividingSearchAgent:: getLinksByString(std::string const & str, ScAddrList & list)
    {
        size_t size = str.size();
        auto *wordChar = new sc_char[size];
        strcpy(wordChar,str.c_str());
        ScStream stream(wordChar,size,SC_STREAM_FLAG_READ);
        ms_context->FindLinksByContent(stream, list);
    }


    void SubdividingSearchAgent:: createLinkByContent(std::string const & str, ScAddr & newLink)
    {
    size_t size = str.size();
    auto *wordChar = new sc_char[size];
    strcpy(wordChar, str.c_str());
    ScStream streamLink(wordChar, size, SC_STREAM_FLAG_READ);
    newLink = ms_context->CreateLink();
    ms_context->SetLinkContent(newLink, streamLink);
    }
}
