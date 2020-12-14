/**
 * PlainText component.
 */

PlainText.DrawComponent = {
    ext_lang: 'plain_text',
    formats: ['format_plain_text'],
    struct_support: true,
    factory: function (sandbox) {
        var paintPanel = new PlainText.PaintPanel(sandbox.container);
        paintPanel.init();
        sandbox.updateContent();
        return paintPanel;
    }
};

SCWeb.core.ComponentManager.appendComponentInitialize(PlainText.DrawComponent);
