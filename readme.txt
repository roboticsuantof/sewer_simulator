# DEPENDENCIES

## IGNITION GAZEBO AND BRIEDGE

	To instal  Ignition Gazebo Fortress, you must fallow the instruction provided in https://gazebosim.org/docs/fortress/install_ubuntu_src/

	Later, to communicate ROS with ignition Fortress, is necesary to install package ros_ign_briege. We have two option:

		Fist, using source, which instruction are provided in https://github.com/gazebosim/ros_gz/tree/noetic.
		Second, is installing:
			- sudo apt install ros-noetic-ros-ign-bridge
			- sudo apt install ros-noetic-ros-ign-gazebo


## PACKAGES

For the correct performance of this simulator package, please install next dependencies:

// To use Joystick with ROS
	sudo apt-get install ros-noetic-joy
// Irterface Between Joystick and Ignition Simulator
	https://github.com/roboticsuantof/teleop_joy_pkg/

// For Localization
	https://github.com/robotics-upo/dll.git 
//For Mapping
 sudo apt-get install ros-noetic-map-server
	
## HOW ITs WORK SIMULATION TELEOP

The main launch for the simulation and teleop the UGV is :

	roslaunch sewer_simulator ign_sewer_simulation.launch

## HOW TO CREATE A MAP

Since it is necessary to have a tree of frames (TFs) from map frame (or ground frame ) to base_link frame, passing through odom frame, to ensure the operation of OCTOMAP node and thus, create a map, in this STACK we use A-LOAM as localization and odometry module. 

A-LOAM allow to generate conection beetween frame (or ground frame ) to odom frame. For that use:

First:
	roslaunch sewer_simulator ign_sewer_simulation.launch

Second:
	roslaunch sewer_simulator amapping_marble.launch

Third:
	You must navigate into de map

Fourth: you must save the map using

	roslaunch sewer_simulator ...........

