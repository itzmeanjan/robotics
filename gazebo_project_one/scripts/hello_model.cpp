#include <gazebo-9/gazebo/gazebo.hh>
#include <cstdio>

namespace gazebo
{
class HelloModelPlugin : public ModelPlugin
{
public:
    HelloModelPlugin()
    {
        printf("Model is waving at you ;)\n");
    }

    void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf) {}
};

GZ_REGISTER_MODEL_PLUGIN(HelloModelPlugin)

} // namespace gazebo
