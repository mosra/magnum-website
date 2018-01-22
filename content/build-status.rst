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
          table.build-status td.m-success,
          table.build-status td.m-warning,
          table.build-status td.m-danger,
          table.build-status td.m-info,
          table.build-status td.m-dim {
            padding: 0;
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
-   `next branch <{filename}/build-status.rst?mosra/corrade=next&mosra/magnum=next&mosra/magnum-plugins=next&mosra/magnum-extras=next&mosra/magnum-integration=next&mosra/magnum-examples=next&mosra/magnum-examples=ports-next&mosra/magnum-bootstrap=next>`_

`Main repositories`_
====================

.. container:: m-container-inflate

    .. container:: m-scroll

        .. raw:: html
            :file: build-status.html

`Bootstrap projects`_
=====================

.. container:: m-container-inflate

    .. container:: m-scroll

        .. raw:: html
            :file: build-status-bootstrap.html
