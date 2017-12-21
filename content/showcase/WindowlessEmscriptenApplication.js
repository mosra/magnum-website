/* Patched version of the upstream Magnum driver code in order to support
   having extra files on another domain. Be careful when updating. The module
   <script> tag has to be *right before* this one for the following to work. */
var scripts = document.getElementsByTagName('script');
var moduleSrc = scripts[scripts.length - 2].src;
var moduleBaseDir = moduleSrc.substr(0, moduleSrc.lastIndexOf('/') + 1);

var Module = {
    preRun: [],
    postRun: [],

    arguments: [],

    printErr: function(message) {
        var log = document.getElementById('log');
        log.innerHTML += Array.prototype.slice.call(arguments).join(' ') + '\n';
    },

    print: function(message) {
        var log = document.getElementById('log');
        log.innerHTML += Array.prototype.slice.call(arguments).join(' ') + '\n';
    },

    locateFile: function(url) { return moduleBaseDir + url; },

    canvas: document.getElementById('module'),

    setStatus: function(message) {
        var status = document.getElementById('status');
        if(status) status.innerHTML = message;
    },

    setStatusDescription: function(message) {
        var statusDescription = document.getElementById('statusDescription');
        if(statusDescription) statusDescription.innerHTML = message;
    },

    totalDependencies: 0,

    monitorRunDependencies: function(left) {
        this.totalDependencies = Math.max(this.totalDependencies, left);

        if(left) {
            Module.setStatus('Downloading...');
            Module.setStatusDescription((this.totalDependencies - left) + '/' + this.totalDependencies);
        } else {
            Module.setStatus('');
            Module.setStatusDescription('');
            document.getElementById('log').style.display = 'block';
        }
    }
};

/* Parse arguments, e.g. /app/?foo=bar&fizz&buzz=3 goes to the app as
   ['--foo', 'bar', '--fizz', '--buzz', '3'] */
var args = decodeURIComponent(window.location.search.substr(1)).trim().split('&');
for(var i = 0; i != args.length; ++i) {
    var j = args[i].indexOf('=');
    /* Key + value */
    if(j != -1) {
        Module.arguments.push('--' + args[i].substring(0, j));
        Module.arguments.push(args[i].substring(j + 1));

    /* Just key */
    } else Module.arguments.push('--' + args[i]);
}

Module.setStatus('Downloading...');

document.getElementById('log').style.display = 'none';

if(Module.canvas) Module.canvas.addEventListener('contextmenu', function(event) {
    event.preventDefault();
}, true);
