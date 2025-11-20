#include "snd_db.h"

void __cdecl SND_ParseInit(
        unsigned int groupCount,
        const snd_group *groups,
        unsigned int curveCount,
        const snd_curve *curves,
        unsigned int panCount,
        const snd_pan *pans,
        unsigned int snapshotGroupCount,
        const snd_snapshot_group *snapshotGroups)
{
  unsigned int n; // [esp+0h] [ebp-14h]
  unsigned int i; // [esp+4h] [ebp-10h]
  unsigned int m; // [esp+8h] [ebp-Ch]
  unsigned int k; // [esp+Ch] [ebp-8h]
  unsigned int j; // [esp+10h] [ebp-4h]

  if ( groupCount >= 0x40
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_db.cpp",
          645,
          0,
          "%s",
          "groupCount < SND_MAX_GROUPS") )
  {
    __debugbreak();
  }
  if ( curveCount >= 0x20
    && !Assert_MyHandler(
          "C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_db.cpp",
          646,
          0,
          "%s",
          "curveCount < SND_MAX_CURVES") )
  {
    __debugbreak();
  }
  memset((unsigned __int8 *)GROUPS, 0, sizeof(GROUPS));
  for ( j = 0; j < groupCount; ++j )
    GROUPS[j] = groups[j].name;
  memset((unsigned __int8 *)PANS, 0, sizeof(PANS));
  for ( k = 0; k < panCount; ++k )
    PANS[k] = pans[k].name;
  memset((unsigned __int8 *)CURVES, 0, sizeof(CURVES));
  for ( m = 0; m < curveCount; ++m )
    CURVES[m] = curves[m].name;
  memset(&byte_E14B50, 0, 0x800u);
  for ( i = 0; i < snapshotGroupCount; ++i )
  {
    if ( &snapshotGroups[i] && snapshotGroups[i].name[0] )
    {
      SND_SNAPSHOT_FIELDS[i + 9].name = snapshotGroups[i].name;
      dword_E14A34[8 * i + 72] = 4 * i + 92;
      byte_E14A38[32 * i + 288] = 1;
      dword_E14A3C[8 * i + 72] = 1;
      dword_E14A40[8 * i + 72] = *(unsigned int *)&FLOAT_0_0;
      dword_E14A44[8 * i + 72] = LODWORD(FLOAT_1_0);
      dword_E14A48[8 * i + 72] = 0;
      dword_E14A4C[8 * i + 72] = 0;
    }
    else
    {
      SND_SNAPSHOT_FIELDS[i + 9].name = 0;
    }
  }
  memset((unsigned __int8 *)SNAPSHOT_GROUPS, 0, sizeof(SNAPSHOT_GROUPS));
  for ( n = 0; n < snapshotGroupCount; ++n )
    SNAPSHOT_GROUPS[n] = snapshotGroups[n].name;
}

void __cdecl SND_AliasGetFileName(const snd_alias_t *alias, char *filename, int size)
{
  if ( !alias && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_db.cpp", 735, 0, "%s", "alias") )
    __debugbreak();
  if ( !alias->soundFile
    && !Assert_MyHandler("C:\\projects_pc\\cod\\codsrc\\src\\sound\\snd_db.cpp", 736, 0, "%s", "alias->soundFile") )
  {
    __debugbreak();
  }
  if ( alias->soundFile->type == 1 )
    I_strncpyz(filename, alias->soundFile->u.loadSnd->name, size);
  else
    Com_sprintf(filename, size, "%s", alias->soundFile->u.loadSnd->name);
}

