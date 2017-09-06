Magnum website
##############

Content for http://magnum.graphics/. Proper documentation coming soon.

## Build instructions

Make sure to clone this repository including submodules.

Setup pelican and other dependencies of the theme via pip. As the theme depends
on some unmerged changes to pelican, it should be installed from
[mosra/pelican@mosra-master](https://github.com/mosra/pelican/tree/mosra-master).

~~~bash
pip install git+https://github.com/mosra/pelican.git@mosra-master pyphen Pillow
~~~

In the root of this repository run pelican to build the website:

~~~bash
pelican
~~~

If you are on Windows, you will need to copy `m.css\css\*.css` to `output\inc\*`.
