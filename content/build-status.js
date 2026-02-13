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
    var reqPipeline = window.XDomainRequest ? new XDomainRequest() : new XMLHttpRequest();
    if(!reqPipeline)
        return;

    var repo = project.substring(project.lastIndexOf('/') + 1);

    var circleci_base_url = 'https://circleci.com';

    /* The query gives back just the latest "pipeline" ID and need to use that
       to query the actual "workflows" of given pipeline, to get their IDs, AND
       THEN use that to query the jobs. Couldn't there just be a way to query
       last jobs of given project directly?! Fucking!!! web stuff. */
    reqPipeline.open('GET', circleci_base_url + '/api/v2/project/gh/' + project + '/pipeline?branch=' + branch);
    reqPipeline.responseType = 'json';
    reqPipeline.onreadystatechange = function() {
        if(reqPipeline.readyState != 4)
            return;

        if(reqPipeline.response == null) {
            console.error('Cannot query CircleCI pipelines for', project);
            return;
        }

        /* If the response has no items, the last build was either more than 90
           days ago or this branch hasn't been built yet. */
        if(reqPipeline.response["items"].length == 0)
            return;

        /* Now query the actual workflows of the pipeline */
        var reqWorkflows = window.XDomainRequest ? new XDomainRequest() : new XMLHttpRequest();
        reqWorkflows.open('GET', circleci_base_url + '/api/v2/pipeline/' + reqPipeline.response["items"][0]["id"] + '/workflow');
        reqWorkflows.responseType = 'json';
        reqWorkflows.onreadystatechange = function() {
            if(reqWorkflows.readyState != 4)
                return;

            if(reqWorkflows.response == null) {
                console.error('Cannot query CircleCI workflows for', project);
                return;
            }

            /* If the response has no items, the build failed for example due
               to a YAML parse error */
            if(reqWorkflows.response["items"].length == 0) {
                console.error('No workflows for the latest CircleCI pipeline for', project, 'likely a YAML parse error');
                return;
            }

            var workflowCreatedAt = reqWorkflows.response["items"][0]["created_at"];
            var workflowId = reqWorkflows.response["items"][0]["id"];
            var pipelineNumber = reqWorkflows.response["items"][0]["pipeline_number"];

            /* And now... query the jobs. This is so fucking slow I cannot
               believe it. */
            var req = window.XDomainRequest ? new XDomainRequest() : new XMLHttpRequest();
            req.open('GET', circleci_base_url + '/api/v2/workflow/' + workflowId + '/job');
            req.responseType = 'json';
            req.onreadystatechange = function() {
                if(req.readyState != 4)
                    return;

                var now = new Date(Date.now());

                for(var i = 0; i != req.response["items"].length; ++i) {
                    var job = req.response["items"][i];

                    var id = job['project_slug'].substring(job['project_slug'].lastIndexOf('/') + 1) + '-' + job['name'];
                    var elem = document.getElementById(id);
                    if(!elem) {
                        console.log('Unknown CircleCI job ID', id);
                        continue;
                    }

                    var type;
                    var status;
                    var age;
                    if(job['status'] == 'success') {
                        type = 'm-success';
                        status = '✔';
                        age = job['stopped_at'];
                    } else if(job['status'] == 'running') {
                        type = 'm-warning';
                        status = '↺';
                        age = job['started_at'];
                    /* For example when I didn't pay for macOS */
                    } else if(job['status'] == 'not_run') {
                        type = 'm-dim';
                        status = '⚠';
                        age = job['stopped_at'];
                    } else if(job['status'] == 'failed' || job['status'] == 'infrastructure_fail' || job['status'] == 'timedout' || job['status'] == 'terminated-unknown'|| job['status'] == 'unauthorized') {
                        type = 'm-danger';
                        status = '✘';
                        age = job['stopped_at'];
                    /* When the job is blocked from running by dependencies
                       that either didn't finish yet or failed */
                    } else if(job['status'] == 'blocked') {
                        type = 'm-dim';
                        status = '⧖';
                        age = workflowCreatedAt;
                    } else if(job['status'] == 'queued') {
                        type = 'm-info';
                        status = '…';
                        age = job['queued_at'];
                    } else if(job['status'] == 'not_running') {
                        type = 'm-info';
                        status = '…';
                        age = job['usage_queued_at'];
                    } else if(job['status'] == 'canceled') {
                        type = 'm-dim';
                        status = '∅';
                        age = job['stopped_at'];
                    } else {
                        /* retried, on_hold -- not sure what exactly these
                           mean */
                        type = 'm-default';
                        status = job['status'];
                        age = job['created_at'];
                    }

                    var age = timeDiff(new Date(Date.parse(age)), now);

                    elem.innerHTML = '<a href="https://app.circleci.com/pipelines/github/' + job['project_slug'].substring(3) + '/' + pipelineNumber + '/workflows/' + workflowId + '/jobs/' + job['job_number'] + '" title="' + job['status'] + ' @ ' + age + '">' + status + '<br /><span class="m-text m-small">' + age + '</span></a>';
                    elem.className = type;
                }
            };
            req.send();
        };
        reqWorkflows.send();
    };
    reqPipeline.send();
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
