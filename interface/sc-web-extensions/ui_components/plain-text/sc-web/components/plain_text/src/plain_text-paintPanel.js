/**
 * Paint panel.
 */
PlainText.PaintPanel = function (containerId) {
	this.containerId = containerId;
    this.init();
};

PlainText.PaintPanel.prototype = {

	init: function () {
		this._callPlainTextRepresentationAgent(this.containerId);
	},
	
	/* Call agent of searching semantic neighborhood,
	send ui_main_menu node as parameter and add it in web window history
	*/
	_callPlainTextRepresentationAgent: function (containerId) {
		var container = $('#' + containerId);
		var self = this;
		var lang = $('#language-select').find(":selected")[0].attributes['sc_addr'].value;
		var question = $('a.history-item.active').attr("sc_addr");
		window.sctpClient.iterate_elements(SctpIteratorType.SCTP_ITERATOR_3F_A_A, [
			question,
			sc_type_arc_pos_const_perm,
			sc_type_node]).
			done(function (it) {
				var question_arg = it[0][2];
				SCWeb.core.Server.resolveScAddr(["ui_menu_plain_text_representation"],
					function (data) {
						var cmd = data["ui_menu_plain_text_representation"];
						SCWeb.core.Server.doCommand(cmd,
							[question_arg, lang], function (plain_text_result) {
								console.log("PlainTextRepresentationAgent is done");
								var result_question_node = plain_text_result.question;
								setTimeout(function () {
									SCWeb.core.Server.resolveScAddr(['nrel_answer'], function (keynodes) {
										var nrel_answer_addr = keynodes['nrel_answer'];
										window.sctpClient.iterate_elements(SctpIteratorType.SCTP_ITERATOR_5F_A_A_A_F, [
											result_question_node,
											sc_type_arc_common | sc_type_const,
											sc_type_node,
											sc_type_arc_pos_const_perm,
											nrel_answer_addr])
											.done(function (iter) {
												var answer = iter[0][2];
												window.sctpClient.iterate_elements(SctpIteratorType.SCTP_ITERATOR_3F_A_A, [
													answer,
													sc_type_arc_pos_const_perm,
													sc_type_link])
													.done(function (it3) {
														var answ_cont = it3[0][2];
														window.sctpClient.get_link_content(answ_cont, 'string')
															.done(function (content) {
																content = self._addStyleToContent(content);
																content = self._addScAddrsToContent(content, container);
			                                                    console.log('plain-text view is done');
															});
													});
											});
									});
								}, 1000);
							});
					});
			});
	},

	_addStyleToContent: function(content){
		content = '<div><style>P {max-width: 700px;} LI{max-width: 700px;}</style>'
			+ content + '</div>';
		return content;
	},

	_addScAddrsToContent: function(content, container){
		var parser = new DOMParser();
		var doc_fragment = parser.parseFromString(content, "text/html");
		var elements = doc_fragment.getElementsByTagName('sc_element');
		var i;
		$.each( elements, function( i, value ){
			var element = elements[i];
			var idtf = element.getAttribute('sys_idtf');
			SCWeb.core.Server.resolveScAddr([idtf], function (keynodes) {
				var element_sc_addr = '' + keynodes[idtf];
				element.setAttribute('sc_addr', element_sc_addr);
				$(element).css("text-decoration", "underline");
			});
		});
		setTimeout(function () {
			var temporary_div = document.createElement('temporary_div');
			temporary_div.appendChild( doc_fragment.documentElement.cloneNode(true) );
			content = temporary_div.innerHTML;
			container.append(content);
		}, 1000);
	}
}
