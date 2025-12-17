Packages
########

:cover: {static}/img/cover-corrade.jpg
:summary: Quick guide for installing Magnum through various packaging systems.

.. role:: console(code)
    :language: shell-session

Quick guide for installing Magnum through various packaging systems.

`ArchLinux`_
============

Packages with the latest Git revision are maintained in the
`AUR <https://aur.archlinux.org>`_. Sorted by dependency order:

-   `corrade-git <https://aur.archlinux.org/packages/corrade-git>`_
-   `magnum-git <https://aur.archlinux.org/packages/magnum-git>`_
-   `magnum-plugins-git <https://aur.archlinux.org/packages/magnum-plugins-git>`_
-   `magnum-extras-git <https://aur.archlinux.org/packages/magnum-extras-git>`_
-   `magnum-integration-git <https://aur.archlinux.org/packages/magnum-integration-git>`_
-   `magnum-bindings-git <https://aur.archlinux.org/packages/magnum-bindings-git>`_
-   `magnum-examples-git <https://aur.archlinux.org/packages/magnum-examples-git>`_

Update by re-running :console:`makepkg` and installing the new package. You can
also install a tagged release or build from within the source itself,
:dox:`further info is in the documentation <building-packages-arch>`.

`Debian, Ubuntu and derivatives`_
=================================

An interactive script building and installing ``*.deb`` packages of the latest
Git revision, contributed by `cat@git.thenight.club <https://git.thenight.club/cat>`_,
is available: `deb.sh <{static}/packages/deb.sh>`_. Alternatively, if you feel
adventurous, you can pipe it straight to shell:

.. code:: shell-session

    bash <(curl https://magnum.graphics/packages/deb.sh)

Internally, the script clones each repository and builds the package from
package sources contained in the repository itself. If you wish to perform
the packaging manually, :dox:`consult the documentation <building-packages-deb>`.

`Homebrew`_
===========

Get the latest Git revision via the :gh:`mosra/homebrew-magnum` tap. Sorted by
dependency order:

.. code:: shell-session

    brew install --HEAD mosra/magnum/corrade
    brew install --HEAD mosra/magnum/magnum
    brew install --HEAD mosra/magnum/magnum-plugins
    brew install --HEAD mosra/magnum/magnum-extras
    brew install --HEAD mosra/magnum/magnum-integration
    brew install --HEAD mosra/magnum/magnum-bindings
    brew install --HEAD mosra/magnum/magnum-examples

To update, replace :console:`install --HEAD` with :console:`upgrade --fetch-HEAD`.
Some packages contain flags for enabling or disabling optional dependencies,
:dox:`see the documentation for more information <building-packages-brew>`.

`Void Linux`_
=============

There is a repository maintained by `cat@git.thenight.club <https://git.thenight.club/cat>`_
that compiles packages for x86_64, i686, AArch64, and ARMv7l. Currently for
glibc only, musl support is planned in the future. The repo can be set up with
these following steps:

.. code:: shell-session
    :class: m-console-wrap

    sudo sh -c "echo \"repository=https://voidpkgs.thenight.club/\" > /usr/share/xbps.d/30-repository-the-void-club.conf"
    sudo xbps-install -S

After setting up the repository the following packages will be available:

-   ``corrade``
-   ``magnum``
-   ``magnum-plugins``
-   ``magnum-extras``
-   ``magnum-integration``
-   ``magnum-examples``
-   ``magnum-all``, which is a meta-package for installing all of the above

`Vcpkg`_
========

Build and install the latest Git revision with the following:

.. code:: shell-session
    :class: m-console-wrap

    vcpkg install --head corrade magnum magnum-plugins magnum-extras magnum-integration magnum-bindings magnum-examples

Updating a vcpkg package needs to be done by first doing a
:console:`vcpkg remove` and then doing a :console:`vcpkg install --head` again.
For each package it's possible to install just a subset of the functionality,
:dox:`details are listed in the documentation <building-packages-vcpkg>`.
