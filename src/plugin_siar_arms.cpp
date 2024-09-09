#include <ignition/gazebo/System.hh>
#include <ignition/gazebo/Model.hh>
#include <ignition/gazebo/components/Joint.hh>
#include <ignition/gazebo/components/JointPosition.hh>
#include <ignition/gazebo/components/JointVelocityCmd.hh>
#include <ignition/gazebo/components/Name.hh>
#include <ignition/plugin/Register.hh>
#include <ignition/transport/Node.hh>
#include <ros/ros.h>
#include <std_msgs/Float32.h>
#include <vector>

namespace ignition
{
  namespace gazebo
  {
    class ArmControlPlugin : public System, public ISystemConfigure, public ISystemUpdate
    {
      public:
        ArmControlPlugin() = default;

        void Configure(const Entity &entity, const std::shared_ptr<const sdf::Element> &sdf,
                      EntityComponentManager &ecm, EventManager &eventMgr) override;

        void Update(const UpdateInfo &info, EntityComponentManager &ecm) override;

      private:
        void OnCmdReceived(const std_msgs::Float32::ConstPtr &msg);

        Entity modelEntity_;
        std::vector<Entity> rightArmJoints_;
        std::vector<Entity> leftArmJoints_;
        ignition::transport::Node node_;

        // ROS variables
        std::unique_ptr<ros::NodeHandle> rosNode_;
        ros::Subscriber cmdSubscriber_;

        // Movement command
        float cmdValue_ = 0.0;
    };
  }
}

void ignition::gazebo::ArmControlPlugin::Configure(const Entity &entity,
                                                   const std::shared_ptr<const sdf::Element> &sdf,
                                                   EntityComponentManager &ecm,
                                                   EventManager &)
{
  modelEntity_ = entity;

  // Configuración de los joints para los brazos derecho e izquierdo
  if (sdf->HasElement("rightArmJoints"))
  {
    std::string jointString = sdf->Get<std::string>("rightArmJoints");
    std::stringstream ss(jointString);
    std::string jointName;
    while (ss >> jointName)
    {
      // Iterate through all entities to find the joint with the given name
      ecm.Each<components::Joint, components::Name>(
          [&](const Entity &entity, const components::Joint *, const components::Name *nameComp) -> bool
          {
            if (nameComp->Data() == jointName)
            {
              rightArmJoints_.push_back(entity);
              return false;  // Stop searching after finding the joint
            }
            return true;  // Continue searching
          });
    }
  }

  if (sdf->HasElement("leftArmJoints"))
  {
    std::string jointString = sdf->Get<std::string>("leftArmJoints");
    std::stringstream ss(jointString);
    std::string jointName;
    while (ss >> jointName)
    {
      // Iterate through all entities to find the joint with the given name
      ecm.Each<components::Joint, components::Name>(
          [&](const Entity &entity, const components::Joint *, const components::Name *nameComp) -> bool
          {
            if (nameComp->Data() == jointName)
            {
              leftArmJoints_.push_back(entity);
              return false;  // Stop searching after finding the joint
            }
            return true;  // Continue searching
          });
    }
  }

  // Inicializar ROS
  if (!ros::isInitialized())
  {
    ROS_FATAL_STREAM("A ROS node for Gazebo has not been initialized, unable to load plugin.");
    return;
  }

  rosNode_.reset(new ros::NodeHandle("~"));
  cmdSubscriber_ = rosNode_->subscribe("arm_command", 1, &ArmControlPlugin::OnCmdReceived, this);
}



void ignition::gazebo::ArmControlPlugin::OnCmdReceived(const std_msgs::Float32::ConstPtr &msg)
{
  // Guardar el comando recibido para aplicarlo en la actualización
  cmdValue_ = msg->data;
}


void ignition::gazebo::ArmControlPlugin::Update(const UpdateInfo &info, EntityComponentManager &ecm)
{
  // Aplicar la posición objetivo a todos los joints
  for (auto &joint : rightArmJoints_)
  {
    // Establecer la posición objetivo basada en el valor del comando recibido
    ecm.SetComponentData<components::JointPosition>(joint, {cmdValue_});
  }

  for (auto &joint : leftArmJoints_)
  {
    // Establecer la posición objetivo basada en el valor del comando recibido
    ecm.SetComponentData<components::JointPosition>(joint, {cmdValue_});
  }
}

// Aquí se realiza el registro del plugin
IGNITION_ADD_PLUGIN(
    ignition::gazebo::ArmControlPlugin,
    ignition::gazebo::System,
    ignition::gazebo::ISystemConfigure,
    ignition::gazebo::ISystemUpdate)

IGNITION_ADD_PLUGIN_ALIAS(ignition::gazebo::ArmControlPlugin, "ignition::gazebo::ArmControlPlugin")