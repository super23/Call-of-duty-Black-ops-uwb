#pragma once

XAnimClientNotifyList *__thiscall XAnimClientNotifyList::XAnimClientNotifyList(XAnimClientNotifyList *this);
void __thiscall XAnimClientNotifyList::~XAnimClientNotifyList(XAnimClientNotifyList *this);
XAnimClientNotifyList *__thiscall XAnimClientNotifyList::GetNotifyList(XAnimClientNotifyList *this);
void __thiscall XAnimClientNotifyList::AddNotify(XAnimClientNotifyList *this, const ClientNotifyData *notifyData);
XAnimClientNotify *__thiscall XAnimClientNotify::XAnimClientNotify(
        XAnimClientNotify *this,
        const ClientNotifyData *notifyData);
void __thiscall XAnimClientNotify::~XAnimClientNotify(XAnimClientNotify *this);
void __thiscall XAnimClientNotify::swap(XAnimClientNotify *this, XAnimClientNotify *otherNotify);
bool __thiscall XAnimClientNotify::IsClientAnimNotify(XAnimClientNotify *this);
const char *__thiscall XAnimClientNotify::GetNotifyStringName(XAnimClientNotify *this);
unsigned int __thiscall XAnimClientNotify::GetNotetrackCLName(XAnimClientNotify *this);
unsigned int __thiscall XAnimClientNotify::GetNotifyType(XAnimClientNotify *this);
unsigned int __thiscall XAnimClientNotify::GetNotifyName(XAnimClientNotify *this);
