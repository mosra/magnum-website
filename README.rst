Magnum website
##############

Content for http://magnum.graphics/. Proper documentation coming soon.

Build instructions
==================

Make sure to clone this repository including submodules.

Setup Pelican and other dependencies of the theme via ``pip``. As the website
depends on some unmerged changes to Pelican, it should be installed from
`mosra/pelican@mosra-master <https://github.com/mosra/pelican/tree/mosra-master>`_.

.. code-block:: sh

    pip install git+https://github.com/mosra/pelican.git@mosra-master pyphen Pillow

Then build the website and start a webserver on `http://localhost:8000` with
the following command. The website is automatically rebuilt on changes in the
input files.

.. code-block:: sh

    make devserver

If you are on Windows and you don't have Git symlinks enabled, you will need to
copy ``m.css/css/*.css`` to ``output/inc/*``.
