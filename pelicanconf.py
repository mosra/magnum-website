import re
import shutil
import logging

AUTHOR = 'Vladimír Vondruš'

#M_SITE_LOGO = 'gr/logo.png'
M_SITE_LOGO_TEXT = 'Magnum'

SITENAME = 'Magnum Engine'
SITESUBTITLE = 'C++11/C++14 and OpenGL graphics engine'
SITEURL = ''

M_BLOG_NAME = 'Magnum Engine Blog'
M_BLOG_URL = 'blog/'

PATH = 'content'

STATIC_URL = 'static/{path}'
STATIC_SAVE_AS = 'static/{path}'
STATIC_PATHS = ['img', 'showcase', 'packages']
EXTRA_PATH_METADATA = {
    'img/favicon.ico': {'path': '../favicon.ico'},
    'packages/deb.sh': {'path': '../packages/deb.sh'}
}

ARTICLE_PATHS = ['blog']
ARTICLE_EXCLUDES = ['blog/authors', 'blog/categories', 'blog/tags']

PAGE_PATHS = ['']
PAGE_EXCLUDES = ['doc', 'img']
READERS = {'html': None} # HTML files are only ever included from reST

PAGE_URL = '{slug}/'
PAGE_SAVE_AS = '{slug}/index.html'

ARCHIVES_URL = 'blog/'
ARCHIVES_SAVE_AS = 'blog/index.html'
ARTICLE_URL = 'blog/{slug}/' # category/ is part of the slug
ARTICLE_SAVE_AS = 'blog/{slug}/index.html'
DRAFT_URL = 'blog/{slug}/' # so the URL is the final one
DRAFT_SAVE_AS = 'blog/{slug}/index.html'
AUTHOR_URL = 'blog/author/{slug}/'
AUTHOR_SAVE_AS = 'blog/author/{slug}/index.html'
CATEGORY_URL = 'blog/{slug}/'
CATEGORY_SAVE_AS = 'blog/{slug}/index.html'
TAG_URL = 'blog/tag/{slug}/'
TAG_SAVE_AS = 'blog/tag/{slug}/index.html'

AUTHORS_SAVE_AS = None # Not used
CATEGORIES_SAVE_AS = None # Not used
TAGS_SAVE_AS = None # Not used

PAGINATION_PATTERNS = [(1, '{base_name}/', '{base_name}/index.html'),
                       (2, '{base_name}/{number}/', '{base_name}/{number}/index.html')]

TIMEZONE = 'Europe/Prague'

DEFAULT_LANG = 'en'

import platform
if platform.system() == 'Windows':
    DATE_FORMATS = {'en': ('usa', '%b %d, %Y')}
else:
    DATE_FORMATS = {'en': ('en_US.UTF-8', '%b %d, %Y')}

FEED_ATOM = None
FEED_ALL_ATOM = None
CATEGORY_FEED_ATOM = None
TRANSLATION_FEED_ATOM = None
AUTHOR_FEED_ATOM = None
AUTHOR_FEED_RSS = None

M_LINKS_NAVBAR1 = [('Features', 'features/', 'features', [
                        ('Extra Functionality', 'features/extras/', 'features/extras'),
                        ('Plugins & Extensions', 'features/extensions/', 'features/extensions'),
                        ('Community Contributions', 'features/community/', 'features/community')]),
                   ('Showcase', 'showcase/', 'showcase', []),
                   ('Docs', '//doc.magnum.graphics/', '', [
                        ('Getting Started', '//doc.magnum.graphics/magnum/getting-started.html', ''),
                        ('Corrade', '//doc.magnum.graphics/corrade/', ''),
                        ('Magnum', '//doc.magnum.graphics/magnum/', ''),
                        ('Python API', '//doc.magnum.graphics/python/', ''),
                        ('Doc Downloads', 'doc-downloads/', 'doc-downloads')])]

M_LINKS_NAVBAR2 = [('Premium Services', 'premium/', 'premium', []),
                   ('Blog', M_BLOG_URL, '[blog]', []),
                   ('GitHub', 'https://github.com/mosra/magnum', '', [
                        ('Contact', 'contact/', 'contact'),
                        ('About', 'about/', 'about')])]

M_LINKS_FOOTER1 = [('Magnum', '/'),
                   ('Features', 'features/'),
                   ('Extra Functionality', 'features/extras/'),
                   ('Plugins & Extensions', 'features/extensions/'),
                   ('Community Contributions', 'features/community/'),
                   ('Showcase', 'showcase/'),
                   ('Premium Services', 'premium/'),
                   ('“Is Magnum what I’m looking for?”', '/is-magnum-what-i-am-looking-for/')]

M_LINKS_FOOTER2 = [('Docs', '//doc.magnum.graphics/'),
                   ('Getting Started', '//doc.magnum.graphics/magnum/getting-started.html'),
                   ('Corrade', '//doc.magnum.graphics/corrade/'),
                   ('Magnum', '//doc.magnum.graphics/magnum/'),
                   ('Python API', '//doc.magnum.graphics/python/'),
                   ('Doc Downloads', 'doc-downloads/'),
                   ('Packages', 'packages/')]

M_LINKS_FOOTER3 = [('Contact Us', 'contact/'),
                   ('Blog Feed', M_BLOG_URL + '/feeds/all.atom.xml'),
                   ('GitHub', 'https://github.com/mosra/magnum'),
                   ('Gitter', 'https://gitter.im/mosra/magnum'),
                   ('Twitter', 'https://twitter.com/czmosra'),
                   ('Google Groups', 'https://groups.google.com/forum/#!forum/magnum-engine'),
                   ('About the Project', 'about/'),
                   ('Build Status', 'build-status/')]

M_CSS_FILES = ['https://fonts.googleapis.com/css?family=Source+Code+Pro:400,400i,600%7CSource+Sans+Pro:400,400i,600,600i&subset=latin-ext',
               'static/m-dark.css']

M_FINE_PRINT = """
| Magnum Engine. Copyright © `Vladimír Vondruš <http://mosra.cz>`_ and
  contributors, 2010--2022. Site powered by `Pelican <https://getpelican.com>`_
  and `m.css <https://mcss.mosra.cz>`_.
| Site content is `available on GitHub under MIT <https://github.com/mosra/magnum-website>`_.
  Contact the team via `GitHub <https://github.com/mosra/magnum>`_,
  `Gitter <https://gitter.im/mosra/magnum>`_, `e-mail <mailto:info@magnum.graphics>`_,
  or `Twitter <https://twitter.com/czmosra>`_.
"""

M_ARCHIVED_ARTICLE_BADGE = """
.. note-warning:: Archived article

    This article is from {year}. While great care is taken to keep information
    up-to-date, please note that not everything in this article might reflect
    current state of the Magnum project, external links might be dead and
    content might be preserved in its original form for archival purposes. Even
    the typos.
"""

M_NEWS_ON_INDEX = ("Latest news on our blog", 3)

DEFAULT_PAGINATION = 10

PLUGIN_PATHS = ['m.css/plugins']
PLUGINS = ['m.abbr',
           'm.alias',
           'm.code',
           'm.components',
           'm.dot',
           'm.dox',
           'm.filesize',
           'm.gh',
           'm.gl',
           'm.htmlsanity',
           'm.images',
           'm.link',
           'm.math',
           'm.metadata',
           'm.plots',
           'm.sphinx',
           'm.vk']

FORMATTED_FIELDS = ['summary', 'description', 'landing', 'badge', 'header', 'footer']

THEME = 'm.css/pelican-theme/'
THEME_STATIC_DIR = 'static/'

M_THEME_COLOR = '#22272e'
M_SOCIAL_TWITTER_SITE = '@czmosra'
M_SOCIAL_TWITTER_SITE_ID = 1537427036
M_SOCIAL_IMAGE = 'static/img/site.jpg'
M_SHOW_AUTHOR_LIST = True

M_HTMLSANITY_SMART_QUOTES = True
M_HTMLSANITY_HYPHENATION = True
M_DOX_TAGFILES = [
    ('external/stl.tag', 'https://en.cppreference.com/w/', [], ['m-flat']),
    ('content/doc/corrade.tag', 'https://doc.magnum.graphics/corrade/', ['Corrade::'], ['m-flat', 'm-text', 'm-strong']),
    ('content/doc/magnum.tag', 'https://doc.magnum.graphics/magnum/', ['Magnum::'], ['m-flat', 'm-text', 'm-strong'])]
M_SPHINX_INVENTORIES = [
    ('external/python.inv', 'https://docs.python.org/3/', [], ['m-flat']),
    ('external/numpy.inv', 'https://docs.scipy.org/doc/numpy/', [], ["m-flat"]),
    ('external/scipy.inv', 'https://docs.scipy.org/doc/scipy/reference/', [], ["m-flat"]),
    # It's in external instead of content/doc because there's already
    # https://doc.magnum.graphics/python/objects.inv that people can download
    ('external/magnum.inv', 'https://doc.magnum.graphics/python/', ['corrade.', 'magnum.'], ['m-flat', 'm-text', 'm-strong'])
]
M_IMAGES_REQUIRE_ALT_TEXT = True
M_METADATA_AUTHOR_PATH = 'blog/authors'
M_METADATA_CATEGORY_PATH = 'blog/categories'
M_METADATA_TAG_PATH = 'blog/tags'

_magnum_colors_src = re.compile(r"""<span class="mh">0x(?P<hex>[0-9a-f]{6})(?P<alpha>[0-9a-f]{2})?(?P<literal>_s?rgba?f?)</span>""")
_magnum_colors_dst = r"""<span class="mh">0x\g<hex>\g<alpha>\g<literal><span class="m-code-color" style="background-color: #\g<hex>;"></span></span>"""

# Code wrapped in MCSS_ELLIPSIS() will get replaced by an (Unicode) ellipsis in
# the output; code wrapped in MCSS_IGNORE() will get replaced by nothing. In
# order to make the same code compilable, add
#
#   #define MCSS_ELLIPSIS(...) __VA_ARGS__
#   #define MCSS_IGNORE(...) __VA_ARGS__
#
# to the snippet code. Equivalent to what's in magnum/doc/conf.py, except for
# a different name.
def _mcss_ignore(code: str):
    for macro, replace in [('MCSS_ELLIPSIS(', '…'), ('MCSS_IGNORE(', '')]:
        while macro in code:
            i = code.index(macro)
            depth = 1
            for j in range(i + len(macro), len(code)):
                if code[j] == '(': depth += 1
                elif code[j] == ')': depth -= 1
                if depth == 0: break
            assert depth == 0, "unmatched %s) parentheses in %s" % (macro, code)
            code = code[:i] + replace + code[j+1:]
    return code

M_CODE_FILTERS_PRE = {
    'C++': _mcss_ignore
}

M_CODE_FILTERS_POST = {
    'C++': lambda str: _magnum_colors_src.sub(_magnum_colors_dst, str)
}

if not shutil.which('latex'):
    logging.warning("LaTeX not found, fallback to rendering math as code")
    M_MATH_RENDER_AS_CODE = True

DIRECT_TEMPLATES = ['archives']
PAGINATED_TEMPLATES = {'archives': None, 'tag': None, 'category': None, 'author': None}

SLUGIFY_SOURCE = 'basename'
PATH_METADATA = '(blog/)?(?P<slug>.+).rst'
SLUG_REGEX_SUBSTITUTIONS = [
        (r'[^\w\s-]', ''),  # remove non-alphabetical/whitespace/'-' chars
        (r'(?u)\A\s*', ''),  # strip leading whitespace
        (r'(?u)\s*\Z', ''),  # strip trailing whitespace
        (r'[-\s]+', '-'),  # reduce multiple whitespace or '-' to single '-'
        (r'C\+\+', 'cpp'),
    ]
