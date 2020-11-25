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

function fetchTravisJobStatus(latestJobs) {
    var req = window.XDomainRequest ? new XDomainRequest() : new XMLHttpRequest();
    if(!req) return;

    req.open("GET", 'https://api.travis-ci.com/jobs?ids[]=' + latestJobs.join('&ids[]='), true);
    req.setRequestHeader("Accept", "application/vnd.travis-ci.2.1+json");
    req.responseType = 'json';
    req.onreadystatechange = function() {
        if(req.readyState != 4) return;

        //console.log(req.response);

        var now = new Date(Date.now());
        var jobs = req.response['jobs'];
        for(var i = 0; i != jobs.length; ++i) {
            var env = jobs[i]['config']['env'];
            if(!env) {
                console.log("Travis job doesn't have any environment, skipping:", jobs[i]['repository_slug']);
                continue;
            }
            var match = env.match(travisJobIdRe);
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

            var type;
            var status;
            var ageField;
            if(jobs[i]['state'] == 'passed') {
                /* Travis CI is useless right now, so it doesn't make sense to
                   mark the increasingly stale builds green. */
                type = 'm-dim';
                status = '✔';
                ageField = 'finished_at';
            } else if(jobs[i]['state'] == 'started') {
                type = 'm-warning';
                status = '↺';
                ageField = 'started_at';
            } else if(jobs[i]['state'] == 'canceled') {
                type = 'm-dim';
                status = '∅';
                ageField = 'finished_at';
            } else if(jobs[i]['state'] == 'received' ||
                      jobs[i]['state'] == 'created' ||
                      jobs[i]['state'] == 'queued') {
                type = 'm-info';
                status = '…';
                ageField = '';
            } else if(jobs[i]['state'] == 'errored' ||
                      jobs[i]['state'] == 'failed') {
                type = 'm-danger';
                status = '✘';
                ageField = 'finished_at';
            } else {
                type = 'm-default';
                status = jobs[i]['state'];
                ageField = 'started_at';
            }

            var age;
            var title;
            if(ageField) {
                age = timeDiff(new Date(Date.parse(jobs[i][ageField])), now);
                title = jobs[i]['state'] + ' @ ' + jobs[i][ageField];
            } else {
                age = '&nbsp;'; /* To avoid layout jumps */
                title = jobs[i]['state'];
            }

            /* Update the field only if it's not already provided by some other
               provider */
            if(!elem.className) {
                elem.innerHTML = '<a href="https://travis-ci.com/' + repo + '/jobs/' + jobs[i]['id'] + '" title="' + title + '">' + status + '<br /><span class="m-text m-small">' + age + '</span></a>';
                elem.className = type;
            }
        }
    };
    req.send();
}

function fetchLatestTravisJobs(project, branch) {
    var req = window.XDomainRequest ? new XDomainRequest() : new XMLHttpRequest();
    if(!req) return;

    req.open("GET", 'https://api.travis-ci.com/repos/' + project + '/branches/' + branch, true);
    req.setRequestHeader("Accept", "application/vnd.travis-ci.2.1+json");
    req.responseType = 'json';
    req.onreadystatechange = function() {
        if(req.readyState != 4 || req.status != 200) return;

        /* Originally I was putting all these IDs into a giant array and
           fetching it just once. But I don't care anymore. */
        fetchTravisJobStatus(req.response['branch']['job_ids']);
    };
    req.send();
}

/* https://circleci.com/docs/api/#recent-builds-for-a-single-project */
function fetchLatestCircleCiJobs(project, branch) {
    var req = window.XDomainRequest ? new XDomainRequest() : new XMLHttpRequest();
    if(!req) return;

    /* TODO: expand the limit once we have more than 10 builds per project */
    req.open('GET', 'https://circleci.com/api/v1.1/project/github/' + project + '/tree/' + branch + '?limit=10&offset=0&shallow=true');
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
            } else if(job['status'] == 'queued' || job['status'] == 'scheduled' || job['status'] == 'not_running') {
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
            } else {
                /* retried, not_run, not_running, no_test, fixed -- not sure
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

    req.open("GET", 'https://codecov.io/api/gh/' + project + '/branch/' + branch, true);
    req.responseType = 'json';
    req.onreadystatechange = function() {
        if(req.readyState != 4) return;

        //console.log(req.response);

        var repo = req.response['repo']['name'];
        var id = 'coverage-' + repo;
        var elem = document.getElementById(id);

        var commit = req.response['commit'];
        var coverage = (commit['totals']['c']*1.0).toFixed(1);
        var type;
        if(commit['state'] != 'complete') type = 'm-info';
        else if(Math.round(coverage) > 90) type = 'm-success';
        else if(Math.round(coverage) > 50) type = 'm-warning';
        else type = 'm-danger';

        var date = commit['updatestamp'];
        var age = timeDiff(new Date(Date.parse(date)), new Date(Date.now()));

        elem.innerHTML = '<a href="https://codecov.io/gh/mosra/' + repo + '/tree/' + commit['commitid'] + '" title="@ ' + date + '"><strong>' + coverage + '</strong>%<br /><span class="m-text m-small">' + age + '</span></a>';
        elem.className = type;
    };
    req.send();
}

function fetch() {
    for(var i = 0; i != projects.length; ++i) {
        fetchLatestTravisJobs(projects[i][0], projects[i][1]);
        /* Only some are on CircleCI right now */
        if(projects[i][0].indexOf('corrade') != -1 ||
           projects[i][0].endsWith('/magnum'))
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
