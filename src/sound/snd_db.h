#pragma once

void __cdecl SND_ParseInit(
        unsigned int groupCount,
        const snd_group *groups,
        unsigned int curveCount,
        const snd_curve *curves,
        unsigned int panCount,
        const snd_pan *pans,
        unsigned int snapshotGroupCount,
        const snd_snapshot_group *snapshotGroups);
void __cdecl SND_AliasGetFileName(const snd_alias_t *alias, char *filename, int size);
