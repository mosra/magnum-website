import os
import sys
sys.path.append(os.curdir)
from pelicanconf import *

# Needs to have the scheme, otherwise feeds and social meta tags are broken.
# All other URLs starting with // will be prepended with the scheme from here.
SITEURL = 'https://magnum.graphics'
M_BLOG_URL = '//blog.magnum.graphics'

STATIC_URL = '//static.magnum.graphics/{path}'
PAGE_URL = '//magnum.graphics/{slug}/'
ARCHIVES_URL = '//blog.magnum.graphics/'
ARTICLE_URL = '//blog.magnum.graphics/{slug}/'
DRAFT_URL = '//blog.magnum.graphics/{slug}/'
AUTHOR_URL = '//blog.magnum.graphics/author/{slug}/'
CATEGORY_URL = '//blog.magnum.graphics/{slug}/'
TAG_URL = '//blog.magnum.graphics/tag/{slug}/'

# htaccess is global for the whole domain
# doc/index.html redirects from doc.magnum.graphics to doc.magnum.graphics/magnum
STATIC_PATHS += ['doc/index.html',
                 'doc/corrade.tag',
                 'doc/magnum.tag',
                 'doc/robots.txt',
                 'htaccess',
                 'robots.txt',
                 'robots-blog.txt',
                 'robots-static.txt',
                 '../m.css/css/m-dark.doxygen.compiled.css']
EXTRA_PATH_METADATA.update({
    'img/favicon-blog.ico': {'path': '../blog/favicon.ico'},
    'img/favicon-doc.ico': {'path': '../doc/favicon.ico'},
    'img/favicon-static.ico': {'path': '../static/favicon.ico'},
    'doc/index.html': {'path': '../doc/index.html'},
    'doc/corrade.tag': {'path': '../doc/corrade.tag'},
    'doc/magnum.tag': {'path': '../doc/magnum.tag'},
    'doc/robots.txt': {'path': '../doc/robots.txt'},
    'htaccess': {'path': '../.htaccess'},
    'robots.txt': {'path': '../robots.txt'},
    'robots-blog.txt': {'path': '../blog/robots.txt'},
    'robots-static.txt': {'path': '../static/robots.txt'},
    '../m.css/css/m-dark.doxygen.compiled.css': {'path': 'm-dark.doxygen.compiled.css'}})

# URL needs to be prepended by / because Pelican removes the first / for some
# reason which makes the second URL look like relative to server root instead
# of having a domain as well :/
PAGINATION_PATTERNS = [(1, '/{url}', '{base_name}/index.html'),
                       (2, '/{url}{number}/', '{base_name}/{number}/index.html')]

assert M_LINKS_NAVBAR2[1][0] == 'Blog'
M_LINKS_NAVBAR2[1] = ('Blog', M_BLOG_URL, '[blog]', [])

assert M_LINKS_FOOTER3[1][0] == 'Blog Feed'
M_LINKS_FOOTER3[1] = ('Blog Feed', M_BLOG_URL + '/feeds/all.atom.xml')

M_SOCIAL_IMAGE = '//static.magnum.graphics/img/site.jpg'

M_CSS_FILES = ['https://fonts.googleapis.com/css?family=Source+Code+Pro:400,400i,600%7CSource+Sans+Pro:400,400i,600,600i&subset=latin-ext',
               STATIC_URL.format(path='m-dark.compiled.css')]
THEME_STATIC_PATHS = ['static/m-dark.compiled.css']

OUTPUT_PATH = 'published/'

FEED_ALL_ATOM = 'blog/feeds/all.atom.xml'
FEED_ALL_ATOM_URL = '//blog.magnum.graphics/feeds/all.atom.xml'
CATEGORY_FEED_ATOM = 'blog/feeds/{slug}.atom.xml'
CATEGORY_FEED_ATOM_URL = '//blog.magnum.graphics/feeds/{slug}.atom.xml'

FEED_MAX_ITEMS = 10

DELETE_OUTPUT_DIRECTORY = True
