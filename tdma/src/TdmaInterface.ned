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

import tdma.mac.TdmaMac;
import tdma.rlc.Rlc;
import tdma.rlc.IRlc;
import inet.queueing.contract.IPacketQueue;
import inet.linklayer.contract.IWirelessInterface;
import inet.networklayer.common.InterfaceEntry;
import inet.physicallayer.contract.packetlevel.IRadio;

package tdma;

//
// TODO auto-generated type
//
network TdmaInterface extends InterfaceEntry like IWirelessInterface
{
    parameters:
        @class(::inet::InterfaceEntry);
        @display("i=block/ifcard");
        string interfaceTableModule;
        string energySourceModule = default("");
        double bitrate @unit(bps);
        *.interfaceTableModule = default(absPath(interfaceTableModule));
        *.energySourceModule = default(absPath(energySourceModule));
        **.bitrate = bitrate;
    gates:
        input upperLayerIn;
        output upperLayerOut;
        input radioIn @labels(Signal);
    submodules:
        rlc: <default("Rlc")> like IRlc {
            parameters:
                @display("p=100,100");
        }
        mac: TdmaMac {
            parameters:
                @display("p=100,200");
        }
        radio: <default("UnitDiskRadio")> like IRadio {
            parameters:
                @display("p=100,300");
        }
    connections:
        upperLayerIn --> rlc.upperLayerIn;
        rlc.upperLayerOut --> upperLayerOut;
        rlc.lowerLayerOut --> mac.upperLayerIn;
        mac.upperLayerOut --> rlc.lowerLayerIn;
        mac.lowerLayerOut --> radio.upperLayerIn;
        radioIn --> { @display("m=s"); } --> radio.radioIn;
        radio.upperLayerOut --> mac.lowerLayerIn;
}


