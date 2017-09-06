AUTHOR = 'Vladimír Vondruš'

#SITE_LOGO = 'gr/logo.png'
SITE_LOGO_TEXT = 'Magnum'

SITENAME = 'Magnum Engine'
SITEURL = ''

BLOGNAME = 'Magnum Engine Blog'
BLOGURL = '/blog/'

PATH = 'content'
ARTICLE_PATHS = ['blog']
PAGE_PATHS = ['pages']

STATIC_URL = '/static/{path}'
STATIC_SAVE_AS = 'static/{path}'

PAGE_URL = '/{slug}/'
PAGE_SAVE_AS = '{slug}/index.html'

ARCHIVES_URL = '/blog/'
ARCHIVES_SAVE_AS = 'blog/index.html'
ARTICLE_URL = '/blog/{category}/{slug}/'
ARTICLE_SAVE_AS = 'blog/{category}/{slug}/index.html'
DRAFT_URL = '/blog/draft/{slug}/'
DRAFT_SAVE_AS = 'blog/draft/{slug}/index.html'
AUTHOR_URL = '/blog/author/{slug}/'
AUTHOR_SAVE_AS = 'blog/author/{slug}/index.html'
CATEGORY_URL = '/blog/{slug}/'
CATEGORY_SAVE_AS = 'blog/{slug}/index.html'
TAG_URL = '/blog/tag/{slug}/'
TAG_SAVE_AS = 'blog/tag/{slug}/index.html'

AUTHORS_SAVE_AS = None # Not used
CATEGORIES_SAVE_AS = None # Not used
TAGS_SAVE_AS = None # Not used

# Not sure why the slash needs to be twice
PAGINATION_PATTERNS = [(1, '//{base_name}/', '{base_name}/index.html'),
                       (2, '//{base_name}/{number}/', '{base_name}/{number}/index.html')]

TIMEZONE = 'Europe/Prague'

DEFAULT_LANG = 'en'
LOCALE = ('en_US', 'usa')
DATE_FORMATS = {'en': '%b %d %Y'}

FEED_ATOM = None
FEED_ALL_ATOM = None
CATEGORY_FEED_ATOM = None
TRANSLATION_FEED_ATOM = None
AUTHOR_FEED_ATOM = None
AUTHOR_FEED_RSS = None

LINKS_NAVBAR1 = [('Features', SITEURL + '/features/', []),
                 ('Showcase', SITEURL + '/showcase/', []),
                 ('Corrade', SITEURL + '/corrade/', [])]

LINKS_NAVBAR2 = [('Docs', '//doc.magnum.graphics/',
                 [('Corrade', '//doc.magnum.graphics/corrade/'),
                  ('Magnum', '//doc.magnum.graphics/magnum/')]),
                ('Blog', BLOGURL, [])]

LINKS_FOOTER1 = [('Magnum', SITEURL + '/'),
                 ('Features', SITEURL + '/features/'),
                 ('Showcase', SITEURL + '/showcase/'),
                 ('Corrade', SITEURL + '/corrade/')]

LINKS_FOOTER2 = [('Docs', '//doc.magnum.graphics/'),
                 ('Getting Started', '//doc.magnum.graphics/magnum/getting-started.html'),
                 ('Corrade', '//doc.magnum.graphics/corrade/'),
                 ('Magnum', '//doc.magnum.graphics/magnum/')]

LINKS_FOOTER3 = [('Connect', None),
                 ('Blog feed', BLOGURL + '/feeds/all.atom.xml'),
                 ('GitHub', 'https://github.com/mosra/magnum'),
                 ('Gitter', 'https://gitter.im/mosra/magnum'),
                 ('Google Groups', 'https://groups.google.com/forum/#!forum/magnum-engine')]

CSS_FILES = ['https://fonts.googleapis.com/css?family=Source+Code+Pro:400,400i,600|Source+Sans+Pro:400,400i,600&amp;subset=latin-ext',
            STATIC_URL.format(path='inc/m-dark.css'),
            STATIC_URL.format(path='inc/pygments-dark.css')]

FINE_PRINT = """
Magnum Engine. Copyright © Vladimír Vondruš 2010-2017. Site powered by Pelican.
Layout and style based on ``m.css``. Contact the author via
`e-mail <mosra@centrum.cz>`_, :abbr:`Jabber (mosra@jabbim.cz)`,
`Twitter <https://twitter.com/czmosra>`_ or smoke signals.
"""

DEFAULT_PAGINATION = 10

STATIC_PATHS = ['img', 'gr', 'showcase']
EXTRA_PATH_METADATA = {'gr/favicon.ico': {'path', 'favicon.ico'}}

PLUGIN_PATHS = ['m.css/pelican-plugins']
PLUGINS = ['m.gl',
           'm.gh',
           'm.dox',
           'm.htmlsanity',
           'm.components',
           'm.images']

THEME = 'm.css/pelican-theme'
THEME_STATIC_DIR = 'static/'
THEME_COLOR = '#22272e'

HTMLSANITY_SMART_QUOTES = True
HTMLSANITY_HYPHENATION = True

DOXYGEN_TAGFILES = [
    ('doxygen/corrade.tag', 'http://doc.magnum.graphics/corrade/', ['Corrade::']),
    ('doxygen/magnum.tag', 'http://doc.magnum.graphics/magnum/', ['Magnum::'])]

TAG_CLOUD_LEVELS = 5

DIRECT_TEMPLATES = ['index', 'archives']
PAGINATED_DIRECT_TEMPLATES = ['archives']

SLUGIFY_SOURCE = 'basename'
SLUG_SUBSTITUTIONS = [('C++', 'cpp')]
