var projects = [['mosra/corrade', 'master'],
                ['mosra/magnum', 'master'],
                ['mosra/magnum-plugins', 'master'],
                ['mosra/magnum-extras', 'master'],
                ['mosra/magnum-integration', 'master'],
                ['mosra/magnum-bindings', 'master'],
                ['mosra/magnum-examples', 'master'],
                ['mosra/magnum-examples', 'ports'],
                ['mosra/magnum-bootstrap', 'master'],
                ['mosra/magnum-singles', 'master'],
                ['mosra/flextgl', 'master'],
                ['mosra/homebrew-magnum', 'master']];
var travisJobIdRe = /JOBID=([a-zA-Z0-9-]+)/

var appveyorDone = 0;
var appveyorJobIdRe = /APPVEYOR_JOB_NAME=([a-zA-Z0-9-]+)/

/* Ability to override the projects via query string */
if(location.search) {
    let params = new URLSearchParams(location.search);
    projects = [];
    for(let p of params) projects.push(p);
}

function timeDiff(before, now) {
    var diff = now.getTime() - before.getTime();

    /* Try days first. If less than a day, try hours. If less than an hour, try
       minutes. If less than a minute, say "now". */
    if(diff/(24*60*60*1000) > 1)
        return Math.round(diff/(24*60*60*1000)) + "d ago";
    else if(diff/(60*60*1000) > 1)
        return Math.round(diff/(60*60*1000)) + "h ago";
    else if(diff/(60*1000) > 1)
        return Math.round(diff/(60*1000)) + "m ago";
    else
        return "now";
}

/* https://circleci.com/docs/api/#recent-builds-for-a-single-project */
function fetchLatestCircleCiJobs(project, branch) {
    var req = window.XDomainRequest ? new XDomainRequest() : new XMLHttpRequest();
    if(!req) return;

    /* TODO: expand the limit once we have more than 20 builds per project */
    req.open('GET', 'https://circleci.com/api/v1.1/project/github/' + project + '/tree/' + branch + '?limit=20&offset=0&shallow=true');
    req.responseType = 'json';
    req.onreadystatechange = function() {
        if(req.readyState != 4) return;

        //console.log(req.response);

        var now = new Date(Date.now());

        /* It's not possible to query just the latest build, so instead we have
           to query N latest jobs and then go as long as they have the same
           commit. Which is kinda silly, but better than going one-by-one like
           with Travis, right? */
        var commit = '';
        for(var i = 0; i != req.response.length; ++i) {
            var job = req.response[i];

            /* Some other commit, we have everything. Otherwise remember the
               commit for the next iteration. */
            if(commit && job['vcs_revision'] != commit)
                break;
            commit = job['vcs_revision'];

            /* If the YML fails to parse, job_name is Build Error. Skip it
               completely to avoid errors down the line. */
            if(job['workflows']['job_name'] == 'Build Error')
                continue;

            var id = job['reponame'] + '-' + job['workflows']['job_name'];
            var elem = document.getElementById(id);
            if(!elem) {
                console.log('Unknown CircleCI job ID', id);
                continue;
            }

            var type;
            var status;
            var ageField;
            if(job['status'] == 'success') {
                type = 'm-success';
                status = '✔';
                ageField = 'stop_time';
            } else if(job['status'] == 'queued' || job['status'] == 'scheduled') {
                type = 'm-info';
                status = '…';
                ageField = 'queued_at';
            } else if(job['status'] == 'not_running') {
                type = 'm-info';
                status = '…';
                ageField = 'usage_queued_at';
            } else if(job['status'] == 'running') {
                type = 'm-warning';
                status = '↺';
                ageField = 'start_time';
            } else if(job['status'] == 'failed' || job['status'] == 'infrastructure_fail' || job['status'] == 'timedout') {
                type = 'm-danger';
                status = '✘';
                ageField = 'stop_time';
            } else if(job['status'] == 'canceled') {
                type = 'm-dim';
                status = '∅';
                ageField = 'stop_time';
            /* For example when I didn't pay for macOS */
            } else if(job['status'] == 'not_run') {
                type = 'm-dim';
                status = '⚠';
                ageField = 'stop_time';
            } else {
                /* retried, no_test, fixed -- not sure
                   what exactly these mean */
                type = 'm-default';
                status = job['status'];
                ageField = 'usage_queued_at';
            }

            var age = timeDiff(new Date(Date.parse(job[ageField])), now);

            /* Update the field only if it's not already filled -- in that case
               it means this job got re-run. */
            if(!elem.className) {
                elem.innerHTML = '<a href="' + job['build_url'] + '" title="' + job['status'] + ' @ ' + job[ageField] + '">' + status + '<br /><span class="m-text m-small">' + age + '</span></a>';
                elem.className = type;
            }
        }
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

        //console.log(req.response);

        var now = new Date(Date.now());
        var repo = req.response['project']['repositoryName'];
        var jobs = req.response['build']['jobs'];
        for(var i = 0; i != jobs.length; ++i) {
            var match = jobs[i]['name'].match(appveyorJobIdRe);
            if(!match) continue;

            /* ID is combined repository name (w/o author) and the job ID from
               environment */
            var id = repo.substr(repo.indexOf('/') + 1) + "-" + match[1];
            var elem = document.getElementById(id);
            if(!elem) {
                console.log('Unknown AppVeyor job ID', id);
                continue;
            }

            var type;
            var status;
            var ageField;
            if(jobs[i]['status'] == 'success') {
                type = 'm-success';
                status = '✔';
                ageField = 'finished';
            } else if(jobs[i]['status'] == 'queued' || jobs[i]['status'] == 'starting') {
                type = 'm-info';
                status = '…';
                ageField = 'created';
            } else if(jobs[i]['status'] == 'running') {
                type = 'm-warning';
                status = '↺';
                ageField = 'started';
            } else if(jobs[i]['status'] == 'failed') {
                type = 'm-danger';
                status = '✘';
                ageField = 'finished';
            } else if(jobs[i]['status'] == 'cancelled') {
                type = 'm-dim';
                status = '∅';
                ageField = 'finished';
            } else {
                type = 'm-default';
                status = jobs[i]['status'];
                ageField = 'created';
            }

            var age = timeDiff(new Date(Date.parse(jobs[i][ageField])), now);

            elem.innerHTML = '<a href="https://ci.appveyor.com/project/' + repo + '/build/' + req.response['build']['version'] + '/job/' + jobs[i]['jobId'] + '" title="' + jobs[i]['status'] + ' @ ' + jobs[i][ageField] + '">' + status + '<br /><span class="m-text m-small">' + age + '</span></a>';
            elem.className = type;
        }
    };
    req.send();
}

function fetchLatestCodecovJobs(project, branch) {
    var req = window.XDomainRequest ? new XDomainRequest() : new XMLHttpRequest();
    if(!req) return;

    req.open("GET", 'https://api.codecov.io/api/v2/github/' + project.split('/')[0] + '/repos/' + project.split('/')[1] + '/branches/' + branch + '/', true);
    req.responseType = 'json';
    req.onreadystatechange = function() {
        if(req.readyState != 4) return;

        //console.log(req.response);

        var id = 'coverage-' + project.split('/')[1];
        var elem = document.getElementById(id);

        var commit = req.response['head_commit'];
        /* Floor to one decimal place so coverage of 99.96% doesn't result in
           misleading 100.0% being shown. toFixed() alone performs a rounding
           to nearest. Display the full value a tooltip. */
        var coverage = (Math.floor(commit['totals']['coverage']*10)*0.1).toFixed(1);
        var coverageFull = commit['totals']['coverage'];
        var type;
        if(commit['state'] != 'complete') type = 'm-info';
        else if(Math.round(coverage) > 90) type = 'm-success';
        else if(Math.round(coverage) > 50) type = 'm-warning';
        else type = 'm-danger';

        var date = req.response['updatestamp'];
        var age = timeDiff(new Date(Date.parse(date)), new Date(Date.now()));

        elem.innerHTML = '<a href="https://app.codecov.io/github/' + project + '/tree/' + branch + '" title="' + coverageFull + '% @ ' + date + '"><strong>' + coverage + '</strong>%<br /><span class="m-text m-small">' + age + '</span></a>';
        elem.className = type;
    };
    req.send();
}

function fetch() {
    for(var i = 0; i != projects.length; ++i) {
        fetchLatestCircleCiJobs(projects[i][0], projects[i][1]);
        /* These are not on AppVeyor */
        if(projects[i][0].indexOf('flextgl') === -1 &&
           projects[i][0].indexOf('homebrew') === -1 &&
           projects[i][0].indexOf('magnum-singles') === -1)
            fetchLatestAppveyorJobs(projects[i][0], projects[i][1]);
        /* These don't have coverage reports */
        if(projects[i][0].indexOf('magnum-examples') == -1 &&
           projects[i][0].indexOf('magnum-bootstrap') == -1 &&
           projects[i][0].indexOf('magnum-singles') == -1 &&
           projects[i][0].indexOf('homebrew') === -1)
            fetchLatestCodecovJobs(projects[i][0], projects[i][1]);
    }
}

fetch();
