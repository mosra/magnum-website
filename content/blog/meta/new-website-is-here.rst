New Magnum website is here --- a stepping stone for what is to come
###################################################################

:date: 2018-01-03
:modified: 2018-11-04
:category: Meta
:cover: {static}/img/blog/new-website-is-here.jpg
:tags: Doxygen, C++, m.css
:summary: The new website does a better job at clearly presenting engine
    features and enables the team to share progress easier than before. Besides
    that, its development resulted in many valuable byproducts for both C++
    developers and web content publishers.

.. note-success:: Content care: Nov 04, 2018

    Updated status of PRs submitted to third-party projects.

One of greatest strengths that developed in Magnum over time is its modularity
and flexibility. Until now the capabilities were not really well communicated,
leading to false impression. The new `Feature Overview pages <{filename}/features.rst>`_
with interactive architecture diagrams are designed to show modularity and
extensibility as a core principle and the focus is now on improving and
documenting the flexibility even further.

The `Build Status <{filename}/build-status.rst>`_ page primary shows
per-project, per-platform build status in a more detailed way than the binary
"everything is awesome" / "something's broken" status badges from CI servers.
There's also a new `Documentation Downloads <{filename}/doc-downloads.rst>`_
page that will be gradually filled with downloadable content for integrating
Magnum docs into your projects or IDEs.

`m.css`_
========

You may have noticed that the pages don't take long to load. Great effort was
put into the overall website performance and responsiveness.

.. note-info::

    As a graphics developer I would be feeling *really bad* for presenting a
    designed-to-be-fast graphics engine with a sluggish website ;)

The website is built using `m.css <https://mcss.mosra.cz>`_, a no-nonsense,
no-JavaScript CSS framework. It was originally developed mainly to power this
website, but thanks to its simplicity and lightweight approach it has already
been adopted by numerous people since its release in late October 2017. Besides
CSS style, m.css provides a theme for `Pelican <https://docs.getpelican.com/en/stable/>`_
static site generator and a bunch of useful plugins for generating nicely
highlighted code blocks, rendering math equations, and more.

`Doxygen theme`_
================

The builtin `Doxygen <https://doxygen.org>`_ HTML output didn't really serve my
expectations anymore and I needed something to match the website theme. That
led to creating a `m.css Doxygen theme <https://mcss.mosra.cz/doxygen/>`_,
which uses XML output from Doxygen to produce a modern and responsive HTML5
markup.

You can see the Doxygen theme live at https://doc.magnum.graphics. Please note
that the theme is still a work-in-progress and some features (such as symbol
search) are yet to be implemented. The documentation content is gradually
proofread, updated, and improved to match the new theme.

`Benefits for projects outside of Magnum`_
==========================================

-   `m.css`_, now with over 400 commits, extensive documentation, continuous
    integration and high test coverage, is production-ready. Definitely check
    it out if you are looking for something to publish your technical blog
    with, present a project or write documentation in.
-   `8 patches integrated to Doxygen <https://github.com/doxygen/doxygen/pulls?utf8=✓&q=is%3Apr+author%3Amosra+>`_,
    mostly related to the XML output, majority of them already included in
    version 1.8.14 that was released in December 2017.
-   `9 patches submitted to Pelican <https://github.com/getpelican/pelican/pulls?utf8=✓&q=is%3Apr+author%3Amosra+>`_,
    improving support for sites spanning multiple subdomains. Most of these are
    scheduled to be in the upcoming version 3.8.
-   `2 patches to latex2svg <https://github.com/tuxu/latex2svg/pulls?utf8=✓&q=is%3Apr+author%3Amosra>`_
    that handles `math rendering in m.css <https://mcss.mosra.cz/admire/math>`_.
-   I am maintaining a `friendly fork of Pygments <https://github.com/mosra/pygments>`_
    with patches for improved C++ and GLSL syntax highlighting that are not
    integrated in any stable version yet.

`Open content`_
===============

All content on this site is coming from a :abbr:`reST <reStructuredText>`
source that's processed to HTML using Pelican and then uploaded to Apache
that's just serving static content. As straightforward as it can get. The full
website content is publicly available on GitHub at :gh:`mosra/magnum-website`.
Even :gh:`this very article <mosra/magnum-website$master/content/blog/meta/new-website-is-here.rst>`,
see how it looks in its raw form:

.. include:: new-website-is-here.rst
    :code: rst
    :end-line: 20

`Support & consulting`_
=======================

If you are looking to use Magnum in a bigger project and need consulting,
training or dedicated support to ensure everything goes smooth, *premium paid
support* is available. If you want to help the project, donations of any kind
are very welcome. Thank you in advance for financial support, hardware
donations, bug reports, code contributions or just letting the world know about
Magnum.

Head over to the `Contact & Support <{filename}/contact.rst>`_ page for
contact details.

`What's next`_
==============

There's a lot in the queue for the next weeks and the new website is the first
stepping stone, allowing us to share the progress of everything that comes
after. One of the more important things to be released is a public roadmap for
Magnum, uncovering the feature priorities and future milestones. The open
website content also lowers the barrier for contributions, enabling people to,
for example, publish guest posts on the blog. But more on that later ;)

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/948660232686796801>`_,
    `Reddit <https://www.reddit.com/r/cpp/comments/7nxo0k/new_website_for_the_magnum_c11c14_graphics_engine/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=16065321>`_,
    `mailing list <https://groups.google.com/forum/#!topic/magnum-engine/qRXQ_PA7hJQ>`_
