var Example = {};

function extend(child, parent) {
    var F = function () {
    };
    F.prototype = parent.prototype;
    child.prototype = new F();
    child.prototype.constructor = child;
    child.superclass = parent.prototype;
}

/**
 * Example component.
 */
Example.DrawComponent = {
    ext_lang: 'plain_text',
    formats: ['format_plain_text'],
    struct_support: true,
    factory: function (sandbox) {
        return new Example.DrawWindow(sandbox);
    }
};

Example.DrawWindow = function (sandbox) {
    this.sandbox = sandbox;
    this.paintPanel = new Example.PaintPanel(this.sandbox.container);
    this.paintPanel.init();
    this.recieveData = function (data) {
        console.log("in recieve data" + data);
    };

    var scElements = {};

    function drawAllElements() {
        var dfd = new jQuery.Deferred();
       // for (var addr in scElements) {
            jQuery.each(scElements, function(j, val){
                var obj = scElements[j];
                if (!obj || obj.translated) return;
// check if object is an arc
                if (obj.data.type & sc_type_arc_pos_const_perm) {
                    var begin = obj.data.begin;
                    var end = obj.data.end;
                    // logic for component update should go here
                }

        });
        SCWeb.ui.Locker.hide();
        dfd.resolve();
        return dfd.promise();
    }

// resolve keynodes
    var self = this;
    this.needUpdate = false;
    this.requestUpdate = function () {
        var updateVisual = function () {
// check if object is an arc
            var dfd1 = drawAllElements();
            dfd1.done(function (r) {
                return;
            });


/// @todo: Don't update if there are no new elements
            window.clearTimeout(self.structTimeout);
            delete self.structTimeout;
            if (self.needUpdate)
                self.requestUpdate();
            return dfd1.promise();
        };
        self.needUpdate = true;
        if (!self.structTimeout) {
            self.needUpdate = false;
            SCWeb.ui.Locker.show();
            self.structTimeout = window.setTimeout(updateVisual, 1000);
        }
    }
    
    this.eventStructUpdate = function (added, element, arc) {
        window.sctpClient.get_arc(arc).done(function (r) {
            var addr = r[1];
            window.sctpClient.get_element_type(addr).done(function (t) {
                var type = t;
                var obj = new Object();
                obj.data = new Object();
                obj.data.type = type;
                obj.data.addr = addr;
                if (type & sc_type_arc_mask) {
                    window.sctpClient.get_arc(addr).done(function (a) {
                        obj.data.begin = a[0];
                        obj.data.end = a[1];
                        scElements[addr] = obj;
                        self.requestUpdate();
                    });
                }
            });
        });
    };
// delegate event handlers
    this.sandbox.eventDataAppend = $.proxy(this.receiveData, this);
    this.sandbox.eventStructUpdate = $.proxy(this.eventStructUpdate, this);
    this.sandbox.updateContent();
};
SCWeb.core.ComponentManager.appendComponentInitialize(Example.DrawComponent);
/**
 * Paint panel.
 */

Example.PaintPanel = function (containerId) {
	this.containerId = containerId;
};

Example.PaintPanel.prototype = {

	init: function () {
		this._initMarkup(this.containerId);
	},

	_initMarkup: function (containerId) {
		this._callPlainTextRepresentationAgent(containerId);
	},

	/* Call agent of searching semantic neighborhood,
	send ui_main_menu node as parameter and add it in web window history
	*/
	_callPlainTextRepresentationAgent: function (containerId) {
		var container = $('#' + containerId);
		var self = this;
		//Resolve sc-addr of current language
		var lang = $('#language-select').find(":selected")[0].attributes['sc_addr'].value;
		// Resolve sc-addr of question
		var question = $('#history-items a').first().attr("sc_addr");
		// Find sc-addr of the question argument
		window.sctpClient.iterate_elements(SctpIteratorType.SCTP_ITERATOR_3F_A_A, [
			question,
			sc_type_arc_pos_const_perm,
			sc_type_node]).
			done(function (it) {
				var question_arg = it[0][2];
				console.log('question_arg: ' + question_arg);
				// Resolve sc-addr of ui_menu_plain_text_representation node
				SCWeb.core.Server.resolveScAddr(["ui_menu_plain_text_representation"],
					function (data) {
						// Get command of ui_menu_plain_text_representation
						var cmd = data["ui_menu_plain_text_representation"];
						console.log('ui_menu_plain_text_representation: ' + cmd);
						// Simulate click on ui_menu_plain_text_representation button
						SCWeb.core.Server.doCommand(cmd,
							[question_arg, lang], function (plain_text_result) {

								console.log("inDoCommand");

								var result_question_node = plain_text_result.question;
								console.log('result_question_node: ' + result_question_node);


								setTimeout(function () {
									// Some sleep
									console.log('sleep');
									// Resolve sc-addr of nrel_answer
									SCWeb.core.Server.resolveScAddr(['nrel_answer'], function (keynodes) {
										var nrel_answer_addr = keynodes['nrel_answer'];
										console.log('nrel_answer_addr: ' + nrel_answer_addr);
										// Get answer node from iter
										window.sctpClient.iterate_elements(SctpIteratorType.SCTP_ITERATOR_5F_A_A_A_F, [
											result_question_node,
											sc_type_arc_common | sc_type_const,
											sc_type_node,
											sc_type_arc_pos_const_perm,
											nrel_answer_addr])
											.done(function (iter) {
												console.log('Iterator: ' + iter);

												var answer = iter[0][2];
												console.log('AnswerLink: ' + answer);

												// Get content from answer
												window.sctpClient.iterate_elements(SctpIteratorType.SCTP_ITERATOR_3F_A_A, [
													answer,
													sc_type_arc_pos_const_perm,
													sc_type_link])
													.done(function (it3) {
														console.log('it3 found');
														var answ_cont = it3[0][2];

														window.sctpClient.get_link_content(answ_cont, 'string')
															.done(function (content) {
																console.log('Content found');
																// console.log(content);

																// Add style to content
																content = self._addStyleToContent(content);
																// Add sc-addr for sc-elements in content
																content = self._addScAddrsToContent(content, container);
																// Add content to container
																// console.log(content);
																// container.append(content);
															})//done content
															.fail(function () {
																console.log('fail to get content');
															})//fail content
													})//done it3
													.fail(function () {
														console.log('fail to find it3');
													});//fail it3
											})//done iter
											.fail(function () {
												console.log('fail to find iter');
											});//fail iter
									});//resolve nrel_answer
								}, 1000);//timeout
							});//SCWeb.core.Server.doCommand
					});//SCWeb.core.Server.resolveScAddr
			});//done
	},//_callPlainTextRepresentationAgent

	_addStyleToContent: function(content){
		content = '<div><style>P {max-width: 700px;} LI{max-width: 700px;}</style>'
			+ content + '</div>';
		return content;
	},

	_addScAddrsToContent: function(content, container){
		console.log('in_addScAddrsToContent');

		// Create DocumentFragment from html content
		var parser = new DOMParser();
		var doc_fragment = parser.parseFromString(content, "text/html");

		// Get list of sc_elements
		var elements = doc_fragment.getElementsByTagName('sc_element');
		console.log('elements.length: ' + elements.length);
		var i;

		$.each( elements, function( i, value ){
			var element = elements[i];
			console.log('sc_element[' + i + ']: ' + element.innerHTML);

			// Resolve sc_addr of element
			var idtf = element.getAttribute('sys_idtf');
			console.log('idtf: ' + idtf);
			SCWeb.core.Server.resolveScAddr([idtf], function (keynodes) {
				var element_sc_addr = '' + keynodes[idtf];
				// console.log('element_sc_addr: ' + element_sc_addr);
				// Add sc_addr attribute
				element.setAttribute('sc_addr', element_sc_addr);
				// console.log('added: ' + element.getAttribute('sc_addr'));
				$(element).css("text-decoration", "underline");
			});
		});

		setTimeout(function () {

			// Update content
			var temporary_div = document.createElement('temporary_div');
			temporary_div.appendChild( doc_fragment.documentElement.cloneNode(true) );
			content = temporary_div.innerHTML;
			console.log('content after adding sc_addr');
			console.log(content);
			container.append(content);
			// document.removeChild(temporary_div);
		}, 1000);//timeout
	}
}