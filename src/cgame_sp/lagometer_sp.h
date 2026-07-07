#pragma once

struct lagometer_t
{
    int frameSamples[128];
    int frameCount;
    int snapshotFlags[128];
    int snapshotSamples[128];
    int entitySamples[128];
    int changedEntitySamples[128];
    int archivedEntitySamples[128];
    int serverBandwidth[128];
    int killcamTimes[128];
    int snapshotCount;
};
