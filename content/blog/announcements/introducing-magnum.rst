Introducing Magnum, a multiplatform 2D/3D graphics engine
#########################################################

:date: 2013-08-30 15:54
:category: Announcements
:tags: C++, Corrade, OpenGL
:summary: After nearly three years of semi-public hyperactive development, I
    think it's the time to release this beast into wild. Say hello to Magnum,
    modular graphics engine written in C++11 and OpenGL.

Magnum started as a simple wrapper to simplify vector/matrix operations so I
could learn and play with OpenGL API without writing too much boilerplate code.
Over the time it expanded into actually usable graphics engine. Its goal is to
simplify low-level graphics development and interaction with OpenGL using
recent C++11 features and to abstract away platform-specific issues.

Magnum is currently ported to these platforms:

-   **OpenGL** 2.1 through 4.4, core profile functionality and modern
    extensions
-   **OpenGL ES** 2.0, 3.0 and extensions to match desktop OpenGL functionality
-   **Linux** and embedded Linux (natively using GLX/EGL and Xlib or through
    GLUT or SDL2 toolkit)
-   **Windows** (through GLUT or SDL2 toolkit)
-   **Google Chrome** (through `Native Client <https://developer.chrome.com/native-client>`_,
    both ``newlib`` and ``glibc`` toolchains are supported)

`What it offers you`_
=====================

Use C++11 to simplify common workflow and OpenGL interaction
------------------------------------------------------------

Magnum makes extensive use of C++11. Most of the new features are used in the
internals to make the library more powerful and you can only guess their
presence, but the best features are on every corner to simplify your life.

C++11 list-initialization and compile-time checks allow for easier and safer
structure initialization. With :cpp:`constexpr` you can even do some limited
vector math at compile-time.

.. code:: c++

    Int* data = new Int{2, 5, -1, 10, 0,                          /* using C++03 */
                        3, 53, -60, -27, // oops
                        9, 0, 4, 7, 135};

    Math::Matrix<3, 5, Int> a;
    a.assignFrom(data);

.. code:: c++

    Math::Matrix<3, 5, Int> a({2, 5, -1, 10, 0},                  /* using C++11 */
                            {3, 53, -60, -27, 25},
                            {9, 0, 4, 7, 135});

Variadic function templates greatly simplify repetitive things and avoid
mistakes, however you are not limited to do this at compile-time only. It is
possible to do the equivalent in run-time, but at the cost of more verbose
code.

.. code:: c++

    /* Shader properties using C++03 and pure OpenGL */
    const int SHADER_POSITION = 0; // three-component
    const int SHADER_NORMAL = 1; // three-component
    const int SHADER_TEXCOORDS = 2; // two-component
    const int SHADER_WEIGHT = 3; // one-component

    /* Mesh configuration */
    glEnableVertexAttribArray(SHADER_POSITION);
    glEnableVertexAttribArray(SHADER_NORMAL);
    glEnableVertexAttribArray(SHADER_TEXCOORDS);
    glEnableVertexAttribArray(SHADER_WEIGHT);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    int offset = 4238;
    glVertexAttribPointer(SHADER_POSITION, 3, GL_FLOAT, GL_FALSE, 40, static_cast<GLvoid*>(offset));
    glVertexAttribPointer(SHADER_NORMAL, 3, GL_FLOAT, GL_FALSE, 40, static_cast<GLvoid*>(offset + 12));
    glVertexAttribPointer(SHADER_TEXCOORD, 2, GL_FLOAT, GL_FALSE, 40, static_cast<GLvoid*>(offset + 24));
    glVertexAttribPointer(SHADER_WEIGHT, 2, GL_FLOAT, GL_FALSE, 40, static_cast<GLvoid*>(offset + 32)); // oops

.. code:: c++

    /* Type-safe shader definition using C++11 and Magnum */
    class Shader: public AbstractShaderProgram {
        public:
            typedef Attribute<0, Vector3> Position;
            typedef Attribute<1, Vector2> Normal;
            typedef Attribute<2, Vector3> TextureCoordinates;
            typedef Attribute<3, Float> Weight;

        // ...
    };

    /* Mesh configuration */
    Buffer vertexBuffer;
    Mesh mesh;
    mesh.addVertexBuffer(vertexBuffer, 4238, Shader::Position(), Shader::Normal(),
        Shader::TextureCoordinates(), Shader::Weight(), 3);

Initializer lists and user-defined literals will save you typing and avoid
nasty mistakes with units in unobtrusive way:

.. code:: c++

    Object3D object;                                              /* using C++03 */
    object.translate(Vector3(1.5f, 0.3f, -1.0f))
        .rotate(35.0f); // this function accepts degrees, right?

.. code:: c++

    Object3D object;                                              /* using C++11 */
    object.translate({1.5f, 0.3f, -1.0f})
        .rotate(35.0_degf);


Strongly typed enums and type-safe :dox:`EnumSet <Containers::EnumSet>` class
prevent hard-to-spot errors with improper enum values and enable proper IDE
autocompletion for enumeration values, saving precious time:

.. code:: c++

    /* Using pure OpenGL, the errors are catched at run-time */
    glClear(GL_COLOR|GL_DEPTH); // oops

.. code:: c++

    /* Using C++11 and Magnum, the errors are catched at compile-time */
    framebuffer.clear(FramebufferClear::Color|FramebufferClear::Depth);

Magnum uses RAII principle, has OpenGL state tracking and transparent support
for :glext:`EXT_direct_state_access`. With automatic fallback to core
functionality for unsupported extensions it allows you to just create an object
and call a function on it without any boilerplate code. You don't need to
handle any explicit initialization and finalization, save and restore the
previous state or bother about extension availability:

.. code:: c++

    GLint texture;                                          /* using pure OpenGL */
    glGenTextures(1, &texture);
    GLint previous;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &previous);
    glBindTexture(GL_TEXTURE_2D, texture);

    if(/* ARB_texture_storage supported, faster code path */) {
        glTexStorage2D(GL_TEXTURE_2D, 4, GL_RGBA8, 256, 256);
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexImage2D(GL_TEXTURE_2D, 1, GL_RGBA8, 128, 128, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexImage2D(GL_TEXTURE_2D, 2, GL_RGBA8, 64, 64, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexImage2D(GL_TEXTURE_2D, 3, GL_RGBA8, 32, 32, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    }

    glBindTexture(GL_TEXTURE_2D, previous);

    // ...

    glDeleteTextures(1, &texture);

.. code:: c++

    Texture2D texture;                                           /* using Magnum */
    texture.setStorage(4, TextureFormat::RGBA8, {256, 256});

These features require compiler with good enough support for C++11. Officialy
supported ones are GCC 4.6+ and Clang 3.1+. There is also compatibility branch
with support for GCC 4.4 and 4.5 (and probably Visual Studio 2012, when I get
to test it). Sometimes the missing features are heavily worked around, which
might case some issues, thus this compatibility is not part of the mainline
code.

Modular and extensible scene graph
----------------------------------

On top of core library taking care of math and OpenGL there are various
optional libraries, which you can, but don't have to use. One of them is scene
graph implementation for both 2D and 3D scenes. The scene graph is templated on
transformation implementation, thus you are free to use matrices, dual
quaternions, dual complex numbers or even roll your own transformation
implementation. Objects in scene graph are not in any linear feature hierarchy
and particular features are attached to given object instead, either
dynamically or using multiple inheritace. This approach allows greater
flexibility compared to linear hierarchy and avoids bubble-up effect (like
having function for setting wheel count on base object).

You can learn more about scene graph :dox:`in the documentation <scenegraph>`.

Integration with other software, plugins for data manipulation
--------------------------------------------------------------

Magnum library itself is kept lightweight and without any external dependencies
to make porting and usage in embedded systems easier. However, in real world
usage, you often need the ability to import data in various formats. Magnum has
support for both static and dynamic plugins and contains plugin interface for
importing meshes, images, audio files and for doing format conversions.
Separate :gh:`plugin repository <mosra/magnum-plugins>` contains JPEG, PNG,
TGA, COLLADA and WAV importer plugins.

Magnum has also builtin plugin-based text layouting and rendering library.
Plugin repository contains FreeType font engine support, HarfBuzz text
layouter, raster font support and also ability to convert between font formats.

It is often desirable to use external (math, physics) library. I'm not going to
boast, Magnum's math library is pretty limited in comparison with most other
math libraries. Magnum provides interface for converting from and to external
representation of mathematic structures, which in the end is presented to user
as simple explicit conversion. :gh:`Integration repository <mosra/magnum-integration>`
contains initial integration of Bullet Physics library.

Magnum doesn't contain its own full-featured window and event handling
abstraction library, instead it is able to hook into various multiplatform
toolkits like GLUT or SDL2 and also lightweight platform-specific toolkits such
as Xlib with GLX/EGL or PPAPI.

Extensive documentation and examples
------------------------------------

:dox:`Documentation <>` is essential part of the engine. Each module and class
has introductional chapter and example usage, each OpenGL support class
provides detailed information about related OpenGL calls and extension
dependence. There is also :gh:`example repository <mosra/magnum-examples>`
containing fully documented examples to ease your learning even more. The
documentation also has a thorough :dox:`guide how to start using Magnum <getting-started>`
in your project, providing even ready-to-build :gh:`bootstrap code <mosra/magnum-bootstrap>`.

More features
-------------

There are many more things worth mentioning, you can read through the
`nearly exhaustive feature list <{filename}/pages/features.rst>`_ for more
information. The `project page <{index}>`_ contains also rationale and design
goals.

`What it won't do`_
===================

Magnum is designed for people who love coding and stands upon integration with
external tools. Don't expect any GameMaker-like GUI, visual shaders, builtin
editors or dedicated IDE. Specialized software will *always* be better at that
job than any integrated editor and this way you can use any tool you want.

Magnum tries to be modular, lightweight and doesn't want to put any
restrictions or limitations on the user. There is no engine-specific mesh
format or effect framework, as it is nearly impossible to create a format which
will suit all imaginable use cases.

`Showcase`_
===========

Magnum is currently used in one small game and one bigger, yet unnanounced one
and the functionality is demonstrated in various examples. See
`showcase page <{filename}/pages/showcase.rst>`_ for images and live
applications.

.. image:: {filename}/img/push-the-box.png
    :alt: Push The Box

`Where can you get it`_
=======================

Because the library is meant to be used by developers and not end users, it is
distributed purely as source code, available on GitHub. The documentation is
available :dox:`for online viewing <>`, you can also generate it directly from
the source code, see instructions on download page for more information. Be
sure to read also the thorough :dox:`Getting Started Guide <getting-started>`.
