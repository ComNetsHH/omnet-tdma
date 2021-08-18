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

package tdma.scheduler;

//
// TODO auto-generated type
//
module TdmaScheduler
{
    parameters:
        bool monitorSchedule = default(false);
        double slotDuration @unit(s) = default(2ms);
        int frameLength = default(10);
    	@class(TdmaScheduler);
    	
        @signal[schedule](type=int);
        @signal[utilization](type=double);
        @statistic[schedule](title="schedule"; record=vector,histogram; interpolationmode=none);
        @statistic[utilization](title="utilization"; record=vector,histogram,timeavg; interpolationmode=none);
}
