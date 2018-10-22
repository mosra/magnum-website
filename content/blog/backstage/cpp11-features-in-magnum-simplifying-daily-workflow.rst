C++11 features in Magnum: Simplifying daily workflow
####################################################

:date: 2013-09-13 18:57
:modified: 2018-01-03
:archived: True
:category: Backstage
:tags: C++, OpenGL
:summary: While the previous part was focused on C++11 features improving
    performance, here I will describe features which are used just to simplify
    your life by writing more concise code with less mistakes.

.. role:: cpp(code)
    :language: c++

.. block-primary:: Backstage

    This article is part of a series describing the *how* and *why* behind
    Magnum graphics engine. Read also the other parts:

    1.  `C++11 features in Magnum: Better memory management <{filename}/blog/backstage/cpp11-features-in-magnum-better-memory-management.rst>`_
    2.  C++11 features in Magnum: Simplifying daily workflow
    3.  `Static and dynamic polymorphism in Magnum <{filename}/blog/backstage/static-and-dynamic-polymorphism-in-magnum.rst>`_

.. note-success:: Content care: Jan 03, 2018

    Code snippets were updated to match current state of the Magnum API and
    some typos and grammar errors were fixed.

`Variadic templates`_
=====================

Variadic templates are used extensively throughout the engine, but in many
cases you won't even notice. With them the engine is able to catch many
mistakes, which otherwise would be impossible to detect, at compile time.
Consider creating 7-element vector the C++03 way:

.. code:: c++

    Float data[] = { 3.5f, 0.3f, 4.0f, -7.3f, 5.0f, 1.0f -5.0f }; // oops
    Math::Vector<7, Float> vec = Math::Vector<7, Float>::from(data);

This operation is not a simple one-line expression, but it must be written in
two statements. Moreover the function cannot generally check that we fed it
with proper amount of data, as it gets just a :cpp:`Float*` pointer. In Magnum
the same can be written using C++11 variadic constructor like this:

.. code:: c++

    Math::Vector<7, Float> vec(3.5f, 0.3f, 4.0f, -7.3f, 5.0f, 1.0f -5.0f); // error!

Note that unlike with e.g. initializer lists, mentioned in previous article,
the error will be detected at compile time --- we forgot to separate last two
values with a comma, thus passed only six elements to the constructor causing
undefined behavior in the C++03 case.

In other situations, variadic templates might calculate some things
automatically and save repetitive typing, which otherwise leads to accidental
mistakes. Imagine adding buffer with interleaved vertex attributes, specifying
one attribute at a time:

.. code:: c++

    Int offset = 4238;
    Int stride = 36;
    Mesh mesh;
    mesh.addVertexBuffer(vertexBuffer, offset, Shader::Position{}, stride-12)
        .addVertexBuffer(vertexBuffer, offset+12, Shader::Normal{}, stride-12)
        .addVertexBuffer(vertexBuffer, offset+24, Shader::TextureCoords{}, stride-12)
        .addVertexBuffer(vertexBuffer, offset+32, Shader::Weight{}, stride-12);

Each interleaved attribute is specified by starting offset and size of the gap
after it (containing data of other attributes). Note that, due to copy-paste
error, all the attributes have specified the same gap, even though the texture
coordinates are two floats and thus occupy only 8 bytes (not 12), and the
weight is only a single four-byte float. If the whole vertex format is
specified at once using variadic function, the sizes, offsets and strides are
calculated automatically behind the scenes, leaving no room for mistakes:

.. code:: c++

    mesh.addVertexBuffer(vertexBuffer, 4238, Shader::Position{}, Shader::Normal{},
        Shader::TextureCoords{}, Shader::Weight{}, 3);

Lastly, with variadic classes you can merge many similar tasks into one.
Imagine having type-safe resource manager for meshes, textures and sound
buffers:

.. code:: c++

    ResouceManager<Mesh> meshManager;
    ResouceManager<Texture2D> textureManager;
    ResouceManager<Audio::Buffer> soundManager;

    Mesh* mesh;
    meshManager.set("steam-locomotive", mesh);
    Texture* texture;
    textureManager.set("steam-locomotive-diffuse", texture);
    Audio::Buffer* buffer;
    soundManager.set("steam-locomotive-honk", buffer);

    // ...

    meshManager.free();
    textureManager.free();
    soundManager.free();

    // ...

    soundManager.clear();

Variadic resource manager implementation allows you to significantly shorten
the above code:

.. code:: c++

    ResourceManager<Mesh, Texture2D, Audio::Buffer> manager;

    manager.set("steam-locomotive", mesh)
           .set("steam-locomotive-diffuse", texture)
           .set("steam-locomotive-honk", buffer);

    // ...

    manager.free();

    // ...

    manager.clear<Audio::Buffer>();

`Method chaining`_
==================

This is not exactly a C++11 feature (however C++11's rvalue references for
:cpp:`this` can be employed to improve performance in some corner cases), but I
will mention it here, as it allows for some neat tricks. Except for helping you
type less (as shown above), this feature allows you to do instantiation and
configuration in single expression, going nicely along the *"everything is an
expression"* approach:

.. code:: c++

    // Configure debug shape rendering
    DebugTools::set("collision-shapes", DebugTools::ShapeRendererConfiguration()
        .setColor(Color3::fromHSV(25.0f, 0.7f, 0.9f))
        .setPointSize(0.35f));

    // Add more ducks to the scene
    (new Duck(&scene))->translate({0.3f, 0.0f, -0.9f})->rotateY(15.0_degf);
    (new Duck(&scene))->translate({0.4f, 0.0f, -1.5f})->rotateY(-5.0_degf);
    (new Duck(&scene))->translate({0.5f, 0.0f, -1.1f})->rotateY(35.0_degf);

Note that this feature also has its downsides, so use it only when it improves
readability and not the other way. For example, more than one method chain in
a single expression can cause the code to be unreadable and prone to errors.

`Miscellaneous`_
================

`Strongly typed units`_
-----------------------

API inconsistence, where some functions accept radians (STL and OpenGL) and
some degrees (OpenAL) leads to problems with mistaken units. C++11 user-defined
literals, explicit conversion operators and :cpp:`constexpr` allow to solve
this in intuitive way without sacrificing performance. The strong types behave
just like any other numeric type and only conversion from and to the underlying
type needs to be done explicitly. For literals, instead of writing ``f`` suffix
you can just write ``_degf`` or ``_radf`` and it will be converted to the
expected units at compile time. All Magnum functions dealing with angles are
taking only the strongly typed values, passing plain numbers to them results in
compile-time error:

.. code:: c++

    Double sin = Math::sin(45.0_deg);
    //Float cos = Math::cos(1.57f); // error
    object->rotateX(15.0_degf);

Other common units are distance and time units. Magnum doesn't impose any
restriction on distance units, similarly to Blender, so whether :cpp:`1.0f` is
one meter or one furlong depends on you (but you need to be consistent, of
course). Time unit literals will be `part of C++14 <https://en.cppreference.com/w/cpp/chrono/operator%22%22h>`_.

`Usage of SFINAE and type traits`_
----------------------------------

With C++11's type traits and :dox:`std::enable_if` it's possible to design
clean generic API free of any workarounds for ambiguous method and constructor
calls. For example, bitwise operations are enabled only for vectors with
integral underlying type. Vectors and matrices have generic support for
(explicit) conversion from and to external types, thanks to :dox:`std::enable_if`
the actual conversion can be then implemented in separate library without
touching the original implementation.

.. code:: c++

    #include <BulletIntegration/Integration.h>

    // Seamless usage of Magnum and Bullet types
    btVector3 a(1.0f, 3.0f, 4.0f);
    Magnum::Vector3 b(a);
    b *= 5.0f;
    a = btVector3(b);
