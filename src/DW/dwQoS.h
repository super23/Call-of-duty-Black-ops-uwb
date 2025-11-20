#pragma once

const bdReference<bdCommonAddr> *__thiscall bdReference<bdCommonAddr>::operator=(
        bdReference<bdCommonAddr> *this,
        const bdReference<bdCommonAddr> *other);
dwQoSMultiProbeListener *__thiscall dwQoSMultiProbeListener::dwQoSMultiProbeListener(dwQoSMultiProbeListener *this);
void __thiscall dwQoSMultiProbeListener::addProbe(dwQoSMultiProbeListener *this, bdQoSRemoteAddr addr);
void __thiscall dwQoSMultiProbeListener::onQoSProbeSuccess(dwQoSMultiProbeListener *this, const bdQoSProbeInfo *info);
bdQoSProbeInfo *__thiscall bdQoSProbeInfo::operator=(bdQoSProbeInfo *this, const bdQoSProbeInfo *__that);
void __thiscall dwQoSMultiProbeListener::onQoSProbeFail(dwQoSMultiProbeListener *this, bdReference<bdCommonAddr> addr);
void __thiscall dwQoSMultiProbeListener::clean(dwQoSMultiProbeListener *this);
int __thiscall dwQoSMultiProbeListener::complete(dwQoSMultiProbeListener *this);
bdQoSRemoteAddr *__thiscall bdQoSRemoteAddr::bdQoSRemoteAddr(bdQoSRemoteAddr *this, const bdQoSRemoteAddr *__that);
void __cdecl dwStartQoSProbes(dwQoSMultiProbeListener *listener, unsigned int numProbes, bdQoSRemoteAddr *serverAddrs);
void __cdecl dwClearQoSProbes();
void __thiscall bdReference<bdCommonAddr>::operator=(bdReference<bdRemoteTask> *this, bdRemoteTask *p);
