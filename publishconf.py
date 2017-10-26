import os
import sys
sys.path.append(os.curdir)
from pelicanconf import *

SITEURL = '//magnum.graphics'
BLOGURL = '//blog.magnum.graphics'

STATIC_URL = '//static.magnum.graphics/{path}'
PAGE_URL = '//magnum.graphics/{slug}/'
ARCHIVES_URL = '//blog.magnum.graphics/'
ARTICLE_URL = '//blog.magnum.graphics/{category}/{slug}/'
DRAFT_URL = '//blog.magnum.graphics/draft/{slug}/'
AUTHOR_URL = '//blog.magnum.graphics/author/{slug}/'
CATEGORY_URL = '//blog.magnum.graphics/{slug}/'
TAG_URL = '//blog.magnum.graphics/tag/{slug}/'

# Not sure why the slash needs to be THRICE?!
PAGINATION_PATTERNS = [(1, '///blog.magnum.graphics/{base_name}/', '{base_name}/index.html'),
                       (2, '///blog.magnum.graphics/{base_name}/{number}/', '{base_name}/{number}/index.html')]

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

CSS_FILES = ['https://fonts.googleapis.com/css?family=Source+Code+Pro:400,400i,600%7CSource+Sans+Pro:400,400i,600&amp;subset=latin-ext',
            STATIC_URL.format(path='inc/m-dark.css'),
            STATIC_URL.format(path='inc/pygments-dark.css')]

OUTPUT_PATH = 'published/'

FEED_ALL_ATOM = 'blog/feeds/all.atom.xml'
FEED_ALL_ATOM_URL = '//blog.magnum.graphics/feeds/all.atom.xml'
CATEGORY_FEED_ATOM = 'blog/feeds/%s.atom.xml'
CATEGORY_FEED_ATOM_URL = '//blog.magnum.graphics/feeds/%s.atom.xml'

DELETE_OUTPUT_DIRECTORY = True

# It's broken (base_name is /blog/) so I'm extending it beyond
DEFAULT_PAGINATION = 15
