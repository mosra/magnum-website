Doc Downloads
#############

:summary: Downloads of Corrade and Magnum documentation

Up-to-date documentation for Corrade, Magnum and Magnum Python Bindings is
actively maintained on

-   https://doc.magnum.graphics/corrade/,
-   https://doc.magnum.graphics/magnum/ and
-   https://doc.magnum.graphics/python/.

.. class:: m-noindent

While it's possible to scrape these for offline viewing, it's advised to build
your own version locally instead. Guide how to do that is available
:dox:`in the documentation <building-doc>`. Thank you for being nice to server
bandwidth.

`Doxygen tag files`_
====================

If you have a project using Magnum and want to cross-link to Magnum
documentation from your Doxygen docs, you can make use of the following
`tag files <http://www.doxygen.nl/manual/external.html>`_:

-   `corrade.tag <https://doc.magnum.graphics/corrade.tag>`_ (:filesize:`{static}/doc/corrade.tag`)
-   `magnum.tag <https://doc.magnum.graphics/magnum.tag>`_ (:filesize:`{static}/doc/magnum.tag`)

Download the files into your project and reference them from your ``Doxyfile``
like shown below. If Doxygen complains about duplicate symbols, use just one of
the files.

.. code:: ini

    TAGFILES += corrade.tag=https://doc.magnum.graphics/corrade/ \
                magnum.tag=https://doc.magnum.graphics/magnum/

.. block-info:: Linking to STL documentation

    Corrade and Magnum docs also cross-link to STL documentation at
    `cppreference.com <https://en.cppreference.com>`_. To do that in your
    project as well, download `their tag file <https://en.cppreference.com/w/Cppreference:Archives>`_ (look in the ``html-book-*`` archives for an
    up-to-date version) and add a line like the following to your ``Doxyfile``:

    .. code:: ini

        TAGFILES += stl.tag=https://en.cppreference.com/w/

`Intersphinx inventory file`_
=============================

If you use the Python API and want to crosslink to that as well, the
`Intersphinx <https://www.sphinx-doc.org/en/master/usage/extensions/intersphinx.html>`_
inventory file is in its usual location:

-   https://doc.magnum.graphics/python/objects.inv (:filesize:`{static}/../external/magnum.inv`)

Either point Sphinx directly to https://doc.magnum.graphics/python/ or download
the file and hook it up manually

.. note-success::

    In case you are wondering, the online Magnum docs are generated using the
    `m.css Doxygen theme <https://mcss.mosra.cz/documentation/doxygen/>`_
    (and the Python API using the `m.css Python doc generator <https://mcss.mosra.cz/documentation/python/>`_).
    It's also possible to conveniently link to Doxygen / Sphinx docs from your
    Pelican blog using the `m.dox <https://mcss.mosra.cz/plugins/links/#doxygen-documentation>`_ and `m.sphinx <https://mcss.mosra.cz/plugins/sphinx/>`_
    m.css plugins.

