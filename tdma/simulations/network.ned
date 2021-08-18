//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 
import tdma.scheduler.TdmaScheduler;
import inet.node.inet.AdhocHost;
import inet.networklayer.configurator.ipv4.Ipv4NetworkConfigurator;
import inet.physicallayer.unitdisk.UnitDiskRadioMedium;
import inet.visualizer.contract.IIntegratedVisualizer;
import inet.node.inet.AdhocHost;
import inet.physicallayer.unitdisk.UnitDiskRadioMedium;

package tdma.simulations;

//
// TODO auto-generated type
//
network Network
{
    parameters:
        int numNodes = default(4);
    submodules:
    	nodes[numNodes]: AdhocHost {}
    	scheduler: TdmaScheduler {
            parameters:
                @display("p=100,400;is=s");
        }
    	//visualizer: <default("IntegratedCanvasVisualizer")> like IIntegratedVisualizer if hasVisualizer() {
       	//	parameters:
        //        @display("p=100,300;is=s");
        //}
        configurator: Ipv4NetworkConfigurator {
            parameters:
                @display("p=100,100;is=s");
        }
        radioMedium: UnitDiskRadioMedium {
            parameters:
                @display("p=100,200;is=s");
        }
}
