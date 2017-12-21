import os
import sys
sys.path.append(os.curdir)
from pelicanconf import *

# Needs to have the scheme, otherwise feeds and social meta tags are broken.
# All other URLs starting with // will be prepended with the scheme from here.
SITEURL = 'http://magnum.graphics'
M_BLOG_URL = '//blog.magnum.graphics'

STATIC_URL = '//static.magnum.graphics/{path}'
PAGE_URL = '//magnum.graphics/{slug}/'
ARCHIVES_URL = '//blog.magnum.graphics/'
ARTICLE_URL = '//blog.magnum.graphics/{category}/{slug}/'
DRAFT_URL = '//blog.magnum.graphics/draft/{slug}/'
AUTHOR_URL = '//blog.magnum.graphics/author/{slug}/'
CATEGORY_URL = '//blog.magnum.graphics/{slug}/'
TAG_URL = '//blog.magnum.graphics/tag/{slug}/'

# htaccess is global for the whole domain
# doc/index.html redirects from doc.magnum.graphics to doc.magnum.graphics/magnum
STATIC_PATHS += ['doc/index.html', 'htaccess']
EXTRA_PATH_METADATA = {'doc/index.html': {'path': '../doc/index.html'},
                       'htaccess': {'path': '../.htaccess'}}

# URL needs to be prepended by / because Pelican removes the first / for some
# reason which makes the second URL look like relative to server root instead
# of having a domain as well :/
PAGINATION_PATTERNS = [(1, '/{url}', '{base_name}/index.html'),
                       (2, '/{url}{number}/', '{base_name}/{number}/index.html')]

M_LINKS_NAVBAR2[1] = ('Blog', M_BLOG_URL, 'archives', [])
M_LINKS_FOOTER3[1] = ('Blog feed', M_BLOG_URL + '/feeds/all.atom.xml')

M_CSS_FILES = ['https://fonts.googleapis.com/css?family=Source+Code+Pro:400,400i,600%7CSource+Sans+Pro:400,400i,600,600i&amp;subset=latin-ext',
            STATIC_URL.format(path='m-dark.compiled.css')]

OUTPUT_PATH = 'published/'

FEED_ALL_ATOM = 'blog/feeds/all.atom.xml'
FEED_ALL_ATOM_URL = '//blog.magnum.graphics/feeds/all.atom.xml'
CATEGORY_FEED_ATOM = 'blog/feeds/%s.atom.xml'
CATEGORY_FEED_ATOM_URL = '//blog.magnum.graphics/feeds/%s.atom.xml'

FEED_MAX_ITEMS = 10

DELETE_OUTPUT_DIRECTORY = True
