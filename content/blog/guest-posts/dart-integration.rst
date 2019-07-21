Using DART to control a robotic manipulator
###########################################

:date: 2019-07-20
:category: Guest Posts
:cover: {static}/img/blog/guest-posts/dart-example.png
:tags: C++, DART, Physics, Robotics, Magnum, DartIntegration
:author: Konstantinos Chatzilygeroudis
:summary: A Magnum example showcasing the capabilities of the DART integration.

.. role:: cpp(code)
    :language: c++
    :class: highlight

`What is a robotic manipulator?`_
=================================

A robotic mechanism is constructed by connecting rigid bodies, called links, together by means of joints,
so that relative motion between adjacent links becomes possible. Actuation of the joints, typically by
electric motors, then causes the robot to move and exert forces in desired ways. Additionally, there exist
several sensors that allows us to read the current position, velocity or torque of an actuator.

A robotic manipulator is a robotic mechanism where its links are arranged in serial fashion, that is, they
form what we call an open-chain. Without going very deeply into the mechanical details behind the actual
machines and without loss of generality, for the remaining of this post a robotic manipulator can be described
fully by the mass and inertia properties of its links, its kinematic configuration, its joint positions and its
joint velocities. Moreover, we will assume that our manipulators can be control either via torque or velocity
commands.

`What is this example about?`_
===============================

Before moving on, let us see what we can do in this example (and what we will explain in this post):

.. raw:: html

    <center><video src="{static}/img/blog/guest-posts/dart-example/dart_magnum.mp4" width="768" height="432" controls></video></center>

As we can see in the video, our example world consists of a robotic manipulator with a gripper
attached in its end-effector (that is, the last link in the serial chain) and three small colored boxes. The user can control
the manipulator with intuitive commands (e.g., move over the red box or move up/down) and pick up the small boxes to create
some towers. They can even destroy the ones that they created!

We will split the post into two main directions: (a) explaining how to use the DartIntegration (and some basic usage of DART),
and (b) explaining the basics of the controller developed for this task (from a robotics perspective).

`Using the DartIntegration`_
============================

`Dynamic Animation and Robotics Toolkit (DART) <http://dartsim.github.io/>`_ is an open-source library that provides
data structures and algorithms for kinematic and dynamic applications in robotics and computer animation. Since its main focus
is robotics applications, every entity in a DART world is a kinematic skeleton, that is, a collection of links interconnected
with joints. Each joint can be actuated with different types of actuation (force or torque, velocity, position) or it can be fixed.
DART additionally support non-rigid bodies/links, but we will only consider robots with rigid body links in this post.
In DART everything begins by creating an empty world:

.. code:: c++

    auto world = dart::simulation::WorldPtr(new dart::simulation::World);

Inside this imaginary world, our robots will live and flourish. The next step is to create our robots. In DART, even the small boxes
that have no actuation are considered as a kinematic skeleton. Let us create our fist "robot" box (for more details on how to use DART,
please `follow their tutorials <http://dartsim.github.io/tutorials_introduction.html>`_):

.. code:: c++

    /* Give a name to our box */
    std::string name = "box";
    /* Give a color to our box */
    Eigen::Vector3d color(1., 0., 0.);
    /* The size of our box */
    const double box_size = 0.06;

    /* Calculate the mass of the box */
    const double box_density = 260; // kg/m^3
    const double box_mass = box_density * std::pow(box_size, 3.);
    /* Create a Skeleton with the given name */
    dart::dynamics::SkeletonPtr box = dart::dynamics::Skeleton::create(name);

    /* Create a body for the box */
    dart::dynamics::BodyNodePtr body =
        box->createJointAndBodyNodePair<dart::dynamics::FreeJoint>(nullptr).second;

    /* Create a shape for the box */
    std::shared_ptr<dart::dynamics::BoxShape> box_shape(
        new dart::dynamics::BoxShape(Eigen::Vector3d(box_size,
                                        box_size,
                                        box_size)));
    auto shapeNode = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box_shape);
    shapeNode->getVisualAspect()->setColor(color);

    /* Set up inertia for the box */
    dart::dynamics::Inertia inertia;
    inertia.setMass(box_mass);
    inertia.setMoment(box_shape->computeInertia(box_mass));
    body->setInertia(inertia);

    /* Setup the center of the box properly */
    box->getDof("Joint_pos_z")->setPosition(box_size / 2.0);

The are several steps that we need to take in order to create a robot (in DART but in general also). Imagine if we have to create a robot with
multiple links and joints! For that reason, several conventions have been defined and used in the robotics community. The most popular ones are
the `Denavit–Hartenberg parameters <https://en.wikipedia.org/wiki/Denavit%E2%80%93Hartenberg_parameters>`_ and the product of exponentials formula
(that is based on the screw theory; for more information please refer to the `Modern Robotics book <http://hades.mech.northwestern.edu/images/7/7f/MR.pdf>`_
by Kevin M. Lynch and Frank C. Park).

`Universal Robot Description Format`_
--------------------------------------

Nevertheless, these are still mathematical formulas and they require quite some work to manually perform all the computations. DART (and most robotic simulators)
already have some of the two conventions implemented and require from the user to give only the relative transformation from each link to their
"parent" joints and from the joints to their "child" links. This greatly simplifies the life of the user, but still writing this in C++ code is
demanding and error prone (just have a look at the code above to create a simple box). For this reason, several description formats have arised
over the years that try to make it easier and more intuitive to create your robots.

One of the most popular formats is the Universal Robot Description Format (URDF) that is part of the `Robotics Operating System (ROS) <https://www.ros.org/>`_
framework. URDF is an XML format for representing a robot model; for more details, please have a look at the `ROS wiki <http://wiki.ros.org/urdf/XML>`_ .
Here is how a part of the URDF model of the manipulator that we are going to use looks like:

.. code:: xml

    <link name="iiwa_link_0">
    <inertial>
      <origin rpy="0 0 0" xyz="-0.1 0 0.07"/>
      <mass value="5"/>
      <inertia ixx="0.05" ixy="0" ixz="0" iyy="0.06" iyz="0" izz="0.03"/>
    </inertial>
    <visual>
      <origin rpy="0 0 0" xyz="0 0 0"/>
      <geometry>
        <mesh filename="package://iiwa_description/meshes/iiwa14/visual/link_0.stl"/>
      </geometry>
      <material name="Grey"/>
    </visual>
    <collision>
      <origin rpy="0 0 0" xyz="-0.01 0 0.07875"/>
      <geometry>
        <box size="0.24 0.23 0.1575"/>
      </geometry>
      <material name="Grey"/>
    </collision>
    <self_collision_checking>
      <origin rpy="0 0 0" xyz="0 0 0"/>
      <geometry>
        <capsule length="0.25" radius="0.15"/>
      </geometry>
    </self_collision_checking>
  </link>
  <!-- joint between link_0 and link_1 -->
  <joint name="iiwa_joint_1" type="revolute">
    <parent link="iiwa_link_0"/>
    <child link="iiwa_link_1"/>
    <origin rpy="0 0 0" xyz="0 0 0.1575"/>
    <axis xyz="0 0 1"/>
    <limit effort="320" lower="-2.96705972839" upper="2.96705972839" velocity="1.4835298642"/>
    <safety_controller k_position="100" k_velocity="2" soft_lower_limit="-2.81870674197" soft_upper_limit="2.81870674197"/>
    <dynamics damping="0.5"/>
  </joint>
  <link name="iiwa_link_1">
    <inertial>
      <origin rpy="0 0 0" xyz="0 -0.03 0.12"/>
      <mass value="4"/>
      <inertia ixx="0.1" ixy="0" ixz="0" iyy="0.09" iyz="0" izz="0.02"/>
    </inertial>
    <visual>
      <origin rpy="0 0 0" xyz="0 0 0"/>
      <geometry>
        <mesh filename="package://iiwa_description/meshes/iiwa14/visual/link_1.stl"/>
      </geometry>
      <material name="Orange"/>
    </visual>
    <collision>
      <origin rpy="0 0 0" xyz="0 -0.02 0.13"/>
      <geometry>
        <box size="0.175 0.19 0.26"/>
      </geometry>
      <material name="Orange"/>
    </collision>
  </link>

This is to get an idea of how a URDF file would look like; no need to understand it!

`Loading a URDF model with DART`_
---------------------------------

Assuming that we have in our possession the URDF model of our robot, here is how to load it
with DART:

.. code:: c++

    /* DART: Load Skeletons/Robots */
    DartLoader loader;
    /* Add packages (needed for URDF loading) */
    /* This is a ROS-related thing in order to find the resources (e.g., meshes) */
    /* see the "package://" in the URDF file above */
    loader.addPackageDirectory("name_of_package", "path/to/package");
    std::string filename = "path/to/URDF/file";

    /* Load the URDF in a DART Skeleton */
    auto manipulator = loader.parseSkeleton(filename);

The `KUKA LBR Iiwa manipulator <https://www.kuka.com/en-ch/products/robotics-systems/industrial-robots/lbr-iiwa>`_ URDF that we used is a
modified version of the one in the `iiwa_ros <https://github.com/epfl-lasa/iiwa_ros>`_ package.
The `Robotiq gripper 2F-85 <https://robotiq.com/products/2f85-140-adaptive-robot-gripper?ref=nav_product_new_button>`_ URDF that we used is
a modified version of the one in the `robotiq_arg85_description <https://github.com/a-price/robotiq_arg85_description>`_
package. Once we have loaded/created all our robots, we add them to the DART world:

.. code:: c++

    /* Add the robot/objects in our DART world */
    world->addSkeleton(manipulator);
    world->addSkeleton(floorSkel);
    world->addSkeleton(redBoxSkel);
    world->addSkeleton(greenBoxSkel);
    world->addSkeleton(blueBoxSkel);

`How does DART connects to Magnum?`_
-------------------------------------

DART connects to Magnum throught the :dox:`SceneGraph` part of Magnum (similar to Bullet Integration).
DartIntegration provides two main classes: (a) :dox:`DartIntegration::World`, and (b) :dox:`DartIntegration::Object`.
The most common usage will be something like the following:

.. code:: c++

    /* DART world */
    auto dart_world = dart::simulation::WorldPtr(new dart::simulation::World);
    /// Add robots and objects into DART world

    /* Create our DARTIntegration object/world */
    auto dartObj = new Object3D{&_scene};
    auto world = std::unique_ptr<Magnum::DartIntegration::World>(new DartIntegration::World(*dartObj, *dart_world));

    /* Simulate with time step of 0.001 seconds */
    world.world().setTimeStep(0.001);

    for(UnsignedInt i = 0; i < simulationSteps; ++i) {
        world.step();

        /* Update graphics at ~60Hz (15*0.001 ~= 60Hz) */
        if(i % 15 == 0) {
            world.refresh();

            /* Get unused/deleted shapes */
            std::vector<std::unique_ptr<DartIntegration::Object>>& unusedObjects = world.unusedObjects();

            /* The user is expected to handle unused objects. One possible handling
            would be to remove them from the parent scene. */
            deleteObjectsFromScene(unusedObjects);

            /* Get updated shapes -- ones that either the materials or the meshes
            have changed */
            std::vector<std::reference_wrapper<DartIntegration::Object>> updatedObjects = world.updatedShapeObjects();

            updateMeshesAndMaterials(updatedObjects);

            /* Clear list of updated objects */
            world.clearUpdatedShapeObjects();
        }
    }

In essence, the user needs to handle the deleted objects and the ones that need to be updated.
In our example, nothing should be deleted and thus we do not handle the deleted objects.
In order to draw the meshes of our robots, we need to create a structure. We will assume a PhongShader
and create the following class:

.. code:: c++

    class DrawableObject: public Object3D, SceneGraph::Drawable3D {
        public:
            explicit DrawableObject(const std::vector<std::reference_wrapper<GL::Mesh>>& meshes, const std::vector<MaterialData>& materials, Object3D* parent, SceneGraph::DrawableGroup3D* group);

            DrawableObject& setMeshes(const std::vector<std::reference_wrapper<GL::Mesh>>& meshes);
            DrawableObject& setMaterials(const std::vector<MaterialData>& materials);
            DrawableObject& setSoftBodies(const std::vector<bool>& softBody);
            DrawableObject& setTextures(std::vector<Containers::Optional<std::reference_wrapper<GL::Texture2D>>>& textures);

        private:
            void draw(const Matrix4& transformationMatrix, SceneGraph::Camera3D& camera) override;

            std::vector<std::reference_wrapper<GL::Mesh>> _meshes;
            Resource<Shaders::Phong> _color_shader;
            Resource<Shaders::Phong> _texture_shader;
            std::vector<MaterialData> _materials;
            std::vector<bool> _isSoftBody;
            std::vector<Containers::Optional<std::reference_wrapper<GL::Texture2D>>> _textures;
    };

Note that each :dox:`DartIntegration::Object` can contain multiple meshes with color or texture material.
To keep track of which objects are being updated (this should only happen if the visual properties, that
is the mesh or material information, of a body changes), we have defined an unordered_map:

.. code:: c++

    std::unordered_map<DartIntegration::Object*, DrawableObject*> _drawableObjects;

To update the information of our objects (both the transformations but also the graphics part), we perform the following:

.. code:: c++

    world->refresh();

To get and update the new meshes and material information, we perform something like the following:

.. code:: c++

    /* For each update object */
    for(DartIntegration::Object& object : world->updatedShapeObjects()) {
        /* Get material information */
        std::vector<MaterialData> materials;
        std::vector<std::reference_wrapper<GL::Mesh>> meshes;
        std::vector<bool> isSoftBody;
        std::vector<Containers::Optional<std::reference_wrapper<GL::Texture2D>>> textures;

        for (size_t i = 0; i < object.drawData().meshes.size(); i++) {
            bool isColor = true;

            if (object.drawData().materials[i].flags() & Trade::PhongMaterialData::Flag::DiffuseTexture) {
                std::reference_wrapper<GL::Texture2D> texture = *object.drawData().textures[object.drawData().materials[i].diffuseTexture()];
                textures.push_back(texture);
                isColor = false;
            } else
                textures.push_back({});

            MaterialData mat;
            mat._ambientColor = object.drawData().materials[i].ambientColor().rgb();
            if (isColor)
                mat._diffuseColor = object.drawData().materials[i].diffuseColor().rgb();
            mat._specularColor = object.drawData().materials[i].specularColor().rgb();
            mat._shininess = object.drawData().materials[i].shininess();
            mat._scaling = object.drawData().scaling;

            /* Get the modified mesh */
            GL::Mesh& mesh = object.drawData().meshes[i];

            meshes.push_back(mesh);
            materials.push_back(mat);
            if (object.shapeNode()->getShape()->getType() == dart::dynamics::SoftMeshShape::getStaticType())
                isSoftBody.push_back(true);
            else
                isSoftBody.push_back(false);
        }

        /* Check if we already have it */
        auto it = _drawableObjects.insert(std::make_pair(&object, nullptr));
        if (it.second) {
            /* If not, create a new object and add it to our drawables list */
            auto drawableObj = new DrawableObject(meshes, materials, static_cast<Object3D*>(&(object.object())), &_drawables);
            drawableObj->setSoftBodies(isSoftBody);
            drawableObj->setTextures(textures);
            it.first->second = drawableObj;
        } else {
            /* Otherwise, update the mesh and the material data */
            it.first->second->setMeshes(meshes).setMaterials(materials).setSoftBodies(isSoftBody).setTextures(textures);
        }
    }

    world->clearUpdatedShapeObjects();

The :dox:`DartIntegration::Object` stores in memory the mesh, material and texture information and thus it is advised
to keep references or pointers to these values.

Putting everything together and with a few additional things (e.g., two lights), we can get something like the following:

.. figure:: {static}/img/blog/guest-posts/dart-example/dart.png
    :alt: DartIntegration rendering
    :figclass: m-flat

    Our example scene in DART rendered in Magnum through the DartIntegration

`Controlling a robotic manipulator`_
=====================================

The example scene looks (relatively) nice, but up until now we cannot control what happens to our imaginary world.
But most of the fun comes when we can control our robots and make them do things. In this second part of the post,
we will see how we can control our manipulator in order to be able to give it intuitive commands to perform some
everyday tasks.

As we have already discussed, our robot's state can be described by its joint positions and its joint velocities.
In the robotics literature, we find the notation :math:`q` for joint positions and :math:`\dot{q}`
for joint velocities.

`Actuator types`_
------------------

Robots are mechanical structures that are interconnected and controlled via electrical components. This means that
the lowest level control signal happens in current. Because handling current is difficult and non-intuitive, there
are two types of motors widely used to provide a higher level of abstraction:

- Torque-controlled actuators; these actuators are equipped with a force/torque sensor and the higher level control signals are the desired torques/forces (depending on the type of the mechanical connection).
- Servo actuators; these actuators are equipped with an encoder sensor (that measures the displacement or current position of the joint) and the higher level control signals are the desired joint positions.

`Controlling in joint space`_
------------------------------

The easiest type of control is to control the robot in what we call the joint-space. This basically means that we have
some desired :math:`q^*` joint positions and some desired :math:`\dot{q}^*` joint velocities (usually set to zero) and we
want to find the control signal to achieve them.

When controlling in joint-space, the most classical controller is a `Proportional–Integral–Derivative (PID) controller <https://en.wikipedia.org/wiki/PID_controller>`_.
In essence, the desired control signal (no matter the type) is computed as follows:

.. math ::

    \tau = K_p (q^*-q) + K_i \int_0^t (q^*-q)dt + K_d\frac{d(q^*-q)}{dt}

The user here will need to tune the parameters :math:`K_p,K_i,K_d`.

In robotics, a variation of this control is widely used (usually named PD-controller):

.. math ::

    \tau = K_p (q^*-q) + K_d (\dot{q}^*-\dot{q})
 
Using these simple controllers, one can do some very amazing and interesting things.

`Controlling in task space`_
-----------------------------

However, most of the interesting things happen in the Cartesian 6D space. This is the space where the end-effector
of our manipulator lives in. Moreover, this is the space where most of the real world tasks can be very intuitively
described. For this reason, we call it the task-space.

`Jacobian`_
^^^^^^^^^^^^^

Jacobian matrices are very useful, and heavily used in robotics engineering and research. Basically, a
Jacobian defines the dynamic relationship between two different representations of a system. Formally,
the Jacobian matrix is the first-order partial derivatives of a function with respect to some variables.
Let's take as an example our robotic manipulator. As we have already, we can describe its configuration
by the joint positions :math:`q`. This configuration also gives a 6D position (that is, translation and
orientation) for the end-effector, let's call it :math:`x(q)`. This relationship is known as the **forward
kinematics** and is usually non-linear. The Jacobian matrix (which is a function), :math:`J(q)`,
give us the following relationship:

.. math ::

    \begin{aligned}
      J(q) &= \frac{\partial{x(q)}}{\partial{q}}\\
      &= \frac{\partial{x(q)}}{\partial{t}}\frac{\partial{t}}{\partial{q}}\Rightarrow\\
      \dot{x} &= J(q)\dot{q}
    \end{aligned}

With this last equation, we have an analytical equation that relates the joint velocities, :math:`\dot{q}`
to end-effector velocities :math:`\dot{x}`. Moreover, this is a linear relationship. Using the property of
conservation of energy (from traditional physics) and some mathematical manipulation, we can generalize this
relationship to forces and torques (we drop the arguments in :math:`J` for clarity):

.. math ::

    \tau = J^TW

where :math:`W` is the wrench (force and torque) applied on the end-effector. Using this equation we can
plan our desired trajectories in the end-effector, write some controller in that space (like a PD controller that we saw previously),
and then transform them to joint-space (the space that we can actually control).

**But how do we get this Jacobian matrix?** We will not go into the details, but once we have the forward
kinematics of our robot (this is either measured or given by the manufacturer), then we just take the
partial derivatives and we get our Jacobian.

`Using DART to perform task-space control`_
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

DART has built-in functions that compute the Jacobian of any link of our robot, and thus controlling our
robots in the task-space is quite easy. Here's what we do (mostly copy-pasted from the example):

.. code:: c++

    /* Get joint velocities of manipulator */
    Eigen::VectorXd dq = _model->getVelocities();

    /* Get full (with orientation) Jacobian of our end-effector */
    Eigen::MatrixXd J = _model->getBodyNode("iiwa_link_ee")->getWorldJacobian();

    /* Get current state of the end-effector */
    Eigen::MatrixXd currentWorldTransformation = _model->getBodyNode("iiwa_link_ee")->getWorldTransform().matrix();
    Eigen::VectorXd currentWorldPosition = currentWorldTransformation.block(0, 3, 3, 1);
    Eigen::MatrixXd currentWorldOrientation = currentWorldTransformation.block(0, 0, 3, 3);
    Eigen::VectorXd currentWorldSpatialVelocity = _model->getBodyNode("iiwa_link_ee")->getSpatialVelocity(dart::dynamics::Frame::World(), dart::dynamics::Frame::World());

    /* Compute desired forces and torques */
    Eigen::VectorXd linearError = _desiredPosition - currentWorldPosition;
    /* PD controller in end-effector position */
    Eigen::VectorXd desiredForces = _pLinearGain * linearError - _dLinearGain * currentWorldSpatialVelocity.tail(3);

    /* Special care needs to be taken to compute orientation differences
     * We use the angle-axis representation */
    Eigen::VectorXd orientationError = dart::math::logMap(_desiredOrientation * currentWorldOrientation.transpose());
    /* PD controller in end-effector orientation */
    Eigen::VectorXd desiredTorques = _pOrientationGain * orientationError - _dOrientationGain * currentWorldSpatialVelocity.head(3);

    /* Combine forces and torques in one vector */
    Eigen::VectorXd tau(6);
    tau.head(3) = desiredTorques;
    tau.tail(3) = desiredForces;

    /* Compute final forces + gravity compensation + regularization */
    forces = J.transpose() * tau + _model->getCoriolisAndGravityForces() - _dRegularization * dq;

    /* Send commands to the robot */
    _model->setCommands(forces);

For making the example, we had to add a few more things, like adding a few boxes, keeping track of their location
and updating the desired end-effector location given the user input. Moreover, we added to our manipulator a gripper
so that it can grasp the boxes. In reality, most commercially available grippers are position controlled (we cannot
send torques as we were describing so far, but we need to give desired positions): to simulate that we use velocity
control for the gripper and what we have been describing so far for the rest of the manipulator. For this reason, we need to keep
a model of our manipulator without the gripper so that DART can gives us all the information we need cleanly and easily.
There is a way to do it without a copy, but involves keeping track of frames and removing specific lines from the
Jacobian, and thus, I chose the copy to be more clear.

`Why DART and not Bullet?`_
============================

`Bullet <https://pybullet.org/wordpress/>`_ is one of the most popular physics engines available and widely used
in the graphics and game industry. Although choosing which physics library to use for a project depends on quite
many parameters and factors, I am going to list a few reasons why I am using DART more actively than Bullet:

- DART is orientated towards computer animation and robotics. This means that it already has implemented a wide range of functionality that is needed for these areas. A few examples consist:
    - Forward/Inverse kinematics,
    - Forward dynamics,
    - Jacobians and their derivatives,
    - Robot dynamics quantities (like mass matrix, coriolis and gravity forces, etc.),
    - Efficient implementation for rigid body chains (although Bullet has recently added this feature),
    - Motion planning.
- Modern C++11/14 code,
- Accurate and stable dynamics for rigid body chains (robotic systems).

Although Bullet is moving towards supporting most of the features that DART already has, to me as a roboticist
I find DART more intuitive and closer to my mind of thinking. Nevertheless, if someone is aiming for big
open worlds (e.g., like in *The Elder Scrolls: Skyrim*) or many objects interacting with each other, DART is not
designed for this type of situations and Bullet will be significantly faster (possibly less accurate though).

Thank you for reading! I'll be back.

.. note-dim::

    Discussion: `Twitter <https://twitter.com/czmosra/status/952961309032767489>`_,
    `Reddit <https://www.reddit.com/r/cpp/comments/7qldic/area_lights_with_ltcs_in_the_magnum_graphics/>`_,
    `Hacker News <https://news.ycombinator.com/item?id=16152101>`_
