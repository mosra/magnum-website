var projects = [['mosra/corrade', 'master'],
                ['mosra/magnum', 'master'],
                ['mosra/magnum-plugins', 'master'],
                ['mosra/magnum-extras', 'master'],
                ['mosra/magnum-integration', 'master'],
                ['mosra/magnum-examples', 'master'],
                ['mosra/magnum-examples', 'ports'],
                ['mosra/magnum-bootstrap', 'master']];
var latestTravisJobs = [];
var travisDone = 0;
var travisJobIdRe = /JOBID=([a-zA-Z0-9-]+)/

var appveyorDone = 0;
var appveyorJobIdRe = /APPVEYOR_JOB_NAME=([a-zA-Z0-9-]+)/

/* Ability to override the projects via query string */
if(location.search) {
    let params = new URLSearchParams(location.search);
    projects = []
    for(let p of params) projects.push(p);

    console.log(projects);
}

function fetchTravisJobStatus(latestJobs) {
    var req = window.XDomainRequest ? new XDomainRequest() : new XMLHttpRequest();
    if(!req) return;

    req.open("GET", 'https://api.travis-ci.org/jobs?ids[]=' + latestJobs.join('&ids[]='), true);
    req.setRequestHeader("Accept", "application/vnd.travis-ci.2+json");
    req.responseType = 'json';
    req.onreadystatechange = function() {
        if(req.readyState != 4) return;

        console.log(req.response);

        var jobs = req.response['jobs'];
        for(var i = 0; i != jobs.length; ++i) {
            var match = jobs[i]['config']['env'].match(travisJobIdRe);
            if(!match) continue;

            /* ID is combined repository name (w/o author) and the job ID from
               environment */
            var repo = jobs[i]['repository_slug'];
            var id = repo.substr(repo.indexOf('/') + 1) + "-" + match[1];
            var elem = document.getElementById(id);
            if(!elem) {
                console.log('Unknown Travis job ID', id);
                continue;
            }

            var type = '';
            var status = jobs[i]['state'];
            if(jobs[i]['state'] == 'passed') {
                type = 'm-success';
                status = '✔';
            } else if(jobs[i]['state'] == 'started') {
                type = 'm-warning';
                status = '↺';
            } else if(jobs[i]['state'] == 'canceled') {
                type = 'm-dim';
                status = '∅';
            } else if(jobs[i]['state'] == 'received' ||
                    jobs[i]['state'] == 'created' ||
                    jobs[i]['state'] == 'queued') {
                type = 'm-info';
                status = '…';
            } else if(jobs[i]['state'] == 'errored' ||
                    jobs[i]['state'] == 'failed') {
                type = 'm-danger';
                status = '✘';
            } else type = 'm-default';

            elem.innerHTML = '<a href="https://travis-ci.org/' + jobs[i]['repository_slug'] + '/jobs/' + jobs[i]['id'] + '">' + status + '</a>';
            elem.className = type;
        }
    };
    req.send();
}

function fetchLatestTravisJobs(project, branch) {
    var req = window.XDomainRequest ? new XDomainRequest() : new XMLHttpRequest();
    if(!req) return;

    req.open("GET", 'https://api.travis-ci.org/repos/' + project + '/branches/' + branch, true);
    req.setRequestHeader("Accept", "application/vnd.travis-ci.2+json");
    req.responseType = 'json';
    req.onreadystatechange = function() {
        if(req.readyState != 4) return;

        latestTravisJobs = latestTravisJobs.concat(req.response['branch']['job_ids']);
        if(++travisDone == projects.length)
            fetchTravisJobStatus(latestTravisJobs);
    };
    req.send();
}

function fetchLatestAppveyorJobs(project, branch) {
    var req = window.XDomainRequest ? new XDomainRequest() : new XMLHttpRequest();
    if(!req) return;

    req.open("GET", 'https://ci.appveyor.com/api/projects/' + project + '/branch/' + branch, true);
    req.responseType = 'json';
    req.onreadystatechange = function() {
        if(req.readyState != 4) return;

        console.log(req.response);

        var repo = req.response['project']['repositoryName'];
        repo = repo.substr(repo.indexOf('/') + 1);
        var jobs = req.response['build']['jobs'];
        for(var i = 0; i != jobs.length; ++i) {
            var match = jobs[i]['name'].match(appveyorJobIdRe);
            if(!match) continue;

            /* ID is combined repository name (w/o author) and the job ID from
               environment */
            var id = repo + "-" + match[1];
            var elem = document.getElementById(id);
            if(!elem) {
                console.log('Unknown AppVeyor job ID', id);
                continue;
            }

            var type = '';
            var status = jobs[i]['status'];
            if(jobs[i]['status'] == 'success') {
                type = 'm-success';
                status = '✔';
            } else if(jobs[i]['status'] == 'queued') {
                type = 'm-info';
                status = '…';
            } else if(jobs[i]['status'] == 'running') {
                type = 'm-warning';
                status = '↺';
            } else if(jobs[i]['status'] == 'failed') {
                type = 'm-danger';
                status = '✘';
            } else type = 'm-default';

            elem.innerHTML = '<a href="https://ci.appveyor.com/project/' + req.response['project']['repositoryName'] + '/build/' + req.response['build']['version'] + '/job/' + jobs[i]['jobId'] + '">' + status + '</a>';
            elem.className = type;
        }
    };
    req.send();
}

for(var i = 0; i != projects.length; ++i) {
    fetchLatestTravisJobs(projects[i][0], projects[i][1]);
    fetchLatestAppveyorJobs(projects[i][0], projects[i][1]);
}
