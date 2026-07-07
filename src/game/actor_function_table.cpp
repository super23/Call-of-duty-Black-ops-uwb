#include "actor.h"

#include "actor_cover.h"
#include "actor_dog_exposed.h"
#include "actor_exposed.h"
#include "actor_generic.h"
#include "actor_grenade.h"
#include "actor_death.h"
#include "actor_pain.h"
#include "actor_negotiation.h"
#include "actor_badplace.h"
#include "actor_animscripted.h"
#ifdef KISAK_SP
#include "actor_react.h"
#endif

const ai_funcs_t AIDogFuncTable[12] =
{
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
  {
    Actor_Dog_Exposed_Start,
    Actor_Dog_Exposed_Finish,
    Actor_Dog_Exposed_Suspend,
    Actor_Generic_Resume,
    Actor_Dog_Exposed_Think,
    Actor_Exposed_Touch,
    Actor_Generic_Pain
  },
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
  {
    Actor_BadPlace_Flee_Start,
    Actor_BadPlace_Flee_Finish,
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
    Actor_BadPlace_Flee_Think,
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
  {
    Actor_Death_Start,
    Actor_BadPlace_Flee_Finish,
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
    Actor_Death_Think,
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
    Actor_Pain_Start,
    Actor_Pain_Finish,
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
    Actor_Pain_Think,
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
#ifdef KISAK_SP
    Actor_React_Start,
    Actor_React_Finish,
#else
    NULL,
    NULL,
#endif
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
#ifdef KISAK_SP
    Actor_React_Think,
#else
    NULL,
#endif
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
#ifdef KISAK_SP
    Actor_ScriptedAnim_Start,
    Actor_ScriptedAnim_Finish,
#else
    NULL,
    NULL,
#endif
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
#ifdef KISAK_SP
    Actor_ScriptedAnim_Think,
#else
    NULL,
#endif
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
#ifdef KISAK_SP
    Actor_CustomAnim_Start,
#else
    NULL,
#endif
    Actor_BadPlace_Flee_Finish,
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
#ifdef KISAK_SP
    Actor_CustomAnim_Think,
#else
    NULL,
#endif
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
    Actor_Negotiation_Start,
    Actor_BadPlace_Flee_Finish,
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
    Actor_Negotiation_Think,
    Actor_Generic_Touch,
    Actor_Generic_Pain
  }
};

const ai_funcs_t AIHumanFuncTable[12] =
{
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
  {
    Actor_Exposed_Start,
    Actor_Exposed_Finish,
    Actor_Generic_Suspend,
    Actor_Exposed_Resume,
    Actor_Exposed_Think,
    Actor_Exposed_Touch,
    Actor_Generic_Pain
  },
  { NULL, NULL, NULL, NULL, NULL, NULL, NULL },
  {
    Actor_Grenade_Start,
    Actor_Grenade_Finish,
    Actor_Grenade_Suspend,
    Actor_Grenade_Resume,
    Actor_Grenade_Think,
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
    Actor_BadPlace_Flee_Start,
    Actor_BadPlace_Flee_Finish,
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
    Actor_BadPlace_Flee_Think,
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
    Actor_CoverArrival_Start,
    Actor_BadPlace_Flee_Finish,
    Actor_Generic_Suspend,
    Actor_CoverArrival_Resume,
    Actor_CoverArrival_Think,
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
    Actor_Death_Start,
    Actor_BadPlace_Flee_Finish,
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
    Actor_Death_Think,
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
    Actor_Pain_Start,
    Actor_Pain_Finish,
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
    Actor_Pain_Think,
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
#ifdef KISAK_SP
    Actor_React_Start,
    Actor_React_Finish,
#else
    NULL,
    NULL,
#endif
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
#ifdef KISAK_SP
    Actor_React_Think,
#else
    NULL,
#endif
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
#ifdef KISAK_SP
    Actor_ScriptedAnim_Start,
    Actor_ScriptedAnim_Finish,
#else
    NULL,
    NULL,
#endif
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
#ifdef KISAK_SP
    Actor_ScriptedAnim_Think,
#else
    NULL,
#endif
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
#ifdef KISAK_SP
    Actor_CustomAnim_Start,
#else
    NULL,
#endif
    Actor_BadPlace_Flee_Finish,
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
#ifdef KISAK_SP
    Actor_CustomAnim_Think,
#else
    NULL,
#endif
    Actor_Generic_Touch,
    Actor_Generic_Pain
  },
  {
    Actor_Negotiation_Start,
    Actor_BadPlace_Flee_Finish,
    Actor_Generic_Suspend,
    Actor_Generic_Resume,
    Actor_Negotiation_Think,
    Actor_Generic_Touch,
    Actor_Generic_Pain
  }
};

#ifdef KISAK_SP
const ai_funcs_t *AIFuncTable[4] =
{
    AIHumanFuncTable,
    AIDogFuncTable,
    AIHumanFuncTable,
    AIDogFuncTable,
};
#else
const ai_funcs_t *AIFuncTable[1] = { AIDogFuncTable };
#endif
