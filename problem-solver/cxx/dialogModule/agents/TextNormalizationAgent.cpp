
#include <sc-memory/cpp/sc_memory.hpp>
#include <sc-memory/cpp/sc_stream.hpp>
#include <sc-memory/cpp/sc_link.hpp>
#include <sc-kpm/sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-kpm/sc-agents-common/utils/AgentUtils.hpp>

#include "TextNormalizationAgent.hpp"
#include "keynodes/keynodes.hpp"

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace utils;

namespace dialogModule {

  SC_AGENT_IMPLEMENTATION(TextNormalizationAgent) {
    if (!edgeAddr.IsValid())
      return SC_RESULT_ERROR;

    ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);

    //get message node
    ScAddr msg = IteratorUtils::getFirstFromSet(ms_context.get(), questionNode);
    if (!msg.IsValid())
      return SC_RESULT_ERROR_INVALID_PARAMS;

    //build answer structure
    ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, msg);
    //add "message -> msg" to answer structure
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::message);
    ScIterator3Ptr messageToMsg = ms_context->Iterator3(Keynodes::message, ScType::EdgeAccessConstPosPerm, msg);
    while (messageToMsg->Next()) {
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, messageToMsg->Get(1));
      break;
    }

    //get message sc-text translation node
    ScAddr transl;
    ScIterator5Ptr msgToTransl = ms_context->Iterator5(ScType::Unknown, ScType::EdgeDCommonConst, msg,
                                                       ScType::EdgeAccessConstPosPerm,
                                                       Keynodes::nrel_sc_text_translation);
    while (msgToTransl->Next()) {
      transl = msgToTransl->Get(0);
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, transl);
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, msgToTransl->Get(1));
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, msgToTransl->Get(3));
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::nrel_sc_text_translation);
      break;
    }
    //get message text
    ScAddr msgTextNode;
    ScIterator3Ptr translToText = ms_context->Iterator3(transl, ScType::EdgeAccessConstPosPerm, ScType::Node);
    while (translToText->Next()) {
      msgTextNode = translToText->Get(2);
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, translToText->Get(1));
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, msgTextNode);
      break;
    }
    ScLink msgTextLink(*ms_context, msgTextNode);
    std::string msgText = msgTextLink.Get<std::string>();
    std::cerr<<"MESSAGE_TEXT: "<<msgText<<"\n";     //DEBUG INFO

    //get nnwf's
    std::vector<ScAddr> nnwfs;
    ScIterator3Ptr nnwfToNodes = ms_context->Iterator3(Keynodes::not_normalized_word_form,
                                                       ScType::EdgeAccessConstPosPerm, ScType::Node);
    while (messageToMsg->Next()) {
      nnwfs.push_back(messageToMsg->Get(2));
    }

    //find nnwfs in text and replace them with normalized forms
    for (ScAddr idtf : nnwfs) {

      //get nnwf standart text
      ScAddr standartNode = IteratorUtils::getFirstByOutRelation(ms_context.get(), idtf,
                                                                 Keynodes::rrel_standart);
      ScLink standartLink(*ms_context, standartNode);
      std::string const standartText = standartLink.Get<std::string>();
      std::cerr<<"STANDART_TEXT:\t\t"<<standartText<<"\n";     //DEBUG INFO

      //find a nnwf in text and replace it with normalized form
      size_t pos = findIC(msgText, standartText);
      while (pos != std::string::npos) {
        //get normalized form text
        ScAddr normalizedNode = IteratorUtils::getFirstByOutRelation(ms_context.get(),
                                                                     idtf, Keynodes::rrel_normalization);
        ScLink normalizedLink(*ms_context, normalizedNode);
        std::string const normalizedText = normalizedLink.Get<std::string>();
        std::cerr<<"NORMALIZED_TEXT:\t\t\t\t"<<normalizedText<<"\n";     //DEBUG INFO

        //replace this nnwf text in the message text with normalized form text
        msgText.replace(pos, standartText.length(), lowerOf(normalizedText));

        //find next occurrence of this nnwf in the message text
        pos = findIC(msgText, standartText);
      }
    }

    //update message link
    msgTextLink.Set(msgText);
    std::cerr<<"NEW_MESSAGE_TEXT: "<<msgText<<"\n";     //DEBUG INFO

    //create edge [set_messages_requiring_parsing -> message]
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, Keynodes::set_messages_requiring_parsing, msg);
    ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, Keynodes::set_messages_requiring_parsing);
    ScIterator3Ptr smrpToMsg = ms_context->Iterator3(Keynodes::set_messages_requiring_parsing,
                                                     ScType::EdgeAccessConstPosPerm, msg);
    while (smrpToMsg->Next()) {
      ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, smrpToMsg->Get(1));
      break;
    }

    //return
    AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
    return SC_RESULT_OK;
  }

  std::string TextNormalizationAgent::lowerOf(std::string data) {
    std::string result;
    std::transform(data.begin(), data.end(), back_inserter(result), ::tolower);
    return result;
  }

  size_t TextNormalizationAgent::findIC(const std::string &data, const std::string &sub) {
    std::string tempData;
    std::transform(data.begin(), data.end(), back_inserter(tempData), ::tolower);
    std::string tempSub;
    std::transform(sub.begin(), sub.end(), back_inserter(tempSub), ::tolower);
    return tempData.find(tempSub);
  }
}
