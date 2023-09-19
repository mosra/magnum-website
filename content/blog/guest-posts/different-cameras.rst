Freeform camera and Orbiting camera classes for Magnum
######################################################

:date: 2018-09-23
:category: Guest posts
:cover: {filename}/img/blog/guest-posts/uzername-cameras0.png
:tags: C++, Camera, OpenGL, Windows
:author: Ivan P.
:summary: A new Magnum example implements usage of Orbiting camera and 
    freely moving (Freeform) cameras for 3D scene

.. role:: cpp(code)
    :language: c++
    :class: highlight
.. role:: label-default
    :class: m-label m-default
.. role:: label-success
    :class: m-label m-success
.. role:: label-primary
    :class: m-label m-primary
.. role:: label-flat-info
    :class: m-label m-flat m-info

The code is available through the `page on Github <https://github.com/uzername/CameraControlsMosraMagnum>`_ 
and it has been tested on Windows 7 32bit, compiling on Visual Studio and using a July2018 version of Mosra/Magnum.
It shows you how to use different kinds of cameras on your scenegraph. Also it mentions tricks how to use scenegraph.

`Introduction`_
===============

Camera is important for any 3D application. In most 3D applications we have many objects staying around and we want to have a logical way
to manipulate the view, like rotating and moving camera around, but not rotating and moving scene around. 

There are two kinds of cameras: 

Orbiting Camera
    It finds its position on some sort of imagined sphere and rotates around the objects on scene, preserving the point where it looks. It is used for engineering and arts 3D applications, like Blender, Autodesk Inventor, or Autocad Mechanical 2015 (you may look on dxf in 3D, OK)
Freely Moving Camera
    A camera's vector is directed to any point, user moves it with mouse, and locates it with keyboard in 3D space. It is used in FPS games, when you join as observer, OK. Or for fish simulation games.

`The Code`_
===========

This program requires the same invocations to compile it like a scenegraph example

When you refer to an application on Github ( `page on Github <https://github.com/uzername/CameraControlsMosraMagnum>`_ ), you see, that I used several files to separate code.

`Main Code`_
------------
`**BaseCamera** class <https://github.com/uzername/CameraControlsMosraMagnum/blob/master/PlainOfCubes/BaseCamera.h>`_ - implements the Free move camera. It was originally suggested by Konstantinos Chatzilygeroudis (costashatz@gmail.com) in conference.

It is extremely important, so here it comes:

Declarations:

..code:: c++
    #include <cmath>
    /*
    Class for camera for mosra/magnum
    Author: Konstantinos Chatzilygeroudis (costashatz@gmail.com)
    */
    class BaseCamera {
    public:
        explicit BaseCamera(Scene3D& scene);
        Object3D* cameraObject();
        SceneGraph::Camera3D* camera();
        BaseCamera& zoom(Magnum::Float percentage);
        BaseCamera& rotate(Magnum::Float dYaw, Magnum::Float dPitch, Magnum::Float sensitivity = 0.001f);
        // pitch is rotation around horizontal. yaw is rotation around vertical.
        BaseCamera& move(Magnum::Float speed);
        // strafe to left and to right
        BaseCamera& strafe(Magnum::Float speed);
        BaseCamera& upDown(Magnum::Float speed);
    protected:
        // no need to delete these manually, they'll be deleted on scene destruction
        Object3D * _cameraObject;
        SceneGraph::Camera3D* _camera;
        Magnum::Float _aspectRatio, _nearPlane, _farPlane;
        Magnum::Rad _fov;
        Magnum::Vector3 _up, _front;
        Magnum::Float _yaw, _pitch;
    };

Definitions:

..code:: c++
	BaseCamera::BaseCamera(Scene3D& scene)
	{
		using namespace Magnum::Math::Literals;
		_fov = 35.0_degf;
		_aspectRatio = 1.0f;
		_nearPlane = 0.01f;
		_farPlane = 100.f;
		_up = Magnum::Vector3{ 0.f, 1.f, 0.f };
		_yaw = -Magnum::Math::Constants<Magnum::Float>::piHalf();
		_pitch = 0.f;
		_front = (Magnum::Vector3{
			std::cos(_yaw) * std::cos(_pitch), std::sin(_pitch), std::sin(_yaw) * std::cos(_pitch) })
			.normalized();
		/* Every scene needs a camera */
		(*(_cameraObject = new Object3D{ &scene }));
		// TO-DO: I should remove this transformation
		_cameraObject->translate(Magnum::Vector3::zAxis(5.f));
		Magnum::Vector3 cameraPos = _cameraObject->transformation().translation();
		_cameraObject->transformation().lookAt(cameraPos, cameraPos + _front, _up);
		(*(_camera = new Magnum::SceneGraph::Camera3D{ *_cameraObject }))
			.setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
			.setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane))
			.setViewport(GL::defaultFramebuffer.viewport().size());
	}
	Object3D* BaseCamera::cameraObject() { return _cameraObject; }
	SceneGraph::Camera3D* BaseCamera::camera() { return _camera; }
	BaseCamera& BaseCamera::zoom(Magnum::Float percentage)
	{
		Magnum::Float perc = std::max(-1.f, std::min(1.f, percentage));
		_fov += perc * _fov;
		_camera->setProjectionMatrix(
			Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane));
		return *this;
	}
	BaseCamera& BaseCamera::rotate(Magnum::Float dYaw, Magnum::Float dPitch, Magnum::Float sensitivity)
	{
		_pitch += dPitch * sensitivity;
		_pitch = std::max(-Magnum::Math::Constants<Magnum::Float>::piHalf(),
			std::min(Magnum::Math::Constants<Magnum::Float>::piHalf(), _pitch));
		_yaw += dYaw * sensitivity;
		_front = (Magnum::Vector3{
			std::cos(_yaw) * std::cos(_pitch), std::sin(_pitch), std::sin(_yaw) * std::cos(_pitch) })
			.normalized();
		Magnum::Vector3 cameraPos = _cameraObject->transformation().translation();
		_cameraObject->setTransformation(Magnum::Matrix4::lookAt(cameraPos, cameraPos + _front, _up));
		return *this;
	}
	BaseCamera& BaseCamera::move(Magnum::Float speed)
	{
		_cameraObject->translate(speed * _front);
		return *this;
	}
	BaseCamera& BaseCamera::strafe(Magnum::Float speed)
	{
		_cameraObject->translate(speed * Magnum::Math::cross(_front, _up).normalized());
		return *this;
	}
	BaseCamera & BaseCamera::upDown(Magnum::Float speed)
	{
		_cameraObject->translate(speed * _up.normalized());
		return *this;
	}

In code we have two objects: camera, cameraObject which is responsible for rotation and which is attached to scene given in constructor. We keep track the angles: pitch rotation (around horizontal) and yaw rotation (around vertical direction). 
They are stored in radians. We also keep track of _forward vector which points to direction of camera.

When needed we recalculate rotation of camera in rotate() method. In this method we place a limitation to _pitch value from -piHalf() to piHalf(). _yaw value is not limited.
Then we recalculate value of _front vector. Essentially it is calculated as position of point on a sphere of radius 1, and formulas are quite similar to the ones used for converting polar coordinates to cartesian (they are easily found on Wikipedia).
They are similar, but not the same. Here they are:

..math::

    _front = \left(\begin{matrix}
            cos(_yaw) * cos(_pitch), \\
            std::sin(_pitch), \\
            std::sin(_yaw) * std::cos(_pitch)
        \end{matrix}\right)

After that we get a current transformation of camera object. Let's get a look at cryptic statement 

..code:: c++
    _cameraObject->setTransformation(Magnum::Matrix4::lookAt(cameraPos, cameraPos + _front, _up));

By using statement ``cameraPos + _front`` we get a translated ``_front`` vector which now runs from ``cameraPos`` point. After that we calculate a new Transformation matrix, as if the camera sight is directed by ``cameraPos + _front`` vector, and camera is located at ``cameraPos`` point.

Another interesting statement is in ``BaseCamera::strafe(Magnum::Float speed)`` method. It looks like this: ``_cameraObject->translate(speed * Magnum::Math::cross(_front, _up).normalized());``. Here we use that result vector of crossproduct of two vectors (front and up vector) is directed to side and is perpendicular to them. Just what we need for side movement, isn't it?

`**OrbitingCamera** class <https://github.com/uzername/CameraControlsMosraMagnum/blob/master/PlainOfCubes/OrbitingCamera.h>`_ - implements the Orbiting camera. Based on previous class, but I'll post it here too, since it is important as well.

Declarations:
..code:: c++
    // https://www.opengl.org/discussion_boards/showthread.php/198988-Implementing-an-orbit-camera
    // https://en.wikipedia.org/wiki/Spherical_coordinate_system#Cartesian_coordinates
    // https://upload.wikimedia.org/wikipedia/commons/thumb/d/dc/3D_Spherical_2.svg/240px-3D_Spherical_2.svg.png
    class OrbitingCamera
    {
    public:
        explicit OrbitingCamera(Scene3D& scene, float in_distanceToOrigin, Magnum::Vector3 in_originPoint);
        //getter
        Object3D* cameraObject();
        //getter
        SceneGraph::Camera3D* camera();
        OrbitingCamera& zoom(Magnum::Float percentage);
        OrbitingCamera& rotate(Magnum::Float dYaw, Magnum::Float dPitch, Magnum::Float sensitivity = 0.001f);
        // pitch is rotation around horizontal. yaw is rotation around vertical. Combine these to orbit around mesh
        OrbitingCamera& rotateAndTranslateInSphere(Magnum::Float dPhi, Magnum::Float dTheta, Magnum::Float sensitivity = 0.001f);
        OrbitingCamera& move(Magnum::Float speed);
        // strafe to left and to right. Changes origin point
        OrbitingCamera& strafe(Magnum::Float speed);
        // strafe up and down. Changes origin point
        OrbitingCamera& upDown(Magnum::Float speed);
    protected:
        // no need to delete these manually, they'll be deleted on scene destruction
        Object3D * _cameraObject;
        SceneGraph::Camera3D* _camera;
        Magnum::Float _aspectRatio, _nearPlane, _farPlane;
        Magnum::Rad _fov;
        Magnum::Vector3 _up, _front;
        //phi is angle between vertical direction.
        //theta is angle in horizontal plane
        Magnum::Float theta, phi;
        Magnum::Float distanceToOrigin;
        Magnum::Vector3 storedOriginPoint;
    };

Definitions:

..code:: c++
    OrbitingCamera::OrbitingCamera(Scene3D & scene, float in_distanceToOrigin, Magnum::Vector3 in_originPoint) {
        using namespace Magnum::Math::Literals;
        this->distanceToOrigin = in_distanceToOrigin;
        this->storedOriginPoint = in_originPoint;
        _fov = 35.0_degf;
        _aspectRatio = 1.0f;
        _nearPlane = 0.01f;
        _farPlane = 100.f;
        _up = Magnum::Vector3{ 0.f, 1.f, 0.f };
        //rotate around up axis
        phi = Magnum::Constants::piHalf();
        theta = 0.0f;
        float x = distanceToOrigin * sin(phi) * cos(theta);
        float y = distanceToOrigin * cos(phi);
        float z = distanceToOrigin * sin(phi) * sin(theta);
        _front = Vector3(x, y, z).normalized();
        /* Every scene needs a camera */
        (*(_cameraObject = new Object3D{ &scene }));
        _cameraObject->setTransformation(Magnum::Matrix4::lookAt(Magnum::Vector3(x, y, z) + storedOriginPoint, storedOriginPoint, _up));
        (*(_camera = new Magnum::SceneGraph::Camera3D{ *_cameraObject }))
            .setAspectRatioPolicy(Magnum::SceneGraph::AspectRatioPolicy::Extend)
            .setProjectionMatrix(Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane))
            .setViewport(GL::defaultFramebuffer.viewport().size());
    }
    Object3D * OrbitingCamera::cameraObject() {
        return _cameraObject;
    }
    SceneGraph::Camera3D * OrbitingCamera::camera() {
        return _camera;
    }
    OrbitingCamera & OrbitingCamera::zoom(Magnum::Float percentage)
    {
        Magnum::Float perc = std::max(-1.f, std::min(1.f, percentage));
        _fov += perc * _fov;
        _camera->setProjectionMatrix(
            Magnum::Matrix4::perspectiveProjection(_fov, _aspectRatio, _nearPlane, _farPlane));
        return *this;
    }
    OrbitingCamera & OrbitingCamera::rotate(Magnum::Float dYaw, Magnum::Float dPitch, Magnum::Float sensitivity)
    {
        return *this;
    }
    OrbitingCamera & OrbitingCamera::rotateAndTranslateInSphere(Magnum::Float dPhi, Magnum::Float dTheta, Magnum::Float sensitivity)
    {
        phi += dPhi * sensitivity;
        phi = std::max(1e-5f,
            std::min(1.0f*Magnum::Math::Constants<Magnum::Float>::pi(), phi));
        theta += dTheta * sensitivity;
        //theta = std::max(0.0f, std::min(2.0f*Magnum::Math::Constants<Magnum::Float>::pi(), theta));
        /*
        _front = (Magnum::Vector3{
        std::cos(_yaw) * std::cos(_pitch), std::sin(_pitch), std::sin(_yaw) * std::cos(_pitch) })
        .normalized();
        Magnum::Vector3 cameraPos = _cameraObject->transformation().translation();
        */
        float x = distanceToOrigin * sin(phi) * cos(theta);
        float y = distanceToOrigin * cos(phi);
        float z = distanceToOrigin * sin(phi) * sin(theta);
        _front = Vector3(x, y, z).normalized();
        //it performs translation to origin point. Params: eye position, target, up
        _cameraObject->setTransformation(Magnum::Matrix4::lookAt(Magnum::Vector3(x, y, z) + storedOriginPoint, storedOriginPoint, _up));
        return *this;
    }
    OrbitingCamera & OrbitingCamera::move(Magnum::Float speed)
    {
        float currentDeltaDistance = distanceToOrigin;
        if (speed < 0) {
            currentDeltaDistance = distanceToOrigin - (speed*_front).length();
        }
        else {
            currentDeltaDistance = distanceToOrigin + (speed*_front).length();
        }
        distanceToOrigin = currentDeltaDistance;
        _cameraObject->translate(speed * _front);

        return *this;
    }
    //left and right. changes origin point, but does not change vector (vector is the same after parallel translation). distanceToOrigin is no change
    OrbitingCamera & OrbitingCamera::strafe(Magnum::Float speed)
    {
        Vector3 translationVector = speed * Magnum::Math::cross(_front, _up).normalized();
        // translation is simply vector addition
        storedOriginPoint = storedOriginPoint + translationVector;
        _cameraObject->translate(translationVector);
        return *this;
    }
    OrbitingCamera & OrbitingCamera::upDown(Magnum::Float speed)
    {
        Vector3 translationVector = speed * _up.normalized();
        storedOriginPoint = storedOriginPoint + translationVector;
        _cameraObject->translate(translationVector);
        return *this;
    }


`Other Code`_
-------------

`**AllDrawables** class <https://github.com/uzername/CameraControlsMosraMagnum/blob/master/PlainOfCubes/AllDrawables.cpp>`_ - it contains scenegraph definitions and declarations. 
It's so important, so I'll put it here too:

.. code:: c++
    using namespace Magnum;
    using namespace Math::Literals;
    class ColoredDrawable : public Object3D, public SceneGraph::Drawable3D {
    public:
        explicit ColoredDrawable(Object3D& object, Shaders::Phong& shader, GL::Mesh& mesh, const Color4& color, SceneGraph::DrawableGroup3D& group) : Object3D{ &object }, SceneGraph::Drawable3D{ *this, &group }, _shader(shader), _mesh(mesh), _color{ color } {}
    private:
        void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;
        Shaders::Phong& _shader;
        GL::Mesh& _mesh;
        Color4 _color;
    };
    class PlainDrawable : public Object3D, public SceneGraph::Drawable3D {
    public:
        explicit PlainDrawable(Object3D& object, Shaders::Flat3D& shader, GL::Mesh& mesh, const Color4& color, SceneGraph::DrawableGroup3D& group) : Object3D{ &object }, SceneGraph::Drawable3D{ *this, &group }, _shader(shader), _mesh(mesh), _color{ color } {
        }
        void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;
    private:
        Shaders::Flat3D& _shader;
        GL::Mesh& _mesh;
        Color4 _color;
    };
    void ColoredDrawable::draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) {
        _shader
            .setDiffuseColor(_color)
            .setLightPosition(camera.cameraMatrix().transformPoint({ -3.0f, 10.0f, 10.0f }))
            .setTransformationMatrix(transformationMatrix)
            .setNormalMatrix(transformationMatrix.rotation())
            .setProjectionMatrix(camera.projectionMatrix());

        _mesh.draw(_shader);
    }
    void PlainDrawable::draw(const Matrix4 & transformationMatrix, SceneGraph::Camera3D & camera)
    {
        _shader.setColor(_color).setTransformationProjectionMatrix(camera.projectionMatrix()*(transformationMatrix));
        _mesh.draw(_shader);
    }

`**MainFile.cpp** and **MainFile.h** files <https://github.com/uzername/CameraControlsMosraMagnum/blob/master/PlainOfCubes/MainFile.cpp>`_ - it contains definitions, found at generic Magnum scenegraph examples.
This is a Main class of SDL2 application. It has the macros invocation for main program (at the very bottom) and events which handle the user input, as following:

.. code:: c++
        void PrimitivesExample::drawEvent() {
            GL::defaultFramebuffer.clear(GL::FramebufferClear::Color | GL::FramebufferClear::Depth);
			//camera is responsible for drawing everything on scene. Everything on scene which should be rendered is passed to scene_drawables
            scene_camera->draw(*scene_drawables);
            swapBuffers();
        }
        void PrimitivesExample::mousePressEvent(MouseEvent& event) {
            if (event.button() != MouseEvent::Button::Left) return;
            Vector2i eventData = event.position();
            deltaX = abs(eventData.x());
            deltaY = abs(eventData.y());
            event.setAccepted();
        }
        void PrimitivesExample::mouseReleaseEvent(MouseEvent& event) {
            deltaX = 0;
            deltaY = 0;
            event.setAccepted();
            redraw();
        }
        void PrimitivesExample::mouseMoveEvent(MouseMoveEvent& event) {
            if (!(event.buttons() & MouseMoveEvent::Button::Left)) return;
            Vector2i eventData = event.position();
            Vector2i relativePosition = eventData - Vector2i{ deltaX,deltaY };
            int relativePositionX = relativePosition.x();
            int relativePositionY = relativePosition.y();
          #ifdef USEORBITINGCAMERA
            myOrbitingCamera->rotateAndTranslateInSphere(relativePositionY, relativePositionX);
          #else
            greekCamera->rotate(relativePositionX, relativePositionY);
          #endif
            deltaX = eventData.x();
            deltaY = eventData.y();
            event.setAccepted();
            redraw();
        }
        void PrimitivesExample::mouseScrollEvent(MouseScrollEvent & event) {
            Vector2 eventdata = event.offset();
            double mouseScroll = eventdata.y();
            float scalingFactor = 0.15f;
        #ifdef USEORBITINGCAMERA
            myOrbitingCamera->move(1.0f - (mouseScroll > 0 ? 1 / (1.0f - (float)scalingFactor) : (1.0f - (float)scalingFactor)));
        #else
            greekCamera->move(1.0f - (mouseScroll > 0 ? 1 / (1.0f - (float)scalingFactor) : (1.0f - (float)scalingFactor)));
        #endif
            event.setAccepted();
            redraw();
        }
        void PrimitivesExample::keyPressEvent(KeyEvent & event)
        {
            Magnum::Platform::Sdl2Application::KeyEvent::Key in_key = event.key();
            switch (in_key)
            {
            case Magnum::Platform::Sdl2Application::KeyEvent::Key::Up: {
            #ifdef USEORBITINGCAMERA
                myOrbitingCamera->upDown(0.25f);
            #else
                greekCamera->upDown(0.25f);
            #endif
                break;
            }
            case Magnum::Platform::Sdl2Application::KeyEvent::Key::Down: {
            #ifdef USEORBITINGCAMERA
                myOrbitingCamera->upDown(-0.25f);
            #else
                greekCamera->upDown(-0.25f);
            #endif
                break;
            }
            case Magnum::Platform::Sdl2Application::KeyEvent::Key::Left: {
            #ifdef USEORBITINGCAMERA
                myOrbitingCamera->strafe(-0.5f);
            #else
                greekCamera->strafe(-0.5f);
            #endif
                break;
            }
            case Magnum::Platform::Sdl2Application::KeyEvent::Key::Right: {
            #ifdef USEORBITINGCAMERA
                myOrbitingCamera->strafe(0.5f);
            #else
                greekCamera->strafe(0.5f);
            #endif
                break;
            }
            default:
                break;
            }
            event.setAccepted();
            redraw();
        }

The main idea here is the following: we recalculate difference between previous position of mouse cursor (deltaX, deltaY) and current mouse cursor position. 
Then we pass it to camera class. On mouse Scroll Event we move camera in direction of its look vector (to one direction, or to opposite one, depending on the sign of scroll result) with certain speed.

It is important to call such lines at end of event processing:

.. code:: c++
            event.setAccepted();
            redraw();

Here I also have a little scenegraph declarations in activateSceneGraph() subroutine. It basically extends the example of scenegraph usage written by Mosra. I would like to notice here that you should keep 
the global list of all GL::Mesh instances you use and all ColoredDrawable and all PlainDrawable instances you use. Because they should exist all the time during execution of your program.

.. code:: c++
            //decl these in private section
            std::map<std::string, ColoredDrawable*> mirroredSceneGraphWithIDs;
            std::map<std::string, PlainDrawable*> mirroredSceneGraphWireframeWithIDs;
            std::vector<std::unique_ptr< GL::Mesh >> storedMeshes;

And:

.. code:: c++
            Trade::MeshData3D cube2 = Primitives::cubeSolid();
            storedMeshes.push_back(std::unique_ptr<GL::Mesh>(new GL::Mesh(MeshTools::compile(cube2))));
            ColoredDrawable* drawablePtr2 = new ColoredDrawable{ *scene_o, *_coloredShader, *(*(storedMeshes.end()-1)), 0xaaaaffff_rgbaf, *scene_drawables };
            drawablePtr2->translate(Vector3{ 0.5, 0.5, 2.0 });
            this->mirroredSceneGraphWithIDs.insert(std::pair<std::string, ColoredDrawable*>("THING_LOADED2", drawablePtr2));

It's really important.