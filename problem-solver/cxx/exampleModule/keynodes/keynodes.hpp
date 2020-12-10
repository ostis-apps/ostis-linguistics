/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/sc_addr.hpp"
#include "sc-memory/cpp/sc_object.hpp"

#include "keynodes.generated.hpp"

namespace exampleModule
{

class Keynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:
    SC_PROPERTY(Keynode("question_find_subdividing"), ForceCreate)
    static ScAddr question_find_subdividing;

    SC_PROPERTY(Keynode("question_message_classification"), ForceCreate)
    static ScAddr question_message_classification;

    SC_PROPERTY(Keynode("question_plain_text_representation"), ForceCreate)
    static ScAddr question_plain_text_representation;

    SC_PROPERTY(Keynode("lang_ru"), ForceCreate)
    static ScAddr lang_ru;

    SC_PROPERTY(Keynode("lang_en"), ForceCreate)
    static ScAddr lang_en;

    SC_PROPERTY(Keynode("nrel_main_idtf"), ForceCreate)
    static ScAddr nrel_main_idtf;

    SC_PROPERTY(Keynode("nrel_idtf"), ForceCreate)
    static ScAddr nrel_idtf;

    SC_PROPERTY(Keynode("rrel_example"), ForceCreate)
    static ScAddr rrel_example;

    SC_PROPERTY(Keynode("rrel_key_sc_element"), ForceCreate)
    static ScAddr rrel_key_sc_element;

    SC_PROPERTY(Keynode("definition"), ForceCreate)
    static ScAddr definition;

    SC_PROPERTY(Keynode("statement"), ForceCreate)
    static ScAddr statement;

    SC_PROPERTY(Keynode("nrel_subdividing"), ForceCreate)
    static ScAddr nrel_subdividing;

    SC_PROPERTY(Keynode("nrel_plain_text_representation"), ForceCreate)
    static ScAddr nrel_plain_text_representation;

    SC_PROPERTY(Keynode("nrel_sc_text_translation"), ForceCreate)
    static ScAddr nrel_sc_text_translation;

    SC_PROPERTY(Keynode("nrel_concatenation"), ForceCreate)
    static ScAddr nrel_concatenation;

    SC_PROPERTY(Keynode("rrel_1"), ForceCreate)
    static ScAddr rrel_1;

    SC_PROPERTY(Keynode("rrel_2"), ForceCreate)
    static ScAddr rrel_2;

    SC_PROPERTY(Keynode("concept_lexeme"), ForceCreate)
    static ScAddr concept_lexeme;

    SC_PROPERTY(Keynode("nrel_paradigm"), ForceCreate)
    static ScAddr nrel_paradigm;

    SC_PROPERTY(Keynode("nrel_intersection"), ForceCreate)
    static ScAddr nrel_intersection;

    SC_PROPERTY(Keynode("concept_tokens_category"), ForceCreate)
    static ScAddr concept_tokens_category;

    SC_PROPERTY(Keynode("nrel_message_key_elements"), ForceCreate)
    static ScAddr nrel_message_key_elements;

};

} // namespace exampleModule
