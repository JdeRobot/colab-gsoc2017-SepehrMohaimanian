/*
 *  Copyright (C) 1997-2016 JDE Developers Team
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see http://www.gnu.org/licenses/.
 *  Authors :
 *       Aitor Martinez Fernandez <aitor.martinez.fernandez@gmail.com>
 */
#include <jderobot/comm/cameraClient.hpp>
#include <jderobot/comm/ice/cameraIceClient.hpp>
#ifdef JDERROS
#include <jderobot/comm/ros/listenerCamera.hpp>
#endif

namespace JdeRobotComm {

CameraClient* 
getCameraClient(Ice::CommunicatorPtr ic, std::string prefix){
	CameraClient* client = 0;
	Ice::PropertiesPtr prop = ic->getProperties();


	int server = prop->getPropertyAsIntWithDefault(prefix+".Server",0);
	switch (server){
		case 0:
		{
			std::cout << "Camera disabled" << std::endl;
			break;
		}
		case 1:
		{
			std::cout << "Receiving Image from ICE interfaces" << std::endl;
			CameraIceClient* cl;
			cl = new CameraIceClient(ic, prefix);
			cl->start();
		    client = (JdeRobotComm::CameraClient*) cl;
		    break;
		}
		case 2:
		{
			#ifdef JDERROS
				std::cout << "Receiving Image from ROS messages" << std::endl;
				std::string nodeName;
				nodeName =  prop->getPropertyWithDefault(prefix+".Name","CameraNode");
				std::string topic;
				topic = prop->getPropertyWithDefault(prefix+".Topic","");
				ListenerCamera* lc;
				lc = new ListenerCamera(0, nullptr, nodeName, topic);
				lc->start();
				client = (JdeRobotComm::CameraClient*) lc;
            #else
				throw "ERROR: ROS is not available";
			#endif

		 	break;
		}
		default:
		{
			std::cerr << "Wrong " + prefix+".Server property" << std::endl;
			break;
		}

	}

	return client;


}

}//NS