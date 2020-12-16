/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include <sc-memory/cpp/sc_stream.hpp>
#include <sc-kpm/sc-agents-common/utils/IteratorUtils.hpp>
#include <sc-kpm/sc-agents-common/utils/AgentUtils.hpp>

#include <sc-memory/cpp/sc_link.hpp>
#include <map>
#include <iostream>

#include "PlainTextRepresentationAgent.hpp"
#include "keynodes/keynodes.hpp"

using namespace std;
using namespace utils;

typedef string (exampleModule::PlainTextRepresentationAgent::*Mapper)(const ScAddr &, const ScAddr&);

namespace exampleModule
{
    SC_AGENT_IMPLEMENTATION(PlainTextRepresentationAgent)
    {
        if (!edgeAddr.IsValid())
            return SC_RESULT_ERROR;
        ScAddr questionNode = ms_context->GetEdgeTarget(edgeAddr);
        ScAddr param = IteratorUtils::getFirstByOutRelation(ms_context.get(), questionNode, Keynodes::rrel_1);
        ScAddr language = IteratorUtils::getFirstByOutRelation(ms_context.get(), questionNode, Keynodes::rrel_2);
        if (!param.IsValid())
            return SC_RESULT_ERROR_INVALID_PARAMS;

        if (!language.IsValid())
            language = Keynodes::lang_en;

        ScAddr answer = ms_context->CreateNode(ScType::NodeConstStruct);

        Mapper mappers[6] = {
            &exampleModule::PlainTextRepresentationAgent::mapMainIdtf,
            &exampleModule::PlainTextRepresentationAgent::mapIdtf,
            &exampleModule::PlainTextRepresentationAgent::mapDefinition,
            &exampleModule::PlainTextRepresentationAgent::mapStatement,
            &exampleModule::PlainTextRepresentationAgent::mapClassification,
            &exampleModule::PlainTextRepresentationAgent::mapExample
        };

        string result = "";
        for (int i = 0; i < 6; i++)
        {
            string mappingRes = (this->*mappers[i])(param, language);
            if (mappingRes != "")
                result += mappingRes;
        }
        ScAddr newLink;
        createLinkByContent(result, newLink);

        ms_context->CreateEdge(ScType::EdgeAccessConstPosPerm, answer, newLink);
        AgentUtils::finishAgentWork(ms_context.get(), questionNode, answer);
        return SC_RESULT_OK;
    }

    void PlainTextRepresentationAgent::createLinkByContent(const string &str, ScAddr &newLink)
    {
        size_t size = str.size();
        auto *wordChar = new sc_char[size];
        strcpy(wordChar, str.c_str());
        ScStream streamLink(wordChar, size, SC_STREAM_FLAG_READ);
        newLink = ms_context->CreateLink();
        ms_context->SetLinkContent(newLink, streamLink);
    }

    ScTemplateSearchResult PlainTextRepresentationAgent::templateSearchForDefOrStat(const ScAddr &mainEntity,
            const ScAddr &language, const ScAddr &type, const string templName)
    {
        ScTemplate templ;

        templ.Triple(
                type,
                ScType::EdgeAccessVarPosPerm,
                ScType::NodeVar >> "_node");
        templ.Triple(
                "_node",
                ScType::EdgeAccessVarPosPerm >> "_edge_pos",
                mainEntity);
        templ.Triple(
                Keynodes::rrel_key_sc_element,
                ScType::EdgeAccessVarPosPerm,
                "_edge_pos");
        templ.Triple(
                ScType::NodeVar >> "_text_translation_node",
                ScType::EdgeDCommonVar >> "_edge_translation",
                "_node");
        templ.Triple(
                Keynodes::nrel_sc_text_translation,
                ScType::EdgeAccessVarPosPerm,
                "_edge_translation");
        templ.Triple(
                "_text_translation_node",
                ScType::EdgeAccessVarPosPerm,
                ScType::Link >> templName);
        templ.Triple(
                language,
                ScType::EdgeAccessVarPosPerm,
                templName);

        ScTemplateSearchResult results;
        ms_context->HelperSearchTemplate(templ, results);
        return results;
    }

    ScTemplateSearchResult PlainTextRepresentationAgent::templateSearchForIdtf(const ScAddr &mainEntity,
                                                                                    const ScAddr &language, const ScAddr &type, const string templName)
    {
        ScTemplate templ;
        templ.Triple(
                mainEntity,
                ScType::EdgeDCommonVar >> "_edge_idtf",
                ScType::Link >> templName);
        templ.Triple(
                type,
                ScType::EdgeAccessVarPosPerm,
                "_edge_idtf");
        templ.Triple(
                language,
                ScType::EdgeAccessVarPosPerm,
                templName);

        ScTemplateSearchResult results;
        ms_context->HelperSearchTemplate(templ, results);
        return results;
    }

    string PlainTextRepresentationAgent::mapMainIdtf(const ScAddr &mainEntity, const ScAddr &language)
    {
        string mainIdtf = getMainIdtf(mainEntity, language);
        string systIdtfOfClass = getSystemIdtf(mainEntity);

        return "<b><sc_element sys_idtf  = \"" +  systIdtfOfClass + "\">" + mainIdtf + "</sc_element></b> ";
    }

    string PlainTextRepresentationAgent::mapIdtf(const ScAddr &mainEntity, const ScAddr &language)
    {
        string templName = "_link";
        ScTemplateSearchResult results = templateSearchForIdtf(mainEntity, language, Keynodes::nrel_idtf, templName);

        if (results.Size() == 0)
            return ""; //no idtf

        string mappingResult = "";
        if (language == Keynodes::lang_en)
            mappingResult += "<p>Alternative names: ";
        else
            mappingResult += "<p>Альтернативные названия: ";

        for (size_t a = 0; a < results.Size(); a++)
        {
            mappingResult += getLinkContent(results[a][templName]);
            if(a != results.Size() - 1)
                mappingResult += ", ";
            else
                mappingResult += "</p>";
        }
        return mappingResult;
    }

    string PlainTextRepresentationAgent::mapStatement(const ScAddr &mainEntity, const ScAddr &language)
    {
        string templName = "_link_statement";
        ScTemplateSearchResult results = templateSearchForDefOrStat(mainEntity, language, Keynodes::statement, templName);

        if (results.Size() == 0)
            return ""; //no statements

        string mappingResult = "";
        for (size_t a = 0; a < results.Size(); a++)
        {
            string statement = getLinkContent(results[a][templName]);
            mappingResult += "<p>" + statement + "</p>";
        }
        return mappingResult;
    }

    string PlainTextRepresentationAgent::mapDefinition(const ScAddr &mainEntity, const ScAddr &language)
    {
        string templName = "_link_definition";
        ScTemplateSearchResult results = templateSearchForDefOrStat(mainEntity, language, Keynodes::definition, templName);

        if (results.Size() == 0)
            return ""; //no definitions

        string mappingResult = "";
        if (language == Keynodes::lang_en)
            mappingResult += "<p>Definition</p> ";
        else
            mappingResult += "<p>Определение</p> ";

        for (size_t a = 0; a < results.Size(); a++)
        {
            string definition = getLinkContent(results[a][templName]);
            mappingResult += "<p><span style=\"border: 4px double black\">" + definition + "</span></p>";
        }
        return mappingResult;
    }

    string PlainTextRepresentationAgent::mapClassification(const ScAddr &mainEntity, const ScAddr &language)
    {
        map<string, vector<ScAddr>> classificationGroups;

        ScTemplate templ;
        templ.Triple(
                ScType::NodeVar >> "_tuple",
                ScType::EdgeDCommonVar >> "_edge_subdividing",
                mainEntity);
        templ.Triple(
                Keynodes::nrel_subdividing,
                ScType::EdgeAccessVarPosPerm,
                "_edge_subdividing");
        templ.Triple(
                "_tuple",
                ScType::EdgeAccessVarPosPerm,
                ScType::NodeVarClass >> "_class");

        ScTemplateSearchResult results;
        ms_context->HelperSearchTemplate(templ, results);

        if (results.Size() == 0)
            return ""; //no subdividing

        for (size_t a = 0; a < results.Size(); a++)
        {
            string mainIdtfOfGroup = getMainIdtf(results[a]["_tuple"], language);
            classificationGroups[mainIdtfOfGroup].push_back(results[a]["_class"]);
        }

        string mappingResult = "";
        if (language == Keynodes::lang_en)
            mappingResult += "<p><b>Classification: </b></p>";
        else
            mappingResult += "<p><b>Классификация: </b></p>";

        for (map<string, vector<ScAddr>>::iterator it = classificationGroups.begin(); it != classificationGroups.end(); it++)
        {
            if (!(it->first == ""))
                mappingResult += "<p>" + it->first + ": </p>";
            for (size_t i = 0; i < (it->second).size(); i++)
            {
                string systIdtfOfClass = getSystemIdtf(it->second[i]);
                string mainIdtf = getMainIdtf(it->second[i], language);

                mappingResult += "<li><sc_element sys_idtf  = \"" + systIdtfOfClass + "\">" + mainIdtf + "</sc_element></li>";
            }
        }
        return mappingResult;
    }

    string PlainTextRepresentationAgent::mapExample(const ScAddr &mainEntity, const ScAddr &language)
    {
        ScTemplate templ;
        templ.Triple(
                ScType::NodeVar >> "_node",
                ScType::EdgeAccessVarPosPerm >> "_edge_pos",
                mainEntity);
        templ.Triple(
                Keynodes::rrel_key_sc_element,
                ScType::EdgeAccessVarPosPerm,
                "_edge_pos");
        templ.Triple(
                ScType::NodeVar >> "_text_translation_node",
                ScType::EdgeDCommonVar >> "_edge_translation",
                "_node");
        templ.Triple(
                Keynodes::nrel_sc_text_translation,
                ScType::EdgeAccessVarPosPerm,
                "_edge_translation");
        templ.Triple(
                "_text_translation_node",
                ScType::EdgeAccessVarPosPerm >> "_edge_example",
                ScType::Link >> "_link");
        templ.Triple(
                Keynodes::rrel_example,
                ScType::EdgeAccessVarPosPerm,
                "_edge_example");
        templ.Triple(
                language,
                ScType::EdgeAccessVarPosPerm,
                "_link");

        ScTemplateSearchResult results;
        ms_context->HelperSearchTemplate(templ, results);

        if (results.Size() == 0)
            return ""; //no examples

        string mappingResult = "";
        if (language == Keynodes::lang_en)
            mappingResult += "<p>Examples: </p> ";
        else
            mappingResult += "<p>Примеры: </p> ";

        for (size_t a = 0; a < results.Size(); a++)
        {
            string example = getLinkContent(results[a]["_link"]);
            mappingResult += "<p>" + example + "</p>";
        }
        return mappingResult;
    }

    string PlainTextRepresentationAgent::getMainIdtf(const ScAddr &entity, const ScAddr &language)
    {
        string templName = "_link";
        ScTemplateSearchResult results = templateSearchForIdtf(entity, language, Keynodes::nrel_main_idtf, templName);

        if (results.Size() == 0)
            return ""; //no main idtf

        return getLinkContent(results[0][templName]);
    }

    string PlainTextRepresentationAgent::getLinkContent(const ScAddr &link)
    {
        ScStream stream;
        try
        {
            ms_context->GetLinkContent(link, stream);
        }
        catch (...)
        {
            return "";
        }

        size_t size = stream.Size();
        auto *text = new sc_char[size + 1];
        stream.Read(text, size, size);
        text[size] = '\0';
        text[0] = toupper(text[0]);
        string res(text);

        return res;
    }

    string PlainTextRepresentationAgent::getSystemIdtf(const ScAddr &entity)
    {
        try
        {
            return ms_context->HelperGetSystemIdtf(entity);
        }
        catch (...)
        {
            return "";
        }
    }

} // namespace exampleModule
