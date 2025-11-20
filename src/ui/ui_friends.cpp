#include "ui_friends.h"

void __cdecl UI_FriendsRegisterDvars()
{
  ui_friendsListOpen = _Dvar_RegisterBool("ui_friendsListOpen", 0, 0, "True when viewing Friends List.");
  ui_playerListOpen = _Dvar_RegisterBool("ui_playerListOpen", 0, 0, "True when viewing recent met player List.");
  ui_leaderboardOpen = _Dvar_RegisterBool("ui_leaderboardOpen", 0, 0, "True when viewing leaderboards");
  ui_lobbyLeaderBoard = _Dvar_RegisterBool(
                          "ui_lobbyLeaderBoard",
                          0,
                          0,
                          "True when we open leaderboard through lobby leaderboard.");
  ui_playercardOpen = _Dvar_RegisterBool("ui_playercardOpen", 0, 0, "True when in game gamercard is open.");
  ui_clancardOpen = _Dvar_RegisterBool("ui_clancardOpen", 0, 0, "True when in game clancard is open.");
  ui_friendCountNotZero = _Dvar_RegisterBool("ui_friendCountNotZero", 0, 0, "Whether the player has any friends.");
  ui_storeOfferCountNotZero = _Dvar_RegisterBool("ui_storeOfferCountNotZero", 0, 0, "Whether the store has any offers.");
  ui_leaderboardFeederCountNotZero = _Dvar_RegisterBool(
                                       "ui_leaderboardFeederCountNotZero",
                                       0,
                                       0,
                                       "Whether the leaderboard has any entry");
  ui_clanName = _Dvar_RegisterString("ui_clanName", (char *)&toastPopupTitle, 0, "Name of player's clan.");
  ui_clanDescription = _Dvar_RegisterString(
                         "ui_clanDescription",
                         (char *)&toastPopupTitle,
                         0,
                         "Description of player's clan.");
  ui_onlineClanCount = _Dvar_RegisterInt(
                         "ui_onlineClanCount",
                         0,
                         0x80000000,
                         0x7FFFFFFF,
                         0,
                         "Number of clan members online.");
  ui_clanRecommendationCount = _Dvar_RegisterInt(
                                 "ui_clanRecommendationCount",
                                 0,
                                 0x80000000,
                                 0x7FFFFFFF,
                                 0,
                                 "Number of players recommended to clan");
  ui_totalClanMemberCount = _Dvar_RegisterInt(
                              "ui_totalClanMemberCount",
                              0,
                              0x80000000,
                              0x7FFFFFFF,
                              0,
                              "Total number of players who are part of the clan.");
  ui_incomingProposalCount = _Dvar_RegisterInt(
                               "ui_incomingProposalCount",
                               0,
                               0x80000000,
                               0x7FFFFFFF,
                               0,
                               "Number of incoming clan requests");
  ui_clanCountNotZero = _Dvar_RegisterBool("ui_clanCountNotZero", 0, 0, "Whether the player has any clan members.");
  ui_recentPlayerCountNotZero = _Dvar_RegisterBool(
                                  "ui_recentPlayerCountNotZero",
                                  0,
                                  0,
                                  "Whether the player has any recent players.");
  ui_groupsCountNotZero = _Dvar_RegisterBool(
                            "ui_groupsCountNotZero",
                            0,
                            0,
                            "Whether whether the player is part of any group");
  ui_recommendedCountNotZero = _Dvar_RegisterBool(
                                 "ui_recommendedCountNotZero",
                                 0,
                                 0,
                                 "Whether the clan has any recommended players.");
  ui_incomingProposalCountNotZero = _Dvar_RegisterBool(
                                      "ui_incomingProposalCountNotZero",
                                      0,
                                      0,
                                      "Whether there are any incoming proposals");
  ui_dropdownListMax = _Dvar_RegisterInt(
                         "ui_dropdownListMax",
                         0,
                         0x80000000,
                         0x7FFFFFFF,
                         0,
                         "Number of elements in list that opened dropdown.");
  ui_dropdownMax = _Dvar_RegisterInt(
                     "ui_dropdownMax",
                     0,
                     0x80000000,
                     0x7FFFFFFF,
                     0,
                     "Number of elements in the dropdown.");
  ui_dropdownOffset = _Dvar_RegisterInt(
                        "ui_dropdownOffset",
                        0,
                        0x80000000,
                        0x7FFFFFFF,
                        0,
                        "Used to calculate the Y offset of the dropdown.");
  ui_dropdownButtonOffset = _Dvar_RegisterInt(
                              "ui_dropdownButtonOffset",
                              0,
                              0x80000000,
                              0x7FFFFFFF,
                              0,
                              "Used to calculate the Y offset of the dropdown buttons.");
}

void __cdecl UI_FriendsUpdate()
{
  if ( ui_listboxIndex->current.integer <= ui_dropdownListMax->current.integer - ui_dropdownMax->current.integer - 1 )
  {
    Dvar_SetIntIfChanged((dvar_s *)ui_dropdownOffset, ui_listboxIndex->current.integer);
    Dvar_SetIntIfChanged((dvar_s *)ui_dropdownButtonOffset, ui_listboxIndex->current.integer);
  }
  else
  {
    Dvar_SetIntIfChanged(
      (dvar_s *)ui_dropdownOffset,
      ui_listboxIndex->current.integer - ui_dropdownMax->current.integer);
    Dvar_SetIntIfChanged(
      (dvar_s *)ui_dropdownButtonOffset,
      ui_listboxIndex->current.integer - ui_dropdownMax->current.integer - 1);
  }
}

