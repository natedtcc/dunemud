/*
** Kalneric - Armour Lab
** by Kiza 23-Jul-1996
** upgraded by Bebop July 2018
*/

/* History:
** 09/jan/2002 Kiza - updated to use Boss's mob powerup code
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

  set_name("kalneric");
  set_race("Atreides");
  set_alias(({"atreid", "atreides"}));
  set_level(330);
  set_hei(20);
  set_guild_level(1);
  add_money(30000 + random(20000));
  set_skill("Lasers", 1000);
  set_short("$CYANKalneric$OFF the Atreides Armourer");
  set_long("\
    Kalneric is a middle aged man, who's experience in armours is unsurpassed.\
    \nHe wears an armour of his own design, which looks extremly tough.\n\n");
  set_gender(1);

  wep = clone_object("/d/Atreid/kiza/ubase/obj/lasgun2.c");
  if (wep)
  {
    move_object(wep, this_object());
    wep->set_unique(1);
  }

  arm = clone_object("/d/Atreid/kiza/ubase/obj/blackjacket.c");
  if (arm)
  {
    move_object(arm, this_object());
  }

  do_mob_powerup();

  return;
}
