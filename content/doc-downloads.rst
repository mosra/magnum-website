Doc Downloads
#############

:summary: Downloads of Corrade and Magnum documentation

Up-to-date documentation for Corrade and Magnum is actively maintained on
http://doc.magnum.graphics/corrade/ and http://doc.magnum.graphics/magnum/.
While it's possible to scrape these for offline viewing, it's advised to build
your own version locally instead. Guide how to do that is available
:dox:`in the documentation <building-doc>`. Thank you for being nice to server
bandwidth.

`Doxygen tag files`_
====================

If you have a project using Magnum and want to cross-link to Magnum
documentation from your Doxygen docs, you can make use of the following
`tag files <http://www.stack.nl/~dimitri/doxygen/manual/external.html>`_:

-   `corrade.tag <http://doc.magnum.graphics/corrade.tag>`_ (:filesize:`{filename}/doc/corrade.tag`)
-   `magnum.tag <http://doc.magnum.graphics/magnum.tag>`_ (:filesize:`{filename}/doc/magnum.tag`)

Download the files into your project and reference them from your ``Doxyfile``
like shown below. If Doxygen complains about duplicate symbols, use just one of
the files.

.. code:: ini

    TAGFILES += corrade.xml=http://doc.magnum.graphics/corrade/ \
                magnum.xml=http://doc.magnum.graphics/magnum/

.. block-info:: Linking to STL documentation

    Corrade and Magnum docs also cross-link to STL documentation at
    `cppreference.com <https://en.cppreference.com>`_. To do that in your
    project as well, download `their tag file <http://en.cppreference.com/w/Cppreference:Archives>`_
    and add a line like the following to your ``Doxyfile``:

    .. code:: ini

        TAGFILES += stl.xml=http://en.cppreference.com/w/

.. note-success::

    In case you are wondering, the online Magnum docs are generated using the
    `m.css Doxygen theme <http://mcss.mosra.cz/doxygen/>`_. It's also possible
    to conveniently link to Doxygen docs from your Pelican blog using
    `a m.css plugin <http://mcss.mosra.cz/plugins/links/#doxygen-documentation>`_.
