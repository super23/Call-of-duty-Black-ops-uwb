#pragma once

void __cdecl GraphFloat_Load(GraphFloat *graph, char *fileName, float scale);
void __cdecl GraphFloat_ParseBuffer(GraphFloat *graph, const char *buffer, const char *fileName);
GraphFloat *__cdecl GraphFloat_Load(char *fileName);
double __cdecl GraphFloat_GetValue(const GraphFloat *graph, float fraction);
void __cdecl GraphFloat_CreateDevGui(GraphFloat *graph, const char *devguiPath);
void __cdecl GraphFloat_DevGuiCB_Event(const DevGraph *graph, DevEventType event);
void __cdecl GraphFloat_SaveToFile(const GraphFloat *graph);
void __cdecl GraphFloat_DevGuiCB_Text(const DevGraph *devGuiGraph, float inputX, float inputY, char *text);
