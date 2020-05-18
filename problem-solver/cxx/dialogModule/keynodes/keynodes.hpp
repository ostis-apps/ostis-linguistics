/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#pragma once

#include "sc-memory/cpp/sc_addr.hpp"
#include "sc-memory/cpp/sc_object.hpp"

#include "keynodes.generated.hpp"

namespace dialogModule
{

class Keynodes : public ScObject
{
  SC_CLASS()
  SC_GENERATED_BODY()

public:

    SC_PROPERTY(Keynode("message"), ForceCreate)
    static ScAddr message;

  SC_PROPERTY(Keynode("set_messages_requiring_normalization"), ForceCreate)
  static ScAddr set_messages_requiring_normalization;

  SC_PROPERTY(Keynode("question_text_normalization"), ForceCreate)
  static ScAddr question_text_normalization;

    SC_PROPERTY(Keynode("set_messages_requiring_parsing"), ForceCreate)
    static ScAddr set_messages_requiring_parsing;

    SC_PROPERTY(Keynode("not_normalized_word_form"), ForceCreate)
    static ScAddr not_normalized_word_form;

    SC_PROPERTY(Keynode("rrel_normalization"), ForceCreate)
    static ScAddr rrel_normalization;

    SC_PROPERTY(Keynode("nrel_sc_text_translation"), ForceCreate)
    static ScAddr nrel_sc_text_translation;

    SC_PROPERTY(Keynode("rrel_standart"), ForceCreate)
    static ScAddr rrel_standart;
};

} // namespace exampleModule
