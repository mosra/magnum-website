Improved Doxygen documentation and search
#########################################

:date: 2019-01-03
:tags: C++, Doxygen, m.css, JavaScript, Python
:summary: Whether you are browsing Magnum docs or use Doxygen for your own C++
    documentation, there's always a way to improve your workflow. This article
    presents the most recent additions to the
    `m.css Doxygen theme <https://mcss.mosra.cz/doxygen/>`_.

.. role:: cpp(code)
    :language: c++
.. role:: ini(code)
    :language: ini

.. |lrarrow| replace:: ↔
.. |smile| replace:: 😊

.. TODO: rename the file


A year ago I switched Magnum docs to a theme written entirely from scratch,
adding a `never-before-seen search functionality <{filename}/blog/meta/implementing-a-fast-doxygen-search.rst>`_
a while later. As the saying goes, eating one's own dog food is always a good
way to find ways to improve it, so during the whole year I collected various
enhancement ideas and noted rough corners that need more polishing, not to
mention I received great feedback from all the happy developers that started
using the theme for their own projects or were merely the users of it through
`Magnum documentation <https://doc.magnum.graphics/>`_.

`Showing include information for all the things`_
=================================================

A long-awaited --- and I would say also the most important --- addition is
information about what to :cpp:`#include` to get given symbol. But wait, that's
not all! While stock Doxygen shows include paths only for classes --- which
made sense for classic OOP-heavy codebases --- it certainly doesn't help much
in Magnum, which has a lot of functionality directly inside namespaces. While
I'm trying hard to have the symbol |lrarrow| file mapping as intuitive as
possible, it's not always clear and can be a lot of struggle especially for
newcomers.

So I went one step further and the documentation now shows :cpp:`#include`
information not only for classes, but also for namespaces (in case they are
fully contained in a single header, such as :dox:`Animation::Easing`), for free
functions, typedefs, variables and enums. That makes a big difference
especially in large namespaces such as :dox:`Math` or :dox:`GL`.

.. image:: {static}/img/blog/meta/improved-doxygen-documentation-and-search/includes.png
    :alt: Include information for free namespace members
    :scale: 50%

.. note-warning::

    Implementing this feature uncovered a Doxygen bug, so if you plan to use
    this feature on your codebase, make sure to have a version with
    :gh:`doxygen/doxygen#6722` included (not yet merged into ``master`` at the
    time of writing this article).

`Parsing additional function attributes`_
=========================================

.. container:: m-left-m

    .. image:: {static}/img/blog/meta/improved-doxygen-documentation-and-search/noexcept.png
        :alt: Additional keywords
        :scale: 40%

The theme was already handling extra attributes like :cpp:`delete`\ d and
:cpp:`default`\ ed functions and :cpp:`noexcept` --- which Doxygen is not
recognizing at all, if I remember correctly. To complete this, now also
:cpp:`override`, :cpp:`final` and conditional :cpp:`noexcept` are parsed and
shown. Besides that, :cpp:`final` is recognized also for classes, if you'd ever
need that, and a few bugs related to parsing of those attributes were fixed.

You can see these in action for example in :dox:`Containers::Optional`.

.. container:: m-clearfix-l

    ..

`Support for documenting private virtual functions`_
====================================================

.. container:: m-right-m

    .. image:: {static}/img/blog/meta/improved-doxygen-documentation-and-search/private-virtual.png
        :alt: Documented private virtual functions
        :scale: 40%

The classic article about `Virtuality by Herb Sutter <http://www.gotw.ca/publications/mill18.htm>`_
suggests that a class has never any public :cpp:`virtual` functions, but rather
a non-virtual public interface and all virtual interfaces private. That makes
the interface design much more flexible and you don't run into weird issues
with covariant return types.

Magnum follows this rule since the very beginning in its application classes
(such as :dox:`Platform::Sdl2Application`) and all plugin interfaces like
:dox:`Trade::AbstractImporter`, but until now Doxygen was not really able to
show documented private functions. To work around that, the private virtual
functions used to be shown as :cpp:`protected`, which was misleading. Well, not
anymore!

.. block-success:: Doxygen support

    If you want to use this feature for your codebase, first make sure you have
    a build with :gh:`doxygen/doxygen#6729` included. Then enable
    :ini:`EXTRACT_PRIVATE_VIRTUAL` in your ``Doxyfile`` (it's disabled by
    default). The patch is not yet merged into ``master`` at the time of
    writing this article.

.. container:: m-clearfix-l

    ..

`Improved ordering for search results`_
=======================================

The initial search implementation `as explained in this article <{filename}/blog/meta/implementing-a-fast-doxygen-search.rst>`_
was picking up the results in whatever order the search data had them in.
This was already million times better and faster than the classic Doxygen
search implementation, but later I realized it could be easily improved to
order the results in a more useful way --- in particular preferring classes and
namespaces over functions and tucking away :label-danger:`deprecated` and
:cpp:`delete`\ d functionality, since you're far less likely to need
documentation for these:

.. container:: m-row

    .. container:: m-col-m-6

        .. figure:: {static}/img/blog/meta/improved-doxygen-documentation-and-search/order-before.png
            :alt: OpenSearch integration in Firefox
            :scale: 50%

            Searching for :dox:`GL::Buffer` before

    .. container:: m-col-m-6

        .. figure:: {static}/img/blog/meta/improved-doxygen-documentation-and-search/order-after.png
            :alt: OpenSearch integration in Firefox
            :scale: 50%

            ... and after

Fortunately, due to the way the search is implemented, this was only a matter
of sorting the results while building the search data, it required no complex
algorithm changes on the client side.

`Autocompletion in the search field`_
=====================================

Besides the above, with frequent use it also became apparent that having to
type long parts of symbol names to narrow down the results is ... annoying.
Again, a solution was rather simple to implement, making use of a property of
the `Trie <https://en.wikipedia.org/wiki/Trie>`_ search structure --- it
collects characters until the first child node that has results and then it
offers them for autocompletion.

.. image:: {static}/img/blog/meta/improved-doxygen-documentation-and-search/autocompletion.png
    :alt: Search autocompletion
    :scale: 50%

.. note-success::

    Because the search is `UTF-8-aware <{filename}/blog/meta/implementing-a-fast-doxygen-search.rst#bonus-unicode>`_,
    I had to take an extra step to ensure the autocompletion doesn't have a
    truncated sequence at the end. So I wrote a small UTF-8 validator
    :gh:`in pure JavaScript <mosra/m.css/blob/9c97cf838ed5e08d719cfed08745c213e4b7dd35/doxygen/search.js#L193-L218>`.
    Was a fun side-quest |smile|

`OpenSearch browser integration`_
=================================

I'm a heavy user of browser's search bar and search key shortcuts (for example,
if I write ``cpp vector::emplace``, my browser will search for :dox:`std::vector::emplace()`
directly on `cppreference.com <https://cppreference.com>`_). If you're like me
and want to have in-browser search available also for Doxygen docs, it's now
discoverable through `OpenSearch <https://opensearch.org>`_ on browsers that
support it. So, for example on Firefox, visiting `doc.magnum.graphics <https://doc.magnum.graphics>`_
will offer you this:

.. image:: {static}/img/blog/meta/improved-doxygen-documentation-and-search/opensearch.png
    :alt: OpenSearch integration in Firefox
    :scale: 50%

Chrome supports OpenSearch as well, but the discovery is well-hidden deep in
the settings --- if you are on the documentation site and open the search
engine settings, it will suggest adding a new search engine. Another way that
works in many browsers (also in Vivaldi, for example) is right-clicking the
search field and selecting *Add search engine*.

The underlying capability that enables all this is recognition of
``?q={query}#search`` in GET parameters. Appending it to the documentation URL
will directly open a search popup with results for ``{query}``.

.. note-info::

    OpenSearch also makes it possible to show search results and autocompletion
    directly in the browser search bar (your browser might be already using
    this for Wikipedia, for example). While it would be nice to have, it
    unfortunately needs a server-side implementation of the search. Because
    `doc.magnum.graphics`_ is currently just a minimal file-serving
    installation and I'm not planning to support server-side scripting there
    (and thus having to deal with additional security issues) any time soon, I
    don't have any immediate reason to implement this.

`Copying Markdown links into a clipboard`_
==========================================

This was implemented mainly as a cheat for myself to be more efficient ---
since I spend a lot of time doing tech support on the Magnum
`Gitter channel <https://gitter.im/mosra/magnum>`_ (drop by and say hi!) and
various other places, I often need to share a link to the documentation. Now I
can, pressing :label-warning:`⌘` :label-default:`M` (the usual Meta key on your
PC or Mac) will put a Markdown link to the result directly into the clipboard.

.. image:: {static}/img/blog/meta/improved-doxygen-documentation-and-search/markdown-links.png
    :alt: Copy search result as a Markdown link
    :scale: 50%

`More goodies`_
===============

Among other things there's now a support for C++14 variable templates and
various other improvements, mainly related to UX of the search popup. See the
:gh:`m.css commit history <mosra/m.css/compare/f240b15238bdbb6692057d3666835eef3e99cc9c...a98ccfb1e6a44d49871dc56c81502209b88a05dc>`
if you want to know more.

The theme is always improving, for a hint on what could come next, see for
example :gh:`mosra/m.css#79`. If you want to get involved, there are
:gh:`various issues <mosra/m.css/issues/labels/help wanted>` marked
:label-warning:`help wanted`. I'm always very happy to accept contributions,
bugreports and suggestions for improvement --- in particular, it's very
possible that the new features are broken for corner cases that I didn't think
about or that some things are not working consistently across all browsers.
Comments on that very welcome.

`Try it out`_
=============

If you are a Magnum user, there's a chance you already spotted some of these
improvements on `doc.magnum.graphics`_ as I was gradually pushing them out. If
you are using Doxygen for your C++ docs, give the `m.css Doxygen theme`_ a go
--- I'm sure you'll like the result both as a library developer and as an user
of the documentation |smile|

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/1080876333331304448>`_,
    `Reddit r/cpp <https://www.reddit.com/r/cpp/comments/ac7u4s/improved_doxygen_documentation_and_search/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=18816897>`_,
    `mailing list <https://groups.google.com/d/topic/magnum-engine/0pFUT8pZw70>`_
