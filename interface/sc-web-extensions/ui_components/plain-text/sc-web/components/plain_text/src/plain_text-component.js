/**
 * PlainText component.
 */

PlainText.DrawComponent = {
    ext_lang: 'plain_text',
    formats: ['format_plain_text'],
    struct_support: true,
    factory: function (sandbox) {
        return new PlainText.PaintPanel(sandbox.container);;
    }
};

SCWeb.core.ComponentManager.appendComponentInitialize(PlainText.DrawComponent);
