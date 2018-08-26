Introducing Guest Posts
#######################

:date: 2018-01-22
:category: Meta
:cover: {filename}/img/blog/introducing-guest-posts.jpg
:tags: m.css, WebGL, WebVR
:summary: One of the goals while building the new Magnum website was to lower
    the barrier for contributing content. With Git and GitHub it's already very
    easy to contribute code to the project itself, so why not extend that to
    the website as well?

.. role:: sh(code)
    :language: sh

Guest posts were kicked off with the
`Area Lights post by Jonathan Hale <{filename}/blog/guest-posts/area-lights-with-ltcs.rst>`_
last week and there are already more articles being prepared for the next
rounds. Guest posts are about sharing success stories, presenting research,
showing productivity tricks or, for example, showing a deeper insight into a
feature that was just recently integrated into Magnum.

We believe that publishing your content here can be beneficial for many sides
--- for you, as it has the potential to reach people outside of your usual
circles, for the community, as fresh and extraordinary things always spark new
interest, and of course, for Magnum itself. Maybe you are working on something
cool and don't have an established place to show it to the world, or maybe you
just finished a paper about a new rendering technique and need a place to
present a WebGL demo? We're set up for that.

Interesting times ahead.

Being this open to new content would be very hard to achieve with a centralized
CMS, as potential contributors would either need access rights or would have to
submit a post with no possibility to check the output before it gets published.
Here, however, you can just build a copy of the whole site on your machine and
iterate on the content locally.

`Writing a guest post`_
=======================

So, how does one write a guest blog post? First of all, please
`get in touch <{filename}/contact.rst>`_ so we can make a mutual agreement on
the topic, scope and timeframe for when it gets published. The topic should
involve Magnum in some way, obviously.

The content is written in reStructuredText and can be submitted via a
pull-request to the :gh:`mosra/magnum-website` repository on GitHub. If you
don't know :abbr:`reST <reStructuredText>` from Sphinx or other documentation
systems, the m.css docs have a
`basic introduction to writing reST markup <http://mcss.mosra.cz/pelican/writing-content/>`_.
Or you can have a look at source files in the repository. For example, the
whole original content of the Area Lights post is contained in the following
commit: :gh:`mosra/magnum-website@fd3ef282258cf224f61cd83d1e0c1247dcb137ff`

The best way for writing a post is setting up a local copy of the website for
immediate visual feedback. It's not strictly required, though --- you can also
contribute the blog post copy simply as a plain text, for example, if that's
a better alternative for you.

`The boring technical details`_
-------------------------------

In order to set up a local copy of the website you need Python 3 with
`Pelican <https://getpelican.com/>`_, `Pyphen <http://pyphen.org/>`_ and
`Pillow <https://pypi.python.org/pypi/Pillow>`_ packages installed and
*optionally* some LaTeX distribution for math rendering.

On ArchLinux it's the following packages:

.. code:: sh

    sudo pacman -S texlive-most pelican python-pillow
    cower -d python-pyphen # Build the python-pyphen package from AUR

On Ubuntu you need these:

.. code:: sh

    sudo apt install texlive-base texlive-latex-extra texlive-fonts-extra python3-matplotlib graphviz
    pip3 install pelican Pyphen Pillow

On Windows and macOS pick some LaTeX distribution and make it available in the
``PATH``. Python packages can be installed via ``pip3`` as well:

.. code:: sh

    pip3 install pelican Pyphen Pillow

.. note-success::

    Setting up LaTeX might be problematic and so the website doesn't strictly
    require it to make life easier for you --- if LaTeX is not found, it will
    render math formulas as unformatted code instead.

Next, make sure to clone the repository (or your fork of the repository)
including submodules so `m.css <http://mcss.mosra.cz>`_ is properly included
(use :sh:`git clone --recursive`, for example).

That's it. Now you can build the website and start a webserver on
``http://localhost:8000`` with the following command. The website is
automatically rebuilt on changes in the input files.

.. code:: sh

    cd magnum-website
    make devserver

.. note-info::

    If you are on Windows and you don't have Git symlinks enabled, you either
    need to reinstall Git with symlinks enabled (there's a checkbox for that
    in the installation wizard) or copy ``m.css/css/*.css`` to ``output/inc/*``,
    otherwise the built website will lack all styling. Sorry for the
    inconvenience.

`By the way...`_
================

The last weeks were quite busy with updates --- in case you didn't see yet,
there is a bunch of new WebGL examples and demos on the `Showcase <{filename}/showcase.rst>`_
page:

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: ../../showcase-figures.rst.in
            :start-after: [bullet]
            :end-before: [/bullet]

    .. container:: m-col-m-6

        .. include:: ../../showcase-figures.rst.in
            :start-after: [picking]
            :end-before: [/picking]

.. container:: m-row m-container-inflate

    .. container:: m-col-m-6

        .. include:: ../../showcase-figures.rst.in
            :start-after: [webvr]
            :end-before: [/webvr]

    .. container:: m-col-m-6

        .. include:: ../../showcase-figures.rst.in
            :start-after: [ui-gallery]
            :end-before: [/ui-gallery]

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/955487950929907712>`_
