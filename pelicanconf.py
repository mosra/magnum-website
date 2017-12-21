AUTHOR = 'Vladimír Vondruš'

#M_SITE_LOGO = 'gr/logo.png'
M_SITE_LOGO_TEXT = 'Magnum'

SITENAME = 'Magnum Engine'
SITESUBTITLE = 'C++11/C++14 and OpenGL graphics engine'
SITEURL = ''

M_BLOG_NAME = 'Magnum Engine Blog'
M_BLOG_URL = 'blog/'

PATH = 'content'
ARTICLE_PATHS = ['blog']
PAGE_PATHS = ['pages']

STATIC_URL = 'static/{path}'
STATIC_SAVE_AS = 'static/{path}'

PAGE_URL = '{slug}/'
PAGE_SAVE_AS = '{slug}/index.html'

ARCHIVES_URL = 'blog/'
ARCHIVES_SAVE_AS = 'blog/index.html'
ARTICLE_URL = 'blog/{category}/{slug}/'
ARTICLE_SAVE_AS = 'blog/{category}/{slug}/index.html'
DRAFT_URL = 'blog/draft/{slug}/'
DRAFT_SAVE_AS = 'blog/draft/{slug}/index.html'
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
DATE_FORMATS = {'en': ('en_US', '%b %d %Y')}

FEED_ATOM = None
FEED_ALL_ATOM = None
CATEGORY_FEED_ATOM = None
TRANSLATION_FEED_ATOM = None
AUTHOR_FEED_ATOM = None
AUTHOR_FEED_RSS = None

M_LINKS_NAVBAR1 = [('Features', 'features/', 'features', []),
                   ('Showcase', 'showcase/', 'showcase', []),
                   ('Corrade', 'corrade/', 'corrade', [])]

M_LINKS_NAVBAR2 = [('Docs', '//doc.magnum.graphics/', '', [
                        ('Getting started', '//doc.magnum.graphics/magnum/getting-started.html', ''),
                        ('Corrade', '//doc.magnum.graphics/corrade/', ''),
                        ('Magnum', '//doc.magnum.graphics/magnum/', '')]),
                   ('Blog', M_BLOG_URL, '[blog]', [])]

M_LINKS_FOOTER1 = [('Magnum', '/'),
                   ('Features', 'features/'),
                   ('Showcase', 'showcase/'),
                   ('Corrade', 'corrade/')]

M_LINKS_FOOTER2 = [('Docs', '//doc.magnum.graphics/'),
                   ('Getting started', '//doc.magnum.graphics/magnum/getting-started.html'),
                   ('Corrade', '//doc.magnum.graphics/corrade/'),
                   ('Magnum', '//doc.magnum.graphics/magnum/')]

M_LINKS_FOOTER3 = [('Connect', None),
                   ('Blog feed', M_BLOG_URL + '/feeds/all.atom.xml'),
                   ('GitHub', 'https://github.com/mosra/magnum'),
                   ('Gitter', 'https://gitter.im/mosra/magnum'),
                   ('Google Groups', 'https://groups.google.com/forum/#!forum/magnum-engine')]

M_CSS_FILES = ['https://fonts.googleapis.com/css?family=Source+Code+Pro:400,400i,600%7CSource+Sans+Pro:400,400i,600,600i&amp;subset=latin-ext',
               STATIC_URL.format(path='m-dark.css')]

M_FINE_PRINT = """
Magnum Engine. Copyright © Vladimír Vondruš 2010-2017. Site powered by
`Pelican <https://getpelican.com>`_ and `m.css <http://mcss.mosra.cz>`_.
Contact the author via `e-mail <mosra@centrum.cz>`_,
:abbr:`Jabber <mosra@jabbim.cz>`, `Twitter <https://twitter.com/czmosra>`_ or
smoke signals.
"""

DEFAULT_PAGINATION = 10

STATIC_PATHS = ['img', 'showcase']

PLUGIN_PATHS = ['m.css/pelican-plugins']
PLUGINS = ['m.abbr',
           'm.code',
           'm.components',
           'm.dox',
           'm.gh',
           'm.gl',
           'm.htmlsanity',
           'm.images']

THEME = 'm.css/pelican-theme/'
THEME_STATIC_DIR = 'static/'

M_THEME_COLOR = '#22272e'

M_HTMLSANITY_SMART_QUOTES = True
M_HTMLSANITY_HYPHENATION = True
M_DOX_TAGFILES = [
    ('doxygen/corrade.tag', 'http://doc.magnum.graphics/corrade/', ['Corrade::']),
    ('doxygen/magnum.tag', 'http://doc.magnum.graphics/magnum/', ['Magnum::'])]
M_IMAGES_REQUIRE_ALT_TEXT = True

DIRECT_TEMPLATES = ['archives']
PAGINATED_DIRECT_TEMPLATES = ['archives']

SLUGIFY_SOURCE = 'basename'
SLUG_SUBSTITUTIONS = [('C++', 'cpp')]
