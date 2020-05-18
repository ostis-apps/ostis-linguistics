/*
* This source file is part of an OSTIS project. For the latest info, see http://ostis.net
* Distributed under the MIT License
* (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
*/

#include "keynodes.hpp"

#include <sc-memory/cpp/sc_memory.hpp>

namespace dialogModule
{

ScAddr Keynodes::message;
ScAddr Keynodes::set_messages_requiring_normalization;
ScAddr Keynodes::set_messages_requiring_parsing;
ScAddr Keynodes::not_normalized_word_form;
ScAddr Keynodes::rrel_normalization;
ScAddr Keynodes::nrel_sc_text_translation;
ScAddr Keynodes::rrel_standart;

}
