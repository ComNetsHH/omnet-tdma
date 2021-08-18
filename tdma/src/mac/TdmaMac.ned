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

import inet.linklayer.acking.AckingMac;

package tdma.mac;

//
// TODO auto-generated type
//
module TdmaMac extends AckingMac
{
    parameters:
        string scheduler = "^.^.^.scheduler";
        double slotDuration @unit(s) = default(2ms);
        int frameLength = default(10);
        int numRetries = default(3);
        
        @class(TdmaMac);
        
}
