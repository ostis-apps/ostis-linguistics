module.exports = function() {

    var kb = 'kb/ui_components/plain-text';
    var components = 'sc-web/components/plain_text/';
    var clientJsDirPath = '../../../../ostis-web-platform/sc-web/client/static/components/js/';

    return  {
        concat: {
            plain_text: {
                src: [
                    components + 'src/plain_text-common.js',
                    components + 'src/plain_text-component.js',
                    components + 'src/plain_text-paintPanel.js'],
                dest: clientJsDirPath + 'plain-text/plain-text.js'
            }
        },
        copy: {
            kb: {
                cwd: kb,
                src: ['*'],
                dest: '../../../../kb/ui_components/plain-text/',
                expand: true,
                flatten: true
            }
        },
        watch: {
            plain_text: {
                files: components + 'src/**',
                tasks: ['concat:plain_text']
            },
            copyKB: {
                files: [kb + '**',],
                tasks: ['copy:kb']
            }
        },
        exec: {
          updateCssAndJs: 'sh add-css-and-js.sh'
        }
    }
};

