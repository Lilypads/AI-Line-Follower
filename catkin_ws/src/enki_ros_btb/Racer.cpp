/*
    Enki - a fast 2D robot simulator
    Copyright (C) 2017 Bernd Porr
    Copyright (C) 1999-2016 Stephane Magnenat <stephane at magnenat dot net>
    Copyright (C) 2004-2005 Markus Waibel <markus dot waibel at epfl dot ch>
    Copyright (c) 2004-2005 Antoine Beyeler <abeyeler at ab-ware dot com>
    Copyright (C) 2005-2006 Laboratory of Intelligent Systems, EPFL, Lausanne
    Copyright (C) 2006-2008 Laboratory of Robotics Systems, EPFL, Lausanne
    See AUTHORS for details

    This program is free software; the authors of any publication 
    arising from research using this software are asked to add the 
    following reference:
    Enki - a fast 2D robot simulator
    http://home.gna.org/enki
    Stephane Magnenat <stephane at magnenat dot net>,
    Markus Waibel <markus dot waibel at epfl dot ch>
    Laboratory of Intelligent Systems, EPFL, Lausanne.

    You can redistribute this program and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "Racer.h"

using namespace std;
namespace Enki
{
    Racer::Racer(int _nSensors):
		DifferentialWheeled(5.2, 100, 0.05),
        infraredSensorLeft(this, Vector(11, 10), 100, M_PI/200, 2, 1200, -0.9, 7, 20),
        infraredSensorRight(this, Vector(11, -10), 100, -M_PI/200, 2, 1200, -0.9, 7, 20),
		camera(this, Vector(0, 0), 0, 0.0, M_PI/4, 50),
		rgbcamera(this, Vector(11, 0), 0.5, 0.0, M_PI/8, 81) //robot, offset pos, height, orientation, halfFieldOfView, pixelCount

        //groundSensorLeft (this, Vector(10, SENSORLEFT), 0, 1, 1, 0),
        //groundSensorRight(this, Vector(10, SENSORRIGHT),0, 1, 1, 0)
    {

        nSensors = _nSensors;
        if (nSensors%2) {
            fprintf(stderr,"BUG: nSensors needs to be even.\n");

        }

	    

        for(int i=0; i<8; i++){
            if(i<4){
                lineSensors[i] = new GroundSensor(this, Vector(10, int(8-i*2)), 0,1,1,0);
            }else{
                lineSensors[i] = new GroundSensor(this, Vector(10, int((i*-2)+6)), 0,1,1,0);
            }
            addLocalInteraction(lineSensors[i]);
        }

        //groundSensorArray = new GroundSensor*[nSensors];
        groundSensorArray = new GroundSensor*[81];
        
        addLocalInteraction(&infraredSensorLeft);
        addLocalInteraction(&infraredSensorRight);
		addLocalInteraction(&camera);
		addLocalInteraction(&rgbcamera);
		//addLocalInteraction(&groundSensorLeft);
		//addLocalInteraction(&groundSensorRight);

        setRectangular(20,10,5, 80); // length, width, height, mass of the robot
        setColor(Enki::Color(0.5,0.5));
    }

    void Racer::setPreds(int _predPos, int _nPred, double _spacing){
        double startPoint=((_nPred -1) * _spacing)/2;
        int j=0;
        for (int i=iterate; i<_nPred+iterate; i++){            
            //groundSensorArray[i]= new GroundSensor(this, Vector(_predPos , -startPoint + j * _spacing), 0, 1, 1, -0.731059,2);
            groundSensorArray[i]= new GroundSensor(this, Vector(_predPos , -startPoint + j * _spacing), 0, 1, 1, 0);
            //cout << "i is:  "<< i <<"  position is:  "<< _predPos << "  and  " <<-startPoint + j * _spacing << "  n  "<< _nPred << "  s  " << _spacing <<endl;
            addLocalInteraction(groundSensorArray[i]);


	  
            j++;
        }
        iterate += _nPred;
    }
}
