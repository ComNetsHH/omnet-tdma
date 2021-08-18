
#include "TdmaScheduler.h"

Define_Module(TdmaScheduler);

TdmaScheduler::TdmaScheduler() {

}

TdmaScheduler::~TdmaScheduler() {
    cancelAndDelete(schedulingSelfMessage);
}

void TdmaScheduler::initialize(int stage) {
    frameLength = par("frameLength");
    slotDuration = par("slotDuration");
    frameDuration = slotDuration * frameLength;

    scheduleSignal = registerSignal("schedule");
    utilizationSignal = registerSignal("utilization");

    schedulingSelfMessage = new cMessage("scheduling");
    slotSelfMessage = new cMessage("slot");
    WATCH(utilization);

    // start first scheduling during the last slot of the current frame
    scheduleAt(frameDuration - 0.5 * slotDuration, schedulingSelfMessage);
    if(par("monitorSchedule")) {
        scheduleAt(frameDuration, slotSelfMessage);
    }
}

int TdmaScheduler::registerClient(TdmaMac *mac, int status) {
    Enter_Method_Silent();
    int nodeId = numNodes++;

    bufferStatus.insert(make_pair(nodeId, status));
    clients.insert(make_pair(nodeId, mac));

    EV << "TdmaScheduler: Registered " << mac->getName() << " as Node #" << nodeId << " with BSR " << status << endl;
    return nodeId;
}

void TdmaScheduler::createSchedule() {
    schedule.resize(0, -1);
    schedule.resize(frameLength, -1);

    vector<int> nodeIds;
    int slotIndex = 0;
    int bufferSum = 0;

    for(auto it = bufferStatus.begin(); it != bufferStatus.end(); ++it) {
        bufferSum = bufferSum + it->second;
        nodeIds.push_back(it->first);
    }

    int clientIndex = intuniform(0, nodeIds.size()-1);
    EV << "CIdx: " << clientIndex << endl;
    EV << "nodeIds: " << nodeIds.size() << endl;
    EV << "BUFFERSUM: " << bufferSum << endl;

    while(slotIndex < frameLength && bufferSum != 0) {
        int nodeId = nodeIds[clientIndex];
        if(bufferStatus[nodeId] > 0) {
            schedule[slotIndex] = nodeId;
            slotIndex++;
            bufferSum--;
        }
        clientIndex = (clientIndex +1) % nodeIds.size();
    }

    for(int i = 0; i < nodeIds.size(); i++) {
        int nodeId = nodeIds[i];
        clients[nodeId]->setSchedule(schedule);
    }
    utilization = (double)slotIndex / (double)frameLength;
    emit(utilizationSignal, utilization);


    EV << "TdmaScheduler: End scheduling:" << endl;
    EV << "    Slot    |   nodeId" << endl;
    EV << "------------+--------------" << endl;
    for(int i = 0; i< schedule.size(); i++) {
        EV << "       "  << i <<  "    |   " << schedule[i] << endl;
    }

}

void TdmaScheduler::handleMessage(cMessage *message) {
    if(message == schedulingSelfMessage) {

        EV << "TdmaScheduler: Start scheduling" << endl;
        createSchedule();
        scheduleAt(simTime() + frameDuration, schedulingSelfMessage);
    } else {
        if(schedule[slotIndex] != -1) {
            emit(scheduleSignal, schedule[slotIndex]);
        }
        slotIndex = (slotIndex +1) % frameLength;
        scheduleAt(simTime() + slotDuration, slotSelfMessage);
    }
}


void TdmaScheduler::reportBufferStatus(int nodeId, int status) {
    Enter_Method_Silent();
    EV << "TdmaScheduler: Node #" << nodeId << " reported a BS of " << status << endl;
    bufferStatus[nodeId] = status;
}



