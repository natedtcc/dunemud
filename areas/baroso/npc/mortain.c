/*
** Mortain - Atreides Weapon Smith
** by Kiza 24-Jul-1996
** upgraded by Bebop July 2018
*/

inherit "/d/Atreid/benedict/lib/atreides.c";
#define WIELD(x) weapons[x]->wield(weapons[x]->query_name())
#define WEAR(x) armors[x]->wear(armors[x]->query_name())

#include "/d/Imperial/boss/sal_streets/npc/mob_powerup.h"

void reset(int arg)
{
  object wep, arm;
  ::reset(arg);
  if (arg)
    return;
  set_name("mortain");
  set_race("Atreides");
  set_alias(({"atreid", "atreides"}));
  set_guild_level(1);
  set_level(340);
  add_money(30000 + random(20000));
  set_hei(20);
  set_skill("Long blade weapons", 1200);
  set_short("$CYANMortain$OFF the Atreides Weapon Smith");
  set_long(
      "Mortain is a large looking man, who is the most experienced weapon " +
      "smith\nof the Atreides. He also looks incredibly tough.\n\n");
  set_gender(1);

  wep = clone_object("/d/Atreid/kiza/ubase/obj/longblade");
  if (wep)
  {
    move_object(wep, this_object());
    wep->set_unique(1);
  }

  arm = clone_object("/d/Atreid/kiza/ubase/obj/uniform2");
  if (arm)
  {
    move_object(arm, this_object());
  }

  do_mob_powerup();

  return;
}
