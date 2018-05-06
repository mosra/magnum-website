#!/usr/bin/env python

# ./print.py initial.bin --no-subtree-merging --no-lookahead-barriers
# python -m test.test_search initial.bin --color > ~/Code/magnum-website/content/blog/meta/implementing-a-fast-doxygen-search/initial.ansi

# ./print.py merged.bin --no-lookahead-barriers
# python -m test.test_search merged.bin --color --show-merged > ~/Code/magnum-website/content/blog/meta/implementing-a-fast-doxygen-search/merged.ansi

# ./print.py barriers.bin --no-subtree-merging
# python -m test.test_search barriers.bin --color --show-lookahead-barriers > ~/Code/magnum-website/content/blog/meta/implementing-a-fast-doxygen-search/barriers.ansi

# ./print.py map.bin --map --no-prefix-merging
# python -m test.test_search map.bin --color > ~/Code/magnum-website/content/blog/meta/implementing-a-fast-doxygen-search/map.ansi

# ./print.py map-merged.bin --map
# python -m test.test_search map-merged.bin --color > ~/Code/magnum-website/content/blog/meta/implementing-a-fast-doxygen-search/map-merged.ansi

# ./print.py unicode.bin --unicode
# python -m test.test_search unicode.bin --color > ~/Code/magnum-website/content/blog/meta/implementing-a-fast-doxygen-search/unicode.ansi

import argparse

from dox2html5 import Trie, ResultMap, ResultFlag, serialize_search_data

if __name__ == '__main__': # pragma: no cover
    parser = argparse.ArgumentParser()
    parser.add_argument('output', help="where to output")
    parser.add_argument('--no-subtree-merging', help="don't merge search data subtrees", action='store_true')
    parser.add_argument('--no-lookahead-barriers', help="don't insert search lookahead barriers", action='store_true')
    parser.add_argument('--no-prefix-merging', help="don't merge search result prefixes", action='store_true')
    parser.add_argument('--map', help="print the map instead", action='store_true')
    parser.add_argument('--unicode', help="print the unicode trie instead", action='store_true')
    args = parser.parse_args()

    trie = Trie()
    trie.insert("magnum", 0);
    trie.insert("magnum::math", 1, lookahead_barriers=[] if args.no_lookahead_barriers else [6]);
    trie.insert("magnum::math::vector", 2, lookahead_barriers=[] if args.no_lookahead_barriers else [6, 12]);
    trie.insert("magnum::math::vector::min", 3, lookahead_barriers=[] if args.no_lookahead_barriers else [6, 12, 20]);
    trie.insert("magnum::math::range", 4, lookahead_barriers=[] if args.no_lookahead_barriers else [6, 12]);
    trie.insert("magnum::math::range::min", 5, lookahead_barriers=[] if args.no_lookahead_barriers else [6, 12, 19]);
    trie.insert("magnum::math::min", 6, lookahead_barriers=[] if args.no_lookahead_barriers else [6, 12]);
    trie.insert("math", 1);
    trie.insert("math::vector", 2, lookahead_barriers=[] if args.no_lookahead_barriers else [4]);
    trie.insert("math::vector::min", 3, lookahead_barriers=[] if args.no_lookahead_barriers else [4, 12]);
    trie.insert("math::range", 4, lookahead_barriers=[] if args.no_lookahead_barriers else [4]);
    trie.insert("math::range::min", 5, lookahead_barriers=[] if args.no_lookahead_barriers else [4, 11]);
    trie.insert("math::min", 6, lookahead_barriers=[] if args.no_lookahead_barriers else [4]);
    trie.insert("vector", 2);
    trie.insert("vector::min", 3, lookahead_barriers=[] if args.no_lookahead_barriers else [6]);
    trie.insert("range", 4);
    trie.insert("range::min", 5, lookahead_barriers=[] if args.no_lookahead_barriers else [5]);
    trie.insert("min", 3);
    trie.insert("min", 5);
    trie.insert("min", 6);

    if args.unicode:
        trie = Trie()
        trie.insert("hýždě", 13)
        trie.insert("hárá", 42)

    map = ResultMap()
    map.add("Magnum", "namespaceMagnum.html", flags=ResultFlag.NAMESPACE)
    map.add("Magnum::Math", "namespaceMagnum_1_1Math.html", flags=ResultFlag.NAMESPACE)
    map.add("Magnum::Math::Vector", "classMagnum_1_1Math_1_1Vector.html", flags=ResultFlag.CLASS)
    map.add("Magnum::Math::Vector::min()", "classMagnum_1_1Math_1_1Vector.html#af029f9f7810201f0bd8d9580af273bde", flags=ResultFlag.FUNC)
    map.add("Magnum::Math::Range", "classMagnum_1_1Math_1_1Range.html", flags=ResultFlag.CLASS)
    map.add("Magnum::Math::Range::min()", "classMagnum_1_1Math_1_1Range.html#ad4919361a2086212fac96da0221e4dcd", flags=ResultFlag.FUNC)
    map.add("Magnum::Math::min()", "namespaceMagnum_1_1Math.html#ae22ef0cb2a5a5e4c5e626a3df670be21", flags=ResultFlag.FUNC)

    with open(args.output, 'wb') as f:
        f.write(serialize_search_data(Trie() if args.map else trie, map if args.map else ResultMap(), merge_subtrees=not args.no_subtree_merging, merge_prefixes=not args.no_prefix_merging))
