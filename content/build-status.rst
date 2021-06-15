Build Status
############

:summary: CI build status of all Magnum projects
:footer:
    .. raw:: html

        <style>
          table.build-status th, table.build-status td {
            text-align: center;
            vertical-align: middle;
          }
          table.build-status th svg {
            fill: #dcdcdc /*var(--color)*/;
          }
          table.build-status td {
            padding: 0;
            min-width: 5rem;
          }
          table.build-status a {
            display: block;
            width: 100%;
            height: 100%;
            padding: 0.25rem 0.5rem;
            text-decoration: none;
          }
        </style>
        <script>

    .. raw:: html
        :file: build-status.js

    .. raw:: html

        </script>

Show builds for:

-   `master branch <{filename}/build-status.rst>`_
-   `next branch <{filename}/build-status.rst?mosra/corrade=next&mosra/magnum=next&mosra/magnum-plugins=next&mosra/magnum-extras=next&mosra/magnum-integration=next&mosra/magnum-bindings=next&mosra/magnum-examples=next&mosra/magnum-examples=ports-next&mosra/magnum-bootstrap=next&mosra/flextgl=next&mosra/magnum-singles=next&mosra/homebrew-magnum=next>`_

.. block-warning:: Travis CI woes

    Travis CI recently switched all OSS projects to `trial plans with a one-time credit allowance <https://blog.travis-ci.com/2020-11-02-travis-ci-new-billing>`_,
    practically preventing everyone from using their services. Affected builds,
    which will get increasingly stale, are marked with a :label-flat-dim:`dim color`
    and migration to other services is underway. Stability of the ``master``
    branch is temporarily not guaranteed on iOS and Android platforms.

`Main repositories`_
====================

.. container:: m-container-inflate

    .. container:: m-scroll

        .. raw:: html
            :file: build-status.html

`Related projects`_
===================

.. container:: m-row

    .. container:: m-col-m-4 m-nopadt

        .. raw:: html
            :file: build-status-flextgl.html

    .. container:: m-col-m-4 m-nopadt

        .. raw:: html
            :file: build-status-singles.html

    .. container:: m-col-m-4 m-nopadt

        .. raw:: html
            :file: build-status-brew.html

`Bootstrap projects`_
=====================

.. container:: m-container-inflate

    .. container:: m-scroll

        .. raw:: html
            :file: build-status-bootstrap.html
