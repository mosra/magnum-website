Magnum website
##############

Content for http://magnum.graphics/.

`Setting up a local copy`_
==========================

In order to set up a local copy (for example
`for writing a guest post <http://blog.magnum.graphics/blog/meta/introducing-guest-posts/>`_),
you need Python 3 with `Pelican <https://getpelican.com/>`_,
`Pyphen <http://pyphen.org/>`_ and `Pillow <https://pypi.python.org/pypi/Pillow>`_
packages installed and *optionally* some LaTeX distribution for math rendering.

On ArchLinux it's the following packages:

.. code:: sh

    sudo pacman -S texlive-most pelican python-pillow python-matplotlib
    cower -d python-pyphen # Build the python-pyphen package from AUR

On Ubuntu you need these:

.. code:: sh

    sudo apt install texlive-base texlive-latex-extra texlive-fonts-extra python3-matplotlib graphviz
    pip3 install pelican Pyphen Pillow

On Windows and macOS pick some LaTeX distribution and make it available in the
``PATH``. Python packages can be installed via ``pip3`` as well:

.. code:: sh

    pip3 install pelican Pyphen Pillow matplotlib

**Note:** Setting up LaTeX might be problematic and so the website doesn't
strictly require it to make life easier for you --- if LaTeX is not found, it
will render math formulas as unformatted code instead.

Next, make sure to clone the repository (or your fork of the repository)
including submodules so `m.css <http://mcss.mosra.cz>`_ is properly included
(use :sh:`git clone --recursive`, for example).

That's it. Now you can build the website and start a webserver on
``http://localhost:8000`` with the following command. The website is
automatically rebuilt on changes in the input files.

.. code:: sh

    cd magnum-website
    make devserver

**Note:** If you are on Windows and you don't have Git symlinks enabled, you
either need to reinstall Git with symlinks enabled (there's a checkbox for that
in the installation wizard) or copy ``m.css/css/*.css`` to ``output/inc/*``,
otherwise the built website will lack all styling. Sorry for the inconvenience.

If you run into problems, please open an issue. The `m.css <http://mcss.mosra.cz>`_
documentation might also help. Thank you.

Pelican workarounds and publishing notes
========================================

.. todo: htacces
.. todo: doc/index.html
